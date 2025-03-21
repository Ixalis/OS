#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

void show_memory_status() {
    struct mallinfo info = mallinfo();

    printf("Memory Allocation Status:\n");
    printf("  Total allocated space (heap): %d bytes\n", info.uordblks);
    printf("  Total free space (heap): %d bytes\n", info.fordblks);
    printf("  Total memory arena (managed by malloc): %d bytes\n", info.arena);
    printf("  Total number of free chunks: %d\n", info.ordblks);
}

int main() {
    // Check the initial program break
    void *initial_break = sbrk(0);
    printf("Initial program break: %p\n", initial_break);

    // Display memory allocation status
    printf("Checking memory allocation status...\n");
    show_memory_status();

    // Check the program break after potential allocations
    void *current_break = sbrk(0);
    printf("Current program break: %p\n", current_break);

    // Compare breaks
    if (initial_break == current_break) {
        printf("Heap is unused. Allocated space: 0 bytes\n");
    } else {
        printf("Heap is used. Allocated space: %ld bytes\n", (char *)current_break - (char *)initial_break);
    }

    return 0;
}