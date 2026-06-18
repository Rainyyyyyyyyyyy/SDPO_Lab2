#include "SAT_DPLL/BBV.h"
#include "SAT_DPLL/NodeBoolTree.h"
#include "SAT_DPLL/boolequation.h"
#include "SAT_DPLL/boolinterval.h"
#include <chrono>

//#include <QVector>
//#include <QDebug>
#define WIN32_LEAN_AND_MEAN  // убирает много лишнего из windows.h
#define NOGDI                 // убирает GDI
#include "Allocator/Allocator.h"


/// Constructor
/// @param[in]  size - size of the fixed blocks
/// @param[in]  objects - maximum number of object. If 0, new blocks are
///		created off the heap as necessary.
/// @param[in]	memory - pointer to a block of static memory for allocator or NULL
///		to obtain memory from global heap. If not NULL, the objects argument
///		defines the size of the memory block (size x objects = memory size in bytes).
///	@param[in]	name - optional allocator name string.
/// Allocator(size_t size, UINT objects=0, CHAR* memory = NULL, const CHAR* name=NULL);

/*
int number_of_objects_max = 1024*1024;
char * mem;

template <typename T_obj> void benchmark (){
    int size_of_block = sizeof(T_obj);
    int number_of_objects_max = 1024*1024;
    QVector <char> memory_pointer (size_of_block * number_of_objects_max);


    Allocator allocatorHeapBlocks(number_of_objects_max);
    Allocator allocatorHeapPool(size_of_block, number_of_objects_max);
    Allocator allocatorStaticPool(size_of_block, number_of_objects_max, memory_pointer.data());

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    allocatorHeapBlocks.Allocate(1000);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double  elapsed =  std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    qDebug()<<"elapsed: "<<elapsed;
}


#include "SAT_DPLL/BBV.h"
int main(int argc, char *argv[]) {
    benchmark<BBV>();
}

*/
/*
 * const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    func();
    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
*/




//#include "Allocator.h"
#include <assert.h>
#include <new>
#include <iostream>

// @see https://github.com/endurodave/Allocator

// On VisualStudio, to disable the debug heap for faster performance when using
// the debugger use this option:
// Debugging > Environment _NO_DEBUG_HEAP=1

//class MyClass{
//    DECLARE_ALLOCATOR
//    // remaining class definition
//};
//IMPLEMENT_ALLOCATOR(MyClass, 0, 0)

// Heap blocks mode unlimited with 100 byte blocks
Allocator allocatorHeapBlocks(100);

// Heap pool mode with 20, 100 byte blocks
Allocator allocatorHeapPool(100, 20);

// Static pool mode with 20, 100 byte blocks
char staticMemoryPool[100 * 20];
Allocator allocatorStaticPool(100, 20, staticMemoryPool);

// Static pool mode with 20 MyClass sized blocks using template
///     =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
///     =   =   =   =   MyClass заменить на свой типа данных (класс)      =   =   =   =
///     =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =   =
/*
 * X
 * BBV
 * BoolEquation
 * BoolInterval
 * NodeBoolTree
 */
#define MyClass NodeBoolTree
AllocatorPool<MyClass, 20> allocatorStaticPool2;

// Benchmark allocators
static const int MAX_BLOCKS = 100000;
static const int MAX_BLOCK_SIZE = 4096;
void* memoryPtrs[MAX_BLOCKS];
void* memoryPtrs2[MAX_BLOCKS];
AllocatorPool<char[MAX_BLOCK_SIZE], MAX_BLOCKS*2> allocatorStaticPoolBenchmark;
Allocator allocatorHeapBlocksBenchmark(MAX_BLOCK_SIZE);

static void out_of_memory()
{
    throw std::bad_alloc();
    // new-handler function called by Allocator when pool is out of memory
    assert(0);
}

typedef void* (*AllocFunc)(int size);
typedef void (*DeallocFunc)(void* ptr);
void Benchmark(const char* name, AllocFunc allocFunc, DeallocFunc deallocFunc);
void* AllocHeap(int size);
void DeallocHeap(void* ptr);
void* AllocStaticPool(int size);
void DeallocStaticPool(void* ptr);
void* AllocHeapBlocks(int size);
void DeallocHeapBlocks(void* ptr);

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{
    std::set_new_handler(out_of_memory);

    // Allocate MyClass using fixed block allocator
    //MyClass* myClass = new MyClass();
    //delete myClass;

    // Allocate 100 bytes in fixed block allocator, then deallocate
    void* memory1 = allocatorHeapBlocks.Allocate(100);
    allocatorHeapBlocks.Deallocate(memory1);

    void* memory2 = allocatorHeapBlocks.Allocate(100);
    allocatorHeapBlocks.Deallocate(memory2);

    void* memory3 = allocatorHeapPool.Allocate(100);
    allocatorHeapPool.Deallocate(memory3);

    void* memory4 = allocatorStaticPool.Allocate(100);
    allocatorStaticPool.Deallocate(memory4);

    void* memory5 = allocatorStaticPool2.Allocate(sizeof(MyClass));
    allocatorStaticPool2.Deallocate(memory5);
    try{

        Benchmark("Heap (Run 1)", AllocHeap, DeallocHeap);
        Benchmark("Heap (Run 2)", AllocHeap, DeallocHeap);
        Benchmark("Heap (Run 3)", AllocHeap, DeallocHeap);
        Benchmark("Static Pool (Run 1)", AllocStaticPool, DeallocStaticPool);
        Benchmark("Static Pool (Run 2)", AllocStaticPool, DeallocStaticPool);
        Benchmark("Static Pool (Run 3)", AllocStaticPool, DeallocStaticPool);
        Benchmark("Heap Blocks (Run 1)", AllocHeapBlocks, DeallocHeapBlocks);
        Benchmark("Heap Blocks (Run 2)", AllocHeapBlocks, DeallocHeapBlocks);
        Benchmark("Heap Blocks (Run 3)", AllocHeapBlocks, DeallocHeapBlocks);
    }catch (std::bad_alloc){
        std::cout<<"Bad Alloc!";
    }
    return 0;
}

//------------------------------------------------------------------------------
// AllocHeap
//------------------------------------------------------------------------------
void* AllocHeap(int size)
{
    return new CHAR[size];
}

//------------------------------------------------------------------------------
// DeallocHeap
//------------------------------------------------------------------------------
void DeallocHeap(void* ptr)
{
    delete [] ptr;
}

//------------------------------------------------------------------------------
// AllocStaticPool
//------------------------------------------------------------------------------
void* AllocStaticPool(int size)
{
    return allocatorStaticPoolBenchmark.Allocate(size);
}

//------------------------------------------------------------------------------
// DeallocStaticPool
//------------------------------------------------------------------------------
void DeallocStaticPool(void* ptr)
{
    allocatorStaticPoolBenchmark.Deallocate(ptr);
}

//------------------------------------------------------------------------------
// AllocHeapBlocks
//------------------------------------------------------------------------------
void* AllocHeapBlocks(int size)
{
    return allocatorHeapBlocksBenchmark.Allocate(size);
}

//------------------------------------------------------------------------------
// DeallocHeapBlocks
//------------------------------------------------------------------------------
void DeallocHeapBlocks(void* ptr)
{
    allocatorHeapBlocksBenchmark.Deallocate(ptr);
}

//------------------------------------------------------------------------------
// Benchmark
//------------------------------------------------------------------------------
void Benchmark(const char* name, AllocFunc allocFunc, DeallocFunc deallocFunc)
{
#if WIN32
    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds, TotalElapsedMicroseconds= {0};
    LARGE_INTEGER Frequency;

    SetProcessPriorityBoost(GetCurrentProcess(), true);

    QueryPerformanceFrequency(&Frequency);

    // Allocate MAX_BLOCKS blocks MAX_BLOCK_SIZE / 2 sized blocks
    QueryPerformanceCounter(&StartingTime);
    for (int i=0; i<MAX_BLOCKS; i++)
        memoryPtrs[i] = allocFunc(MAX_BLOCK_SIZE / 2);
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    std::cout << name << " allocate time: " << ElapsedMicroseconds.QuadPart << std::endl;
    TotalElapsedMicroseconds.QuadPart += ElapsedMicroseconds.QuadPart;

    // Deallocate MAX_BLOCKS blocks (every other one)
    QueryPerformanceCounter(&StartingTime);
    for (int i=0; i<MAX_BLOCKS; i+=2)
        deallocFunc(memoryPtrs[i]);
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    std::cout << name << " deallocate time: " << ElapsedMicroseconds.QuadPart << std::endl;
    TotalElapsedMicroseconds.QuadPart += ElapsedMicroseconds.QuadPart;

    // Allocate MAX_BLOCKS blocks MAX_BLOCK_SIZE sized blocks
    QueryPerformanceCounter(&StartingTime);
    for (int i=0; i<MAX_BLOCKS; i++)
        memoryPtrs2[i] = allocFunc(MAX_BLOCK_SIZE);
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    std::cout << name << " allocate time: " << ElapsedMicroseconds.QuadPart << std::endl;
    TotalElapsedMicroseconds.QuadPart += ElapsedMicroseconds.QuadPart;

    // Deallocate MAX_BLOCKS blocks (every other one)
    QueryPerformanceCounter(&StartingTime);
    for (int i=1; i<MAX_BLOCKS; i+=2)
        deallocFunc(memoryPtrs[i]);
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    std::cout << name << " deallocate time: " << ElapsedMicroseconds.QuadPart << std::endl;
    TotalElapsedMicroseconds.QuadPart += ElapsedMicroseconds.QuadPart;

    // Deallocate MAX_BLOCKS blocks
    QueryPerformanceCounter(&StartingTime);
    for (int i=MAX_BLOCKS-1; i>=0; i--)
        deallocFunc(memoryPtrs2[i]);
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    std::cout << name << " deallocate time: " << ElapsedMicroseconds.QuadPart << std::endl;
    TotalElapsedMicroseconds.QuadPart += ElapsedMicroseconds.QuadPart;

    std::cout << name << " TOTAL TIME: " << TotalElapsedMicroseconds.QuadPart << std::endl;

    SetProcessPriorityBoost(GetCurrentProcess(), false);
#endif
}


