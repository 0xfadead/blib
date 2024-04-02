#ifndef __BLIB_DATASTRUCTURES_ARRAYS_DYNAMIC_H__
#define __BLIB_DATASTRUCTURES_ARRAYS_DYNAMIC_H__
#include <blib/defines/defines.h>
#include <stdint.h>

typedef struct {
  unsigned long __cap__; /* Maximum capacity/memory allocated in array */
  unsigned long num; /* Number of currently used elements */
  unsigned int element_size; /* Size of each element */

  unsigned long __deadzone__; /* Zone in front of the array (results from any `*_quick()` functions) */
  uint8_t* __malloc_start__; /* Start/ptr to the start of the array */
} dynamic_arr;

dynamic_arr __intern_dynamic_generic_arr_new(unsigned int element_size);

/**
 * @function dynamic_arr_copy
 * @brief Copy a specified dynamic array
 * @param src
 * [in,out] Dynamic array to be copied
 */
dynamic_arr dynamic_arr_copy(nonnullable const dynamic_arr* src);
/**
 * @function dynamic_arr_get_start
 * @brief Get the real start of a dynamic array
 * @param self
 * [in] The dynamic array
 */
void* dynamic_arr_get_start(nonnullable const dynamic_arr* self);

/**
 * @function dynamic_arr_replace
 * @brief Replace element of array at specified index with specified element
 * @param self
 * [in,out] The generated dynamic array
 * @param index
 * [in] Index of element to be replaced
 * @param element
 * [in] Pointer to new element 
 */
void dynamic_arr_replace(nonnullable dynamic_arr* self, unsigned long index, nonnullable const void* element);
/**
 * @function dynamic_arr_bulk_replace
 * @brief Replace sepcified number of elements of dynamic array at specified index
 * @param self
 * [in,out] The dynamic array
 * @param index
 * [in] Start index of replacement
 * @param elements
 * [in] New elements
 * @param num
 * [in] Number of replacement elements
 */
void dynamic_arr_bulk_replace(nonnullable dynamic_arr* self, unsigned long index, nonnullable const void* elements, unsigned long num);
/**
 * @function dynamic_arr_set
 * @brief Set specified number of elements starting from `index' to `element'
 * @param self
 * [in,out] The dynamic array
 * @param index
 * [in] Start index of setting
 * @param element
 * [in] New element
 * @param num
 * [in] Number of elements to set
 */
void dynamic_arr_set(nonnullable dynamic_arr* self, unsigned long index, nonnullable const void* element, unsigned long num);

/**
 * @function dynamic_arr_flip
 * @brief Flip two elements of array at the specified indexes with each other
 * @param self
 * [in,out] The generated dynamic array
 * @param index1
 * [in] The index of the first element 
 * @param index2
 * [in] The index of the second element 
 */
void dynamic_arr_flip(nonnullable dynamic_arr* self, unsigned long index1, unsigned long index2);
/**
 * @function dynamic_arr_bulk_flip
 * @brief Flip two arrays of elements, at the specified indexes, with each other
 * @param self
 * [in,out] The dynamic array
 * @param index1
 * [in] The index of the first array of elements
 * @param index2
 * [in] The index of the second array of elements
 * @param num
 * [in] The number of elements to flip
 */
void dynamic_arr_bulk_flip(nonnullable dynamic_arr* self, unsigned long index1, unsigned long index2, unsigned long num);

/**
 * @function dynamic_arr_peek
 * @brief Read element in the array at the specified index
 * @param self
 * [in,out] The generated dynamic array
 * @param index
 * [in] The index of the element
 * @param out
 * [out] Pointer to write the element to 
 */
void dynamic_arr_peek(nonnullable const dynamic_arr* self, unsigned long index, nonnullable void* out);
/**
 * @function dynamic_arr_bulk_peek
 * @brief Read specified amount of elements in dynamic array from specified index on
 * @param self
 * [in,out] The dynamic array
 * @param index
 * [in] Index to start the peeking at
 * @param out
 * [out] Buffer for elements
 */
void dynamic_arr_bulk_peek(nonnullable dynamic_arr* self, unsigned long index, nonnullable void* out, unsigned long num);

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
 * @function dynamic_arr_bulk_append
 * @brief Add a specified amount of elements to the end of a dynamic array
 * @param self
 * [in,out] The dynamic array
 * @param elements
 * [in] 
 */
void dynamic_arr_bulk_append(nonnullable dynamic_arr* self, nonnullable const void* elements, unsigned long num);
/** 
 * @function dynamic_arr_prepend
 * @brief Append an element to the end of a dynamic array
 * @param self
 * [in,out] The generated dynamic array
 * @param element
 * [in] The element to be appended 
 */
void dynamic_arr_prepend(nonnullable dynamic_arr* self, nonnullable const void* element);
/**
 * @function dynamic_arr_bulk_prepend
 * @brief Add a specified amount of elements to the start of a dynamic array
 * @param self
 * [in,out] The dynamic array
 * @param elements
 * [in] 
 */
void dynamic_arr_bulk_prepend(nonnullable dynamic_arr* self, nonnullable const void* elements, unsigned long num);
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
 * @function dynamic_arr_bulk_insert_at
 * @brief Insert a specified number of elements at the specified index
 * @param self
 * [in,out] The dynamic array
 * @param index
 * The index where to insert the new elements at
 * @param elements
 * Pointer to elements
 * @param num
 * Number of elements to insert
 */
void dynamic_arr_bulk_insert_at(nonnullable dynamic_arr* self, unsigned long index, nonnullable void* elements, unsigned long num);

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
 * @function dynamic_arr_resize_to
 * @brief Resize the dynamic array to specified size
 * @param self
 * [in,out] The dynamic array
 * @param out
 * [out,opt] Buffer for the elements
 * @param new_size
 * [in] New size of the array
 */
void dynamic_arr_resize_to(nonnullable dynamic_arr* self, nullable void* out, unsigned long new_size);
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
 * @function dynamic_arr_bulk_remove_at
 * @brief Remove specified amount of elements at index from dynamic array
 * @param self
 * [in,out] The dynamic array
 * @param index
 * [in] The index to start removing elements at
 * @param out
 * [out] Buffer for removed elements
 * @param num
 * [in] Number of elements to remove
 */
void dynamic_arr_bulk_remove_at(nonnullable dynamic_arr* self, unsigned long index, nullable void* out, unsigned long num);
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
