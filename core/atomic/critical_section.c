#include <stdint.h>

#define BUILD_DLL

#include <atomic/critical_section.h>


#if (OS_PLATFORM & OS_PLATFORM_WINDOWS)

	#include <windows.h>

	CRITICAL_SECTION gCS;  // shared structure

	DLL_FNC(void)	enter_to_critical_section()
	{
        EnterCriticalSection(&gCS);
	}
	DLL_FNC(void)	exit_from_critical_section()
	{
        LeaveCriticalSection(&gCS);
    }
#else

    #include <pthread.h>

    static	pthread_mutex_t gMutex;
    static	int				gMutexInitialized = 23563456;

    void	InitMutex()
    {
        if (gMutexInitialized != 0)
        {
            gMutexInitialized = 0;

            pthread_mutexattr_t Attr;

            pthread_mutexattr_init(&Attr);
            pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&gMutex, &Attr);
        }
    }


	void	enter_to_critical_section()
	{
        InitMutex();
        pthread_mutex_lock(&gMutex);
	}
	void	exit_from_critical_section()
    {
        pthread_mutex_unlock(&gMutex);
    }

#endif // defined

DLL_FNC(void)	atomic_yield()
{
#if (OS_PLATFORM & OS_PLATFORM_WINDOWS)
    Sleep(0);
#else
    sched_yield();
#endif
}


DLL_FNC(void)	spinlock_init(volatile int* i)
{
	*i = 0;
}

#if defined(USE_SPIN_ASM)
inline int try_lock(volatile int* i)
{
	int oldLock;
	#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
      asm volatile (
        "movl $1,%%eax\n\t"
        "xchg %%eax,%0\n\t"
        "movl %%eax,%1\n\t"
        : "=m" (*i), "=m" (oldLock)
        :
        : "%eax", "memory"
      );
	#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
      int *ptrLock = &mLock;
      __asm {
        mov eax,1
        mov ecx,ptrLock
        xchg eax,[ecx]
        mov oldLock,eax
      }
	#elif defined(__GNUC__) && (defined(__ppc__))
      int newLock = 1;
      asm volatile (
        "\n1:\n\t"
        "lwarx  %0,0,%1\n\t"
        "cmpwi  0,%0,0\n\t"
        "bne-   2f\n\t"
        "stwcx. %2,0,%1\n\t"
        "bne-   1b\n\t"
        "isync\n"
        "2:\n\t"
        : "=&r" (oldLock)
        : "r" (&mLock), "r" (newLock)
        : "cr0", "memory"
      );
	#endif
	return (oldLock == 0);
}

void	spinlock_lock(volatile int*)
{
	for (;;)
	{
		int	ret;
		if((ret=op_xchg<unsigned int>(&mMutex,1))==0)
		{
			// hurray, we have it
			return;
		}
		else
		{
			exit_from_thread();
		}
	}
}

void	spinlock_unlock(volatile int* i)
{
  #if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
      asm volatile (
        "movl $0,%%eax\n\t"
        "xchg %%eax,%0\n\t"
        : "=m" (mLock)
        :
        : "%eax", "memory"
      );
  #elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
      int *ptrLock = &mLock;
      __asm {
        mov eax,0
        mov ecx,ptrLock
        xchg eax,[ecx]
      }
  #elif defined(__GNUC__) && (defined(__ppc__))
      asm volatile (
        "sync\n\t"  // Replace with lwsync where possible?
        : : : "memory"
      );
      mLock = 0;
  #endif
}
#else

	DLL_FNC(void)	spinlock_unlock(volatile int* i)
	{
		*i = 0;
	}

	DLL_FNC(void)	spinlock_lock(volatile int* i)
	{
		for (;;)
		{
			int	ret;
			if((ret=atomic_xchg_int(i,1))==0)
			{
				// hurray, we have it
				return;
			}
			else
			{
				atomic_yield();
			}
		}
	}

#endif

volatile	int	gObjectMutex[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

DLL_FNC(void)	ccore_lock_object(const void* i)
{
	spinlock_lock(&gObjectMutex[((uintptr_t)i) & 0x1F]);
}

DLL_FNC(void)	ccore_unlock_object(const void* i)
{
	spinlock_unlock(&gObjectMutex[((uintptr_t)i) & 0x1F]);
}

volatile int gInterfacesMutex = 0;
DLL_FNC(void)	ccore_lock_objects()
{
	spinlock_lock(&gInterfacesMutex);
}

DLL_FNC(void)	ccore_unlock_objects()
{
	spinlock_unlock(&gInterfacesMutex);
}


