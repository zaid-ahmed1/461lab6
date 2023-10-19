#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include "myalloc.h"

// Global state for our custom memory allocator
static void *_arena_start = NULL;   // Start of the memory arena
static size_t _arena_size = 0;      // Size of the memory arena

void myinit(size_t size) {
    // Request a contiguous block of memory using mmap
    _arena_start = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (_arena_start == MAP_FAILED) {
        // mmap failed. Handle this error as needed for your application.
        _arena_start = NULL;
        _arena_size = 0;
        perror("mmap failed");
        return;
    }

    _arena_size = size;
}

void *myalloc(size_t size) {
    // This is a very rudimentary allocator. It will not free memory, and 
    // it will not handle running out of memory in the arena.
    // For demonstration purposes only.

    // If we don't have enough space in the arena, return NULL
    if (_arena_size < size) {
        return NULL;
    }

    void *allocated_chunk = _arena_start;

    _arena_start += size;            // Move the arena start pointer
    _arena_size -= size;             // Deduct the allocated size from available size

    return allocated_chunk;
}

void mydestroy() {
    if (_arena_start) {
        munmap(_arena_start, _arena_size);
        _arena_start = NULL;
        _arena_size = 0;
    }
}

// int main() {
//     myinit(4096);  // Request a memory arena of 4096 bytes

//     int *arr = (int *) myalloc(4 * sizeof(int));  // Allocate space for 4 integers

//     if (arr) {
//         for (int i = 0; i < 4; i++) {
//             arr[i] = i * i;  // Example use of the allocated memory
//         }

//         for (int i = 0; i < 4; i++) {
//             printf("%d ", arr[i]);
//         }
//         printf("\n");
//     }

//     mydestroy();  // Clean up

//     return 0;
// }

