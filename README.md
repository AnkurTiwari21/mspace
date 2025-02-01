## 🧑‍💻⚙️ mspace - Memory Space 
This repository provides a learnerning attempt for custom implementation of a memory allocator in C , **simulating the behavior of malloc and free using mmap() and munmap() syscalls** . The goal is to track memory allocation and deallocation **using a linked list data structure** and to benchmark its performance by comparing the time taken for memory allocation and deallocation.

## 🌟 Features
 - Custom Memory Allocation: Simulates the malloc function by allocating memory using mmap() syscall and managing it using a custom linked list.
 - Block Metadata Management: Uses a linked list (BLOCK structure) to manage allocated memory blocks. Each block contains metadata such as:
    - Memory address
    - Size of the allocated memory
    - Free status (busy or free)
    - Pointers to the previous and next blocks in the list.
 - Linked List Structure: Dynamically maintains a linked list of allocated memory blocks, where each block holds information about its status and size.
 - Memory Deallocation: Implements a custom free operation, which removes the memory block from the linked list and deallocates the memory using free.
 - Efficient Block Reuse: When a block is freed, it can potentially be reused if it's large enough to fit future allocation requests.

## 📝 Program Logic
  1. **Memory Allocation (mspacec)**:

   - Memory is allocated using mmap() syscall.
   - A linked list node is created to store metadata about the allocated block, including its size and memory address.
   - If a free block of sufficient size is found in the list, it is reused instead of allocating new memory.
     
  2. **Memory Deallocation (mspacer)**:
   - Memory is allocated using munmap() syscall.
   - The block is found in the linked list using its memory address.
   - Once found, the block is removed from the linked list, and the memory is deallocated using free.
     
## ⏲️ Benchmarking:

   - **The program benchmarks the time taken for 10,000 memory allocations of 1024 bytes and deallocations**.
   - The times are printed to the console for comparison.
     
   ## Time Benchmark Results 📊
   
   1. **Custom Memory Allocator**
  - For the custom memory allocator, the following benchmark results were recorded after running 10,000 allocations and deallocations:
  - <img width="411" alt="Screenshot 2025-02-02 at 4 03 23 AM" src="https://github.com/user-attachments/assets/a067b65b-730f-4661-b991-73483042ca0e" />

    
   2. **Standard malloc and free**
  - For comparison, here are the benchmark results for 10,000 allocations and deallocations using malloc and free:
  - <img width="416" alt="Screenshot 2025-02-02 at 3 56 34 AM" src="https://github.com/user-attachments/assets/5b5a9039-c4ea-4373-b05d-2a25d305dfe7" />
    
## Observations 
 - Custom Allocator: The custom memory allocator takes significantly more time to perform the same number of allocations and deallocations compared to malloc and free. This is due to the overhead of managing a linked list and tracking memory blocks manually.
 - Standard malloc: malloc and free are much faster for both allocations and deallocations. The low deallocation time is especially notable due to the optimizations in the underlying malloc implementation.
