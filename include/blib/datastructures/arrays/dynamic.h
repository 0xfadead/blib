#ifndef __BLIB_DATASTRUCTURES_ARRAYS_DYNAMIC_H__
#define __BLIB_DATASTRUCTURES_ARRAYS_DYNAMIC_H__
#include <blib/defines/defines.h>

typedef struct {
  unsigned long cap; /* Maximum capacity/memory allocated in array */
  unsigned long num; /* Number of currently used elements */
  unsigned int size; /* Size of each element */

  unsigned long deadzone; /* Zone in front of the array (results from any `*_quick()` functions) */
  char* start; /* Start/ptr to the start of the array */
} dynamic_arr;

dynamic_arr __intern_dynamic_generic_arr_new(unsigned int element_size);

/**
 * @function dynamic_arr_copy
 * @brief Copy a specified dynamic array
 * @param src
 * [in] Dynamic array to be copied
 */
dynamic_arr dynamic_arr_copy(nonnullable const dynamic_arr* src);

/**
 * @function dynamic_arr_replace
 * @brief Replace element of array at specified index with specified element
 * @param self
 * [in] The generated dynamic array
 * @param index
 * [in] Index of element to be replaced
 * @param element
 * [in] Pointer to new element 
 */
void dynamic_arr_replace(nonnullable dynamic_arr* self, unsigned long index, nonnullable const void* element);

/**
 * @function dynamic_arr_flip
 * @brief Flip two elements of array at the specified indexes with each other
 * @param self
 * [in] The generated dynamic array
 * @param index1
 * [in] The index of the first element 
 * @param index2
 * [in] The index of the second element 
 */
void dynamic_arr_flip(nonnullable dynamic_arr* self, unsigned long index1, unsigned long index2);

/**
 * @function dynamic_arr_peek
 * @brief Read element in the array at the specified index
 * @param self
 * [in] The generated dynamic array
 * @param index
 * [in] The index of the element
 * @param out
 * [out] Pointer to write the element to 
 */
void dynamic_arr_peek(nonnullable const dynamic_arr* self, unsigned long index, nonnullable void* out);

/**
 * @function dynamic_arr_append
 * @brief Append an element to the start of an dynamic array
 * @param self
 * [in,out] The generated dynamic array
 * @param element
 * [in] The element to be appended 
 */
void dynamic_arr_append(nonnullable dynamic_arr* self, nonnullable const void* element);
/** 
 * @function dynamic_arr_prepend
 * @brief Append an element to the end of an dynamic array
 * @param self
 * [in,out] The generated dynamic array
 * @param element
 * [in] The element to be appended 
 */
void dynamic_arr_prepend(nonnullable dynamic_arr* self, nonnullable const void* element);
/**
 * @function dynamic_arr_insert_at
 * @brief Insert an element at the specified index
 * @param self
 * [in,out] The generated dynamic array
 * @param index
 * [in] The index of the new element
 * @param element
 * [in] The element to be inserted 
 */
void dynamic_arr_insert_at(nonnullable dynamic_arr* self, unsigned long index, nonnullable const void* element);

/**
 * @function dynamic_arr_precate
 * @brief Remove first element of an dynamic array
 * @param self
 * [in,out] The generated dynamic array 
 * @param out
 * [out,opt] Pointer to write the element to 
 */
void dynamic_arr_precate(nonnullable dynamic_arr* self, nullable void* out);
/**
 * @function dynamic_arr_quick_precate
 * @brief Faster version of dynamic_arr_precate(), but wastes memory
 * @param self
 * [in,out] The generated dynamic array
 * @param out
 * [out,opt] Pointer to write the element to 
 */
void dynamic_arr_quick_precate(nonnullable dynamic_arr* self, nullable void* out);
/**
 * @function dynamic_arr_truncate
 * @brief Pop last element of array
 * @param self
 * [in,out] The generated dynamic array
 * @param out
 * [out,opt] Pointer to write the element to 
 */
void dynamic_arr_truncate(nonnullable dynamic_arr* self, nullable void* out);
/**
 * @function dynamic_arr_trim
 * @brief Deallocate all unused space of array
 * @param self
 * [in,out] The generated dynamic array 
 */
void dynamic_arr_trim(nonnullable dynamic_arr* self);
/**
 * @function dynamic_arr_remove_at
 * @brief Remove element at the specified index
 * @param self
 * [in,out] The generated dynamic array
 * @param index
 * [in] The index for the element to be removed at
 * @param out
 * [out,opt] Pointer to write the element to 
 */
void dynamic_arr_remove_at(nonnullable dynamic_arr* self, unsigned long index, nullable void* out);
/**
 * @function dynamic_arr_remove_at_quick
 * @brief Faster version of `dynamic_arr_remove_at()`, but wastes memory
 * @param self
 * [in,out] The generated dynamic array
 * @param index
 * [in] The index for the element to be removed at
 * @param out
 * [out,opt] Pointer to write the element to 
 */
void dynamic_arr_quick_remove_at(nonnullable dynamic_arr* self, unsigned long index, nullable void* out);
/**
 * @function dynamic_arr_cleanup
 * @brief Free and cleanup the specified array
 * @param self
 * [in,out] The generated dynamic array 
 */
void dynamic_arr_cleanup(nonnullable dynamic_arr* self);

/**
 * @function dynamic_arr_new
 * @brief Create a new dynamic array
 * @param type Type
 * [in] of the array-elements 
 */
#define dynamic_arr_new(type) __intern_dynamic_generic_arr_new(sizeof(type))

#endif // !__BLIB_DATASTRUCTURES_ARRAYS_DYNAMIC_H__
