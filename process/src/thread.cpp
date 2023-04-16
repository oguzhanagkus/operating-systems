
#include <thread.h>

using namespace myos;
using namespace myos::common;

Thread::Thread(void (*entrypoint)())
{
    cpustate = (CPUState *)(stack + 4096 - sizeof(CPUState));

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    cpustate->eip = (uint32_t)entrypoint;

    cpustate->eflags = 0x202;
}

Thread::~Thread()
{
}

void Thread::exit()
{
}

void Thread::yield()
{
}

void Thread::join()
{
}

/******************************************************************************/

ThreadManager::ThreadManager()
{
    numThreads = 0;
    currentThread = -1;
}

ThreadManager::~ThreadManager()
{
}

bool ThreadManager::AddThread(Thread *thread)
{
    if (numThreads >= 8)
        return false;

    threads[numThreads++] = thread;

    return true;
}

void ThreadManager::SwitchThreads()
{
}
