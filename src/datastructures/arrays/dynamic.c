#include <blib/datastructures/arrays/dynamic.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ==================
 * Convenience Macros
 * ================== */
#if DISABLE_RUNTIME_BOUNDS_CHECKS
#define check_index(self, operation, i)
#else
#define check_index(self, operation, i) \
  do {                                  \
    if (i >= self->num) {               \
      fprintf(stderr,                   \
          "Attempt to " operation " element %lu from dynamic array of element count %lu!\n" \
          "=== ABORT ===\n",            \
          i, self->num);                \
      abort();                          \
    }                                   \
  } while(0)
#endif

#if DISABLE_RUNTIME_BOUNDS_CHECKS
#define check_rm_len(self, operation, number)
#else
#define check_rm_len(self, operation, number) \
  do {                                        \
    if (number > self->num) {                 \
      fputs(                                  \
          "Attempt to " operation " a dynamic array of element count 0!\n" \
          "=== ABORT ===\n", stderr);         \
      abort();                                \
    } \
  } while (0)
#endif

#if DISABLE_RUNTIME_BOUNDS_CHECKS
#define check_len(self, operation, n)
#else
#define check_len(self, operation, n) \
  do {                                \
    if (n > self->num) {              \
      fprintf(stderr,                 \
        "Attempt to " operation " %lu element(s) from dynamic array of element count %lu!\n" \
        "=== ABORT ===\n",            \
        n, self->num);                \
      abort();                        \
    }                                 \
  while (0)
#endif

#if DISABLE_RUNTIME_BOUNDS_CHECKS
#define check_index_len(self, operation, i, n)
#else
#define check_index_len(self, operation, i, n) \
  do {                                         \
    if (i + n > self->num) {                   \
      fprintf(stderr,                          \
          "Attempt to " operation " %lu element(s) at index %lu from dynamic array of element count %lu!\n" \
          "=== ABORT ===\n",                   \
          n, i, self->num);                    \
      abort();                                 \
    }                                          \
  } while (0)
#endif

#define index2off(self, i) ((i + self->__deadzone__) * self->element_size)

#ifndef bug_notice
#define bug_notice                                                   \
          "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" \
          "!! NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE !!\n" \
          "!!       This may be a problem inside of blib.      !!\n" \
          "!!    Please submit a bug report at its git repo:   !!\n" \
          "!!        https://gihub.com/0xfadead/blib           !!\n" \
          "!!          Sorry for the inconvenience...          !!\n" \
          "!! NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE !!\n" \
          "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
#endif
/* ================== */

/* ==================================
 * Convenience Function Declaractions
 * ================================== */
void dynamic_arr_resize(dynamic_arr* self, long change, bool explicit_size);
void dynamic_arr_move(dynamic_arr* self, long change, unsigned long offset, unsigned long len);
void dynamic_arr_print(const dynamic_arr* self);
/* ================================== */

/* =============
 * API Functions
 * ============= */
dynamic_arr __intern_dynamic_generic_arr_new(unsigned int element_size) {
  dynamic_arr arr = {0};

  arr.element_size = element_size;
  arr.__cap__ = 1;

  arr.__malloc_start__ = malloc(arr.element_size);
  if (!arr.__malloc_start__) {
    fprintf(stderr, 
        "Failed to allocate initial memory for dynamic array of size %u: %s\n"
        "=== ABORT ===\n",
        arr.element_size, strerror(errno));

    abort();
  }

  return arr;
}

dynamic_arr dynamic_arr_copy(const dynamic_arr *src) {
  dynamic_arr dst = {
    .__cap__ = src->num,
    .num = src->num,
    .element_size = src->element_size,
    .__deadzone__ = 0
  };

  dst.__malloc_start__ = malloc(src->num * src->element_size);
  if (!dst.__malloc_start__) {
    fprintf(stderr,
        "Failed to allocate initial memory for dynamic array copy of array size %lu: %s\n"
        "=== ABORT ===\n",
        dst.num * dst.element_size, strerror(errno));

    abort();
  }

  memcpy(dst.__malloc_start__, src->__malloc_start__ + (src->__deadzone__ * src->element_size), src->num * src->element_size);

  return dst;
}

void* dynamic_arr_get_start(nonnullable const dynamic_arr* self) {
  return self->__malloc_start__ + index2off(self, 0);
} /* dynamic_arr_get_start */

void dynamic_arr_replace(dynamic_arr* self, unsigned long index, const void* element) {
  check_index(self, "replace", index);
  memcpy(self->__malloc_start__ + index2off(self, index), element, self->element_size);

  return;
} /* dynamic_arr_replace */

void dynamic_arr_bulk_replace(dynamic_arr* self, unsigned long index, const void* elements, unsigned long num) {
  check_index_len(self, "bulk-replace", index, num);
  memcpy(self->__malloc_start__ + index2off(self, index), elements, num * self->element_size);

  return;
} /* dynamic_arr_bulk_replace */

void dynamic_arr_set(nonnullable dynamic_arr* self, unsigned long index, nonnullable const void* element, unsigned long num) {
  check_index_len(self, "set", index, num);

  for (unsigned long i = index; i < num + index; i++)
    memcpy(self->__malloc_start__ + index2off(self, i), element, self->element_size);

  return;
} /* dynamic_arr_set */
void dynamic_arr_flip(dynamic_arr* self, unsigned long index1, unsigned long index2) {
  uint8_t* buffer = NULL;
  if (self->num < self->__cap__) {
    buffer = self->__malloc_start__ + index2off(self, self->num);
  } else if (self->__deadzone__) {
    buffer = self->__malloc_start__;
  } else {
    dynamic_arr_resize(self, 1, false);
    buffer = self->__malloc_start__ + (self->num * self->element_size);
  }

  dynamic_arr_peek(self, index1, buffer);
  dynamic_arr_replace(self, index1, self->__malloc_start__ + (index2 * self->element_size));
  dynamic_arr_replace(self, index2, buffer);

  return;
} /* dynamic_arr_flip */

void dynamic_arr_bulk_flip(dynamic_arr* self, unsigned long index1, unsigned long index2, unsigned long num) {
  check_index_len(self, "bulk-flip (1)", index1, num);
  check_index_len(self, "bulk-flip (2)", index2, num);
  if (!num)
    return;

  const unsigned long size = num * self->element_size;

  uint8_t* ptr = self->__malloc_start__ + index2off(self, self->num);
  unsigned long elems_size = (self->__cap__ - self->num) % (size + 1);
  if (elems_size)
    goto memory_cpy;

  ptr = self->__malloc_start__;
  elems_size = self->__deadzone__ % (size + 1);
  if (elems_size)
    goto memory_cpy;

  dynamic_arr_resize(self, size, false);
  ptr = self->__malloc_start__ + index2off(self, self->num);
  elems_size = size;

memory_cpy:
  while (num) {
    memcpy(ptr, self->__malloc_start__ + index2off(self, index1), size);
    memcpy(self->__malloc_start__ + index2off(self, index1), self->__malloc_start__ + index2off(self, index2), size);
    memcpy(self->__malloc_start__ + index2off(self, index2), ptr, size);

    index1 += num;
    index2 += num;

    num -= elems_size % (size + 1);
  }

  return;
} /* dynamic_arr_bulk_flip */

void dynamic_arr_peek(const dynamic_arr* self, unsigned long index, void* out) {
  check_index(self, "read", index);

  memcpy(out, self->__malloc_start__ + index2off(self, index), self->element_size);

  return;
} /* dynamic_arr_peek */

void dynamic_arr_bulk_peek(dynamic_arr* self, unsigned long index, void* out, unsigned long num) {
  check_index_len(self, "bulk-peek", index, num);
  
  memcpy(out, self->__malloc_start__ + index2off(self, index), num * self->element_size);

  return;
} /* dynamic_arr_bulk_peek */

void dynamic_arr_append(dynamic_arr* self, const void* element) {
  dynamic_arr_resize(self, 1, false);

  memcpy(self->__malloc_start__ + index2off(self, self->num), element, self->element_size);
  self->num++;
  return;
} /* dynamic_arr_append */

void dynamic_arr_bulk_append(dynamic_arr* self, const void* elements, unsigned long num) {
  dynamic_arr_resize(self, num, false);

  memcpy(self->__malloc_start__ + index2off(self, self->num), elements, num * self->element_size);
  self->num += num;

  return;
} /* dynamic_arr_bulk_append */

void dynamic_arr_prepend(dynamic_arr* self, const void* element) {
  if (self->__deadzone__) {
    self->__deadzone__--;
    memcpy(self->__malloc_start__ + index2off(self, 0), element, self->element_size);
  } else {
    dynamic_arr_resize(self, 1, false);
    dynamic_arr_move(self, 1, 0, self->num);
    memcpy(self->__malloc_start__ + index2off(self, 0), element, self->element_size);
  }

  self->num++;

  return;
} /* dynamic_arr_prepend */

void dynamic_arr_bulk_prepend(dynamic_arr* self, const void* elements, unsigned long num) {
  unsigned long deadzone_elems = num % (self->__deadzone__ + 1);
  unsigned long move_elems = num - deadzone_elems;

  dynamic_arr_resize(self, move_elems, false);
  dynamic_arr_move(self, move_elems, 0, self->num);
  memcpy(self->__malloc_start__ + index2off(self, 0), (uint8_t*)elements + ((num - move_elems) * self->element_size), move_elems);

  self->__deadzone__ -= deadzone_elems;
  memcpy(self->__malloc_start__ + index2off(self, 0), elements, deadzone_elems * self->element_size);

  self->num += num;

  return;
} /* dynamic_arr_bulk_prepend */

void dynamic_arr_insert_at(dynamic_arr* self, unsigned long index, const void* element) {
  check_index(self, "insert", index);

  if (self->__deadzone__ && self->num >> 1 >= index) {
    self->__deadzone__--;
    index++;

    dynamic_arr_move(self, -1, 0, index);

    memcpy(self->__malloc_start__ + index2off(self, index), element, self->element_size);
  } else {
    dynamic_arr_resize(self, 1, false);
    dynamic_arr_move(self, 1, index, self->num - index);

    memcpy(self->__malloc_start__ + index2off(self, index), element, self->element_size);
  }
  self->num++;

  return;
} /* dynamic_arr_insert_at */

void dynamic_arr_bulk_insert_at(dynamic_arr* self, unsigned long index, void* elements, unsigned long num) {
  check_index_len(self, "bulk-insert", index, num);
  
  const          uint8_t preferred_side   = self->num >> 1 < index;
  const unsigned long    deadzone_inserts = preferred_side * (num % (self->__deadzone__ + 1));
  const unsigned long    end_inserts      = num - deadzone_inserts;

  dynamic_arr_move(self, -deadzone_inserts, 0, index + 1);
  dynamic_arr_resize(self, end_inserts, false);
  dynamic_arr_move(self, end_inserts, index + 1, self->num - index);

  memcpy(self->__malloc_start__ + index2off(self, index - deadzone_inserts), elements, num * self->element_size);

  self->num += num;

  return;
} /* dynamic_arr_bulk_insert_at */

void dynamic_arr_precate(dynamic_arr* self, void* out) {
  check_rm_len(self, "precate", 1);

  if (out)
    memcpy(out, self->__malloc_start__ + index2off(self, 0), self->element_size);
  dynamic_arr_move(self, -1, 0, self->num);
  dynamic_arr_resize(self, -1, false);
  self->num--;

  return;
} /* dynamic_arr_precate */

void dynamic_arr_quick_precate(dynamic_arr *self, void *out) {
  if (out)
    memcpy(out, self->__malloc_start__ + index2off(self, 0), self->element_size);

  self->__deadzone__++;

  return;
} /* dynamic_arr_quick_precate */

void dynamic_arr_truncate(dynamic_arr *self, void *out) {
  check_rm_len(self, "truncate", 1);

  self->num--;
  if (out)
    memcpy(out, self->__malloc_start__ + index2off(self, self->num), self->element_size);
  dynamic_arr_resize(self, -1, false);
  self->num--;

  return;
} /* dynamic_arr_truncate */

void dynamic_arr_resize_to(dynamic_arr* self, void* out, unsigned long new_size) {
  if (out && new_size < self->num)
    memcpy(out, self->__malloc_start__ + index2off(self, new_size), (self->num - new_size) * self->element_size);

  self->num = new_size;

  dynamic_arr_resize(self, (new_size ? new_size : 1), true);

  return;
} /* dynamic_arr_resize_to */

void dynamic_arr_trim(dynamic_arr *self) {
  dynamic_arr_move(self, -self->__deadzone__, self->__deadzone__, self->num);
  dynamic_arr_resize(self, (self->num ? self->num : 1), true);

  return;
} /* dynamic_arr_trim */

void dynamic_arr_remove_at(dynamic_arr* self, unsigned long index, void* out) {
  check_index(self, "remove", index);

  if (out)
    memcpy(out, self->__malloc_start__ + index2off(self, index), self->element_size);

  dynamic_arr_move(self, -1, index, self->num - index);
  dynamic_arr_resize(self, -1, false);
  self->num--;

  return;
} /* dynamic_arr_remove_at */

void dynamic_arr_quick_remove_at(dynamic_arr* self, unsigned long index, void* out) {
  check_index(self, "remove", index);

  if (out)
    memcpy(out, self->__malloc_start__ + index2off(self, index), self->element_size);
  if (self->num >> 1 >= index) {
    dynamic_arr_move(self, 1, 0, index);
    self->__deadzone__++;
  } else {
    dynamic_arr_move(self, -1, index, self->num - index);
    dynamic_arr_resize(self, -1, false);
  }

  self->num--;

  return;
} /* dynamic_arr_quick_remove_at */

void dynamic_arr_bulk_remove_at(dynamic_arr* self, unsigned long index, void* out, unsigned long num) {
  check_index_len(self, "bulk-remove", index, num);

  if (out)
    memcpy(out, self->__malloc_start__ + index2off(self, index), num * self->element_size);

  dynamic_arr_move(self, -num, index + num, self->num - index - num);
  dynamic_arr_resize(self, -num, false);

  self->num -= num;

  return;
} /* dynamic_arr_bulk_remove_at */

void dynamic_arr_cleanup(dynamic_arr* self) {
  free(self->__malloc_start__);
  
  *self = (dynamic_arr) {0};

  return;
} /* dynamic_arr_cleanup */
/* ============= */

/* =====================
 * Convenience Functions
 * ===================== */
void dynamic_arr_resize(dynamic_arr* self, long change, const bool explicit_size) {
  if (!explicit_size && !change)
    return;

  unsigned long newcap = self->__cap__;
  if (explicit_size) {
    if (change <= 0) {
      fprintf(stderr, 
          "Attempt to explicitly resize dynamic array to size %ld!\n"
          bug_notice
          "=== ABORT ===\n",
          change);

      abort();
    }
    newcap = change;
  } else if (change < 0) {
    if ((unsigned long)-change > self->num) {
      fprintf(stderr, 
          "Attempt to resize dynamic array of size %lu to size %ld!\n"
          "=== ABORT ===\n",
          self->num, (long)self->num + change);

      abort();
    }

    const unsigned long back_deadzone = self->__cap__ - self->num;
    newcap -= (back_deadzone >> 1);
  } else if (self->num + change >= self->__cap__) {
    newcap += (self->__cap__ >> 1) + 1;
  } else {
    return;
  }

  if (self->__cap__ == newcap)
    return;

#if DEBUG_DYNAMIC_ARR_RESIZE
  printf("realloc: %lu -> %lu\n", self->cap, newcap);
#endif
  self->__malloc_start__ = realloc(self->__malloc_start__, newcap * self->element_size);
  if (!self->__malloc_start__) {
    fprintf(stderr, 
        "Failed to reallocate memory for dynamic array of old size %lu and new size %lu: %s\n"
        "=== ABORT ===\n",
        self->__cap__, newcap, strerror(errno));

    abort();
  }
  self->__cap__ = newcap;

  return;
}

void dynamic_arr_move(dynamic_arr* self, long change, unsigned long offset, unsigned long len) {
#if DEBUG_DYNAMIC_ARR_MOVE
  printf("dynamic_arr_move: %ld %s {%lu SKIP, %lu byte(s)}\n", change, (change > 0 ? "->" : "<-"), offset, len);
#endif

  if (!change || !len 
      || (change < 0 ? (unsigned long)(-change) > offset : (unsigned long)change > len))
    return;

  if (change < 0) {
    for (unsigned long i = offset + (-change); i < offset + len; i++)
      memcpy(
          self->__malloc_start__ + index2off(self, i + change), 
          self->__malloc_start__ + index2off(self, i), 
          self->element_size);

    return;
  }

  for (unsigned long i = offset + len - change; i > offset; i--)
    memcpy(
        self->__malloc_start__ + index2off(self, i + change), 
        self->__malloc_start__ + index2off(self, i), 
        self->element_size);

  memcpy(
      self->__malloc_start__ + index2off(self, offset + change), 
      self->__malloc_start__ + index2off(self, offset), 
      self->element_size);

  return;
}

void dynamic_arr_print(const dynamic_arr* self) {
  printf("%lu * %u byte(s): {%lu * %u byte(s), ", 
      self->num, self->element_size, self->__deadzone__, self->element_size);

  for (unsigned long i = 0; i < self->num; i++) {
    for (unsigned long j = 0; j < self->element_size; j++)
      printf("%x", *(self->__malloc_start__ + index2off(self, i) + j));
    printf(", ");
  }
  printf("%lu * %u byte(s)}\n", 
      self->__cap__ - self->num, self->element_size);

  return;
}
/* ===================== */
