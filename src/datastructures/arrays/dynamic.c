#include <blib/datastructures/arrays/dynamic.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/* ==================
 * Convenience Macros
 * ================== */
#define check_index(self, operation, i) \
  if (i >= self->num) {      \
    fprintf(stderr,          \
        "Attempt to " operation " element %lu from dynamic array of element count %lu!\n" \
        "=== ABORT ===\n",   \
        i, self->num);       \
    abort();                 \
  }

#define check_rm_len(self, operation, number) \
  if (number > self->num) {           \
    fputs(                            \
        "Attempt to " operation " a dynamic array of element count 0!\n" \
        "=== ABORT ===\n", stderr);   \
    abort();                          \
  }

#define check_len(self, operation, n) \
  if (n > self->num) {                \
    fprintf(stderr,                   \
      "Attempt to " operation " %lu element(s) from dynamic array of element count %lu!\n" \
      "=== ABORT ===\n",              \
      n, self->num);                  \
    abort();                          \
  }

#define check_index_len(self, operation, i, n) \
  if (i + n > self->num) { \
    fprintf(stderr,        \
        "Attempt to " operation " %lu element(s) at index %lu from dynamic array of element count %lu!\n" \
        "=== ABORT ===\n", \
        n, i, self->num);  \
    abort();               \
  }

#define index2off(self, i) ((i + self->deadzone) * self->size)

#ifndef bug_notice
#define bug_notice                                                   \
          "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" \
          "!! NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE !!\n" \
          "!!       This may be a problem inside of blib.      !!\n" \
          "!!    Please submit a bug report at its git repo!   !!\n" \
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
/* ================================== */

/* =============
 * API Functions
 * ============= */
dynamic_arr __intern_dynamic_generic_arr_new(unsigned int element_size) {
  dynamic_arr arr = {0};

  if (sizeof(char) != 1) {
    fputs(
        "sizeof(char) is not 1 byte: All pointer arithmetic in blib relies on chars being one byte\n"
        "=== ABORT ===\n", 
        stderr);

    abort();
  }

  arr.size = element_size;
  arr.cap = 1;

  arr.start = malloc(arr.size);
  if (!arr.size) {
    fprintf(stderr, 
        "Failed to allocate initial memory for dynamic array of size %u: %s\n"
        "=== ABORT ===\n",
        arr.size, strerror(errno));

    abort();
  }

  return arr;
}

dynamic_arr dynamic_arr_copy(const dynamic_arr *src) {
  dynamic_arr dst = {
    .cap = src->num,
    .num = src->num,
    .size = src->size,
    .deadzone = 0
  };

  dst.start = malloc(src->num * src->size);
  if (!dst.start) {
    fprintf(stderr,
        "Failed to allocate initial memory for dynamic array copy of array size %lu: %s\n"
        "=== ABORT ===\n",
        dst.num * dst.size, strerror(errno));

    abort();
  }

  memcpy(dst.start, src->start + (src->deadzone * src->size), src->num * src->size);

  return dst;
}

void dynamic_arr_replace(dynamic_arr* self, unsigned long index, const void* element) {
  check_index(self, "replace", index);
  memcpy(self->start + index2off(self, index), element, self->size);

  return;
} /* dynamic_arr_replace */

void dynamic_arr_bulk_replace(dynamic_arr* self, unsigned long index, const void* elements, unsigned long num) {
  check_index_len(self, "bulk-replace", index, num);
  memcpy(self->start + index2off(self, index), elements, num * self->size);

  return;
} /* dynamic_arr_bulk_replace */

void dynamic_arr_flip(dynamic_arr* self, unsigned long index1, unsigned long index2) {
  char* buffer = NULL;
  if (self->num < self->cap) {
    buffer = self->start + index2off(self, self->num);
  } else if (self->deadzone) {
    buffer = self->start;
  } else {
    dynamic_arr_resize(self, 1, false);
    buffer = self->start + (self->num * self->size);
  }

  dynamic_arr_peek(self, index1, buffer);
  dynamic_arr_replace(self, index1, self->start + (index2 * self->size));
  dynamic_arr_replace(self, index2, buffer);

  return;
} /* dynamic_arr_flip */

void dynamic_arr_bulk_flip(dynamic_arr* self, unsigned long index1, unsigned long index2, unsigned long num) {
  check_index_len(self, "bulk-flip (1)", index1, num);
  check_index_len(self, "bulk-flip (2)", index2, num);
  if (!num)
    return;

  const unsigned long size = num * self->size;

  char* ptr = self->start + index2off(self, self->num);
  unsigned long elems_size = (self->cap - self->num) % (size + 1);
  if (elems_size)
    goto memory_cpy;

  ptr = self->start;
  elems_size = self->deadzone % (size + 1);
  if (elems_size)
    goto memory_cpy;

  dynamic_arr_resize(self, size, false);
  ptr = self->start + index2off(self, self->num);
  elems_size = size;

memory_cpy:
  while (num) {
    memcpy(ptr, self->start + index2off(self, index1), size);
    memcpy(self->start + index2off(self, index1), self->start + index2off(self, index2), size);
    memcpy(self->start + index2off(self, index2), ptr, size);

    index1 += num;
    index2 += num;

    num -= elems_size % (size + 1);
  }

  return;
} /* dynamic_arr_bulk_flip */

void dynamic_arr_peek(const dynamic_arr* self, unsigned long index, void* out) {
  check_index(self, "read", index);

  memcpy(out, self->start + index2off(self, index), self->size);
  return;
} /* dynamic_arr_peek */

void dynamic_arr_bulk_peek(dynamic_arr* self, unsigned long index, void* out, unsigned long num) {
  check_index_len(self, "bulk-peek", index, num);
  
  memcpy(out, self->start + index2off(self, index), num * self->size);
} /* dynamic_arr_bulk_peek */

void dynamic_arr_append(dynamic_arr* self, const void* element) {
  dynamic_arr_resize(self, 1, false);

  memcpy(self->start + index2off(self, self->num), element, self->size);
  self->num++;
  return;
} /* dynamic_arr_append */

void dynamic_arr_bulk_append(dynamic_arr* self, const void* elements, unsigned long num) {
  dynamic_arr_resize(self, num, false);

  memcpy(self->start + index2off(self, self->num), elements, num * self->size);
  self->num += num;

  return;
} /* dynamic_arr_bulk_append */

void dynamic_arr_prepend(dynamic_arr* self, const void* element) {
  if (self->deadzone) {
    self->deadzone--;
    memcpy(self->start + index2off(self, 0), element, self->size);
  } else {
    dynamic_arr_resize(self, 1, false);
    dynamic_arr_move(self, 1, 0, self->num);
    memcpy(self->start + index2off(self, 0), element, self->size);
  }

  return;
} /* dynamic_arr_prepend */

void dynamic_arr_bulk_prepend(dynamic_arr* self, const void* elements, unsigned long num) {
  unsigned long deadzone_elems = num % (self->deadzone + 1);
  unsigned long move_elems = num - deadzone_elems;

  if (move_elems) {
    dynamic_arr_resize(self, move_elems, false);
    dynamic_arr_move(self, move_elems, 0, self->num);
    memcpy(self->start + index2off(self, 0), (char*)elements + ((num - move_elems) * self->size), move_elems);
  }

  if (deadzone_elems) {
    self->deadzone -= deadzone_elems;
    memcpy(self->start + index2off(self, 0), elements, deadzone_elems * self->size);
  }

  return;
} /* dynamic_arr_bulk_prepend */

void dynamic_arr_insert_at(dynamic_arr* self, unsigned long index, const void* element) {
  check_index(self, "insert", index);

  if (self->deadzone && self->num >> 1 >= index) {
    self->deadzone--;
    index++;
    dynamic_arr_move(self, -1, 0, index + 1);

    memcpy(self->start + index2off(self, index), element, self->size);
  } else {
    dynamic_arr_resize(self, 1, false);
    dynamic_arr_move(self, 1, index, self->num - index);

    memcpy(self->start + index2off(self, index), element, self->size);
  }

  return;
} /* dynamic_arr_insert_at */

void dynamic_arr_bulk_insert_at(dynamic_arr* self, unsigned long index, void* elements, unsigned long num) {
  check_index_len(self, "bulk-insert", index, num);
  
  const unsigned char preferred_side   = self->num >> 1 < index;
  const unsigned long deadzone_inserts = preferred_side * (num % (self->deadzone + 1));
  const unsigned long end_inserts      = num - deadzone_inserts;

  dynamic_arr_move(self, -deadzone_inserts, 0, index + 1);
  dynamic_arr_resize(self, end_inserts, false);
  dynamic_arr_move(self, end_inserts, index + 1, self->num - index);

  memcpy(self->start + index2off(self, index - deadzone_inserts), elements, num * self->size);

  return;
} /* dynamic_arr_bulk_insert_at */

void dynamic_arr_precate(dynamic_arr* self, void* out) {
  check_rm_len(self, "precate", 1);

  if (out)
    memcpy(out, self->start + index2off(self, 0), self->size);
  dynamic_arr_move(self, -1, 0, self->num);
  dynamic_arr_resize(self, -1, false);
  self->num--;

  return;
} /* dynamic_arr_precate */

void dynamic_arr_quick_precate(dynamic_arr *self, void *out) {
  if (out)
    memcpy(out, self->start + index2off(self, 0), self->size);

  self->deadzone++;

  return;
} /* dynamic_arr_quick_precate */

void dynamic_arr_truncate(dynamic_arr *self, void *out) {
  check_rm_len(self, "truncate", 1);

  self->num--;
  if (out)
    memcpy(out, self->start + index2off(self, self->num), self->size);
  dynamic_arr_resize(self, -1, false);
  self->num--;

  return;
} /* dynamic_arr_truncate */

void dynamic_arr_resize_to(dynamic_arr* self, void* out, unsigned long new_size) {
  if (out && new_size < self->num)
    memcpy(out, self->start + index2off(self, new_size), (self->num - new_size) * self->size);

  dynamic_arr_resize(self, (new_size ? new_size : 1), true);

  return;
} /* dynamic_arr_resize_to */

void dynamic_arr_trim(dynamic_arr *self) {
  dynamic_arr_move(self, -self->deadzone, self->deadzone, self->num);
  dynamic_arr_resize(self, (self->num ? self->num : 1), true);

  return;
} /* dynamic_arr_trim */

void dynamic_arr_remove_at(dynamic_arr* self, unsigned long index, void* out) {
  check_index(self, "remove", index);

  if (out)
    memcpy(out, self->start + index2off(self, index), self->size);

  dynamic_arr_move(self, -1, index + 1, self->num - index - 1);
  dynamic_arr_resize(self, -1, false);
  self->num--;

  return;
} /* dynamic_arr_remove_at */

void dynamic_arr_quick_remove_at(dynamic_arr* self, unsigned long index, void* out) {
  check_index(self, "remove", index);

  if (out)
    memcpy(out, self->start + index2off(self, index), self->size);
  if (self->num >> 1 >= index) {
    dynamic_arr_move(self, 1, index, index + 1);
    self->deadzone--;
  } else {
    dynamic_arr_move(self, -1, index + 1, self->num - index - 1);
    dynamic_arr_resize(self, -1, false);
  }
} /* dynamic_arr_quick_remove_at */

void dynamic_arr_bulk_remove_at(dynamic_arr* self, unsigned long index, void* out, unsigned long num) {
  check_index_len(self, "bulk-remove", index, num);

  if (out)
    memcpy(out, self->start + index2off(self, index), num * self->size);

  dynamic_arr_move(self, -num, index + num, self->num - index - num);
  dynamic_arr_resize(self, -num, false);

  return;
} /* dynamic_arr_bulk_remove_at */

void dynamic_arr_cleanup(dynamic_arr* self) {
  free(self->start);
  
  *self = (dynamic_arr) {0};

  return;
} /* dynamic_arr_cleanup */
/* ============= */

/* =====================
 * Convenience Functions
 * ===================== */
void dynamic_arr_resize(dynamic_arr* self, long change, bool explicit_size) {
  if (!explicit_size && !change)
    return;

  unsigned long newcap = self->cap;
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

    const unsigned long back_deadzone = self->cap - self->num;
    newcap = newcap - (back_deadzone >> 1);
  } else if (self->num + change >= self->cap + self->deadzone) {
    newcap += (self->cap >> 1) + 1;
  } else {
    return;
  }

  if (self->cap == newcap)
    return;

  self->start = realloc(self->start, newcap * self->size);
  if (!self->start) {
    fprintf(stderr, 
        "Failed to reallocate memory for dynamic array of old size %lu and new size %lu: %s\n"
        "=== ABORT ===\n",
        self->cap, newcap, strerror(errno));

    abort();
  }
  self->cap = newcap;

  return;
}

void dynamic_arr_move(dynamic_arr* self, long change, unsigned long offset, unsigned long len) {
  if (!change || !len || (unsigned int)abs((int)change) >= len)
    return;

  if (change < 0) {
    for (unsigned long i = offset - change; i < len; i++)
      memcpy(
          self->start + index2off(self, i + change), 
          self->start + index2off(self, i), 
          self->size);

    return;
  }

  for (unsigned long i = len + offset - change; i >= offset + 1; i++)
    memcpy(
        self->start + index2off(self, i + change + offset), 
        self->start + index2off(self, i + offset), 
        self->size);

  return;
}
/* ===================== */
