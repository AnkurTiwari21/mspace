#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUM_ALLOCATIONS 10000
#define ALLOCATION_SIZE 1024  // Size of each allocation in bytes

// Block structure for linked list
// 1. ADDRESS POINTER
// 2. SPACE (IN BYTES)
// 3. FREE_STATUS (0=FREE / 1=BUSY)
// 4. PREV_NODE_PTR
// 5. NEXT_NODE_PTR

typedef struct BLOCK {
    void *ADDRESS_POINTER;
    size_t SPACE;     // size of the block in bytes
    bool FREE_STATUS; // 0 = FREE, 1 = BUSY
    struct BLOCK *PREV_NODE_PTR;
    struct BLOCK *NEXT_NODE_PTR;
} BLOCK;

BLOCK *root = NULL;

// Deallocate memory space provided to function
int dealloc(void *addr) {
    free(addr);  // Using free instead of munmap
    return 0;    // Return success
}

// Allocate memory using malloc
void *alloc(size_t size) {
    void *addr = malloc(size);  // Using malloc instead of mmap
    if (addr == NULL) {
        printf("malloc allocation failed\n");
        return NULL;
    }
    return addr;
}

// Detect and return a block that fits the requested size
BLOCK *detectAndReturnBlock(size_t size) {
    BLOCK *head = root;
    if (head == NULL) {
        return NULL;
    }
    while (head != NULL) {
        if (head->FREE_STATUS == true && head->SPACE >= size) {
            return head;
        }
        head = head->NEXT_NODE_PTR;
    }
    return NULL;
}

// Create a new block and add it to the linked list
BLOCK *createBlockInLL(size_t size, void *addressPtr, bool status, BLOCK *prev, BLOCK *next) {
    BLOCK *newBlock = (BLOCK *)malloc(sizeof(BLOCK));
    if (newBlock == NULL) {
        printf("Error creating node in LL\n");
        return NULL;
    }

    newBlock->ADDRESS_POINTER = addressPtr;
    newBlock->SPACE = size;
    newBlock->FREE_STATUS = status;
    newBlock->PREV_NODE_PTR = prev;
    newBlock->NEXT_NODE_PTR = next;

    return newBlock;
}

// Remove a block from the linked list and deallocate the memory
int removeBlockFromLL(BLOCK *blockAddrInMem) {
    if (blockAddrInMem->PREV_NODE_PTR != NULL) {
        blockAddrInMem->PREV_NODE_PTR->NEXT_NODE_PTR = blockAddrInMem->NEXT_NODE_PTR;
    }
    if (blockAddrInMem->NEXT_NODE_PTR != NULL) {
        blockAddrInMem->NEXT_NODE_PTR->PREV_NODE_PTR = blockAddrInMem->PREV_NODE_PTR;
    }

    dealloc(blockAddrInMem->ADDRESS_POINTER);

    free(blockAddrInMem);  // Free the block structure itself
    return 0;
}

// Allocate memory and add it to the linked list
void *mspacec(size_t size) {
    BLOCK *blockAddr = detectAndReturnBlock(size);
    if (blockAddr != NULL) {
        return blockAddr->ADDRESS_POINTER;  // Return the memory addr to use
    }

    void *memSpacePtr = alloc(size);
    if (memSpacePtr == NULL) {
        return NULL;  // Allocation failed
    }

    BLOCK *head = root;
    if (head == NULL) {
        root = createBlockInLL(size, memSpacePtr, false, NULL, NULL);
    } else {
        while (head->NEXT_NODE_PTR != NULL) {
            head = head->NEXT_NODE_PTR;
        }
        BLOCK *newBlock = createBlockInLL(size, memSpacePtr, false, head, NULL);
        head->NEXT_NODE_PTR = newBlock;
    }

    return memSpacePtr;
}

// Deallocate memory and remove the corresponding block from the linked list
int mspacer(void *memSpacePtr) {
    BLOCK *head = root;
    bool found = false;
    while (head != NULL) {
        if (head->ADDRESS_POINTER == memSpacePtr) {
            found = true;
            break;
        }
        head = head->NEXT_NODE_PTR;
    }

    if (!found) {
        return 1;  // Pointer not found
    }

    int error = removeBlockFromLL(head);
    if (error == 1) {
        printf("Error removing block from LL\n");
        return 1;
    }

    return 0;
}

int main() {
    // Example usage
    clock_t start, end;
    double allocation_time, deallocation_time;

    // Measure allocation time
    start = clock();
    void *allocations[NUM_ALLOCATIONS];  // Array to hold pointers to allocated memory

    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        allocations[i] = mspacec(ALLOCATION_SIZE);
        if (allocations[i] == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }
    }
    end = clock();
    allocation_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Measure deallocation time
    start = clock();
    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        mspacer(allocations[i]);
    }
    end = clock();
    deallocation_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print the results
    printf("Time taken for %d allocations: %.6f seconds\n", NUM_ALLOCATIONS, allocation_time);
    printf("Time taken for %d deallocations: %.6f seconds\n", NUM_ALLOCATIONS, deallocation_time);

    return 0;
}
