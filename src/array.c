#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

#define ARRAY_RAW_DATA(array) ((int*)(array) - 2)
#define ARRAY_CAPACITY(array) (ARRAY_RAW_DATA(array)[0])
#define ARRAY_OCCUPIED(array) (ARRAY_RAW_DATA(array)[1])

// void* ft_realloc(void* ptr, size_t new_size) {
//     if (new_size == 0) {
//         free(ptr); // If new size is 0, just free the old memory and return NULL.
//         return NULL;
//     }

//     void* new_ptr = malloc(new_size); // Allocate new memory block.
    
//     if (new_ptr == NULL) {
//         // Allocation failed, return NULL without freeing the old block to avoid data loss.
//         return NULL;
//     }

//     if (ptr != NULL) {
//         // Copy data from the old block to the new block.
//         size_t copy_size = new_size;
//         if (new_size > sizeof(ptr)) {
//             copy_size = sizeof(ptr);
//         }
//         memcpy(new_ptr, ptr, copy_size);
        
//         free(ptr); // Free the old memory block.
//     }

//     return new_ptr;
// }

// void* array_hold(void* array, int count, int item_size) {
//     if (array == NULL) {
//         int raw_size = (sizeof(int) * 2) + (item_size * count);
//         int* base = (int*)malloc(raw_size);
//         base[0] = count;  // capacity
//         base[1] = count;  // occupied
//         return base + 2;
//     } else if (ARRAY_OCCUPIED(array) + count <= ARRAY_CAPACITY(array)) {
//         ARRAY_OCCUPIED(array) += count;
//         return array;
//     } else {
//         int needed_size = ARRAY_OCCUPIED(array) + count;
//         int float_curr = ARRAY_CAPACITY(array) * 2;
//         int capacity = needed_size > float_curr ? needed_size : float_curr;
//         int occupied = needed_size;
//         int raw_size = sizeof(int) * 2 + item_size * capacity;
//         // int* base = (int*)realloc(ARRAY_RAW_DATA(array), raw_size);
// 		int* base = (int*)ft_realloc(ARRAY_RAW_DATA(array), raw_size);
//         base[0] = capacity;
//         base[1] = occupied;
//         return base + 2;
//     }
// }

// int array_length(void* array) {
//     return (array != NULL) ? ARRAY_OCCUPIED(array) : 0;
// }

// void array_free(void* array) {
//     if (array != NULL) {
//         free(ARRAY_RAW_DATA(array));
//     }
// }