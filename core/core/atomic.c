#define BUILD_DLL

#include <udk/core.h>

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














#if defined(__Qt__)

#include <QElapsedTimer>

static	qint64			gGlobalTimer = -1;
static	QElapsedTimer	gGlobalQtTimer;

unsigned long long get_time()
{
	if (gGlobalTimer < 0)
	{
		gGlobalQtTimer.start();
		gGlobalTimer = gGlobalQtTimer.nsecsElapsed();
	}
	return gGlobalQtTimer.nsecsElapsed() - gGlobalTimer;
}

unsigned long long	time_to_nanoseconds(unsigned long long iTime)
{
	return iTime;
}


unsigned long long get_ellapsed_nanoseconds(unsigned long long  T0, unsigned long long  T1)
{
	return T1 - T0;
}


#elif defined(__APPLE__)

#include <time.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

static	mach_timebase_info_data_t	gTimebaseInfo;
static double gClockToSeconds;
static double gClockToNanoseconds;

FORCE_INLINE static void init_time()
{
	if ( gTimebaseInfo.denom == 0 )
	{
		(void) mach_timebase_info(&gTimebaseInfo);
		gClockToSeconds = ((double)gTimebaseInfo.numer) / gTimebaseInfo.denom;
		gClockToNanoseconds = ((double)gTimebaseInfo.numer) / gTimebaseInfo.denom;
	}
}

/*
DLL_FUNCTION(uint64_t) cpu_clock_get_current_tick(void)
{
    unsigned int lo, hi;
    __asm__ __volatile__ 
	(
      "rdtsc" : "=a"(lo), "=d"(hi)
    );
    return ((uint64_t)hi << 32) | lo;
}
 */

DLL_FUNCTION(uint64_t)	get_clock_tick()
{
	return clock();
}

DLL_FUNCTION(uint64_t)	app_time_get_current()
{
	return mach_absolute_time();
}

DLL_FUNCTION(double)	app_time_to_nanoseconds(uint64_t t)
{
	init_time();
	return t * gClockToNanoseconds;
}

DLL_FUNCTION(double)	app_time_get_seconds()
{
	init_time();
	return mach_absolute_time() * gClockToSeconds;
}

DLL_FUNCTION(double)	app_time_get_nanoseconds()
{
	init_time();
	return mach_absolute_time() * gClockToNanoseconds;
}

#elif (OS_PLATFORM & OS_PLATFORM_WINDOWS)

    #include <windows.h>

    static	double	gFrequency;
    static	int		gNeedInitialization = 1;

    unsigned long long	get_clock_tick()
    {
        //return clock();
        return 0;
    }

    unsigned long long	get_time()
    {
    	unsigned long long t;
        if (gNeedInitialization)
		{
			LARGE_INTEGER tt;
			QueryPerformanceFrequency(&tt);
			gNeedInitialization = 0;
			gFrequency = 1000000.0 / (((double)tt.QuadPart)/1000.0);
		}
		QueryPerformanceCounter((LARGE_INTEGER*)&t);
        return t;
    }

    unsigned long long	get_ellapsed_nanoseconds(unsigned long long  i0,unsigned long long  i1)
    {
    	const LARGE_INTEGER* t0 = (const LARGE_INTEGER*)&i0;
    	const LARGE_INTEGER* t1 = (const LARGE_INTEGER*)&i1;
		return (long long)(( t1->QuadPart - t0->QuadPart ) * gFrequency);
    }
    unsigned long long	time_to_nanoseconds(unsigned long long iTime)
    {
		const LARGE_INTEGER* t = (const LARGE_INTEGER*)&iTime;
		return (long long)(t->QuadPart * gFrequency);
    }

#elif defined (__ANDROID__)

#include <time.h>

uint64_t	time_get_clock_tick()
{
	return clock();
}

uint64_t	time_get_current_time()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (uint64_t)(now.tv_sec)*1000000000LL + now.tv_nsec;
}

uint64_t	time_get_ellapsed_time_nanoseconds(uint64_t _TimeA,uint64_t _TimeB)
{
	return _TimeB-_TimeA;
}
uint64_t	time_convert_to_nanoseconds(uint64_t _Time)
{
	return _Time;
}

#else

    #include <time.h>

	unsigned long long	get_clock_tick()
	{
		return clock();
	}

	unsigned long long	get_current_time()
	{
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
#if defined(__GCC__)
        return ((unsigned long long)(now.tv_sec))*1000000000LL + now.tv_nsec;
#else
		return unsigned long long(now.tv_sec)*1000000000LL + now.tv_nsec;
#endif
	}

	unsigned long long	get_ellapsed_nanoseconds(unsigned long long  i0,unsigned long long  i1)
	{
		return  i1 - i0;
	}
	unsigned long long	time_convert_to_nanoseconds(unsigned long long iTime)
	{
		return iTime;
	}

#endif

