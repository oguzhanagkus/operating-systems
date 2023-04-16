
#ifndef __MYOS__THREAD_H
#define __MYOS__THREAD_H

#include <common/types.h>
#include <gdt.h>

namespace myos
{
    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;
    } __attribute__((packed));

    class Thread
    {
        friend class ThreadManager;

    private:
        CPUState *cpustate;
        common::uint32_t pc;        // Program counter
        common::uint8_t stack[512]; // Stack
        common::uint8_t state;      // State: 0 = ready, 1 = running, 2 = waiting, 3 = finished

    public:
        Thread(void function()); // Create a new thread like pthread_create
        ~Thread();

        void exit();  // Terminate the thread
        void yield(); // Yield the CPU to another thread
        void join();  // Wait for the thread to terminate
    };

    class ThreadManager
    {
    private:
        Thread *threads[8];
        int numThreads;
        int currentThread;

    public:
        ThreadManager();
        ~ThreadManager();

        bool AddThread(Thread *thread);
        void SwitchThreads();
    };

}

#endif