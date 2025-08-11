#define BUILD_DLL

#include <core/atomic.h>


#if (OS_PLATFORM & OS_PLATFORM_WINDOWS)

	#include <windows.h>

static INIT_ONCE gOnce = INIT_ONCE_STATIC_INIT;
static CRITICAL_SECTION gCS;

static BOOL CALLBACK init_cs(PINIT_ONCE, PVOID, PVOID*) {
    InitializeCriticalSection(&gCS);
    return TRUE;
}

DLL_FNC(void) enter_to_critical_section() 
{
    InitOnceExecuteOnce(&gOnce, init_cs, NULL, NULL);
    EnterCriticalSection(&gCS);
}
DLL_FNC(void) exit_from_critical_section() 
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


	DLL_FNC(void)	enter_to_critical_section()
	{
        InitMutex();
        pthread_mutex_lock(&gMutex);
	}
	DLL_FNC(void)	exit_from_critical_section()
    {
        pthread_mutex_unlock(&gMutex);
    }

#endif // defined



#include <sched.h>

DLL_FNC(void)	thread_yield()
{
#if (OS_PLATFORM & OS_PLATFORM_WINDOWS)
    Sleep(0);
#else
    sched_yield();
#endif
}











DLL_FNC(void)	spinlock_init(volatile int* mutex)
{
	*mutex = 0;
}

DLL_FNC(void)	spinlock_unlock(volatile int* mutex)
{
	atomic_store_release_i32(&mutex, 0u);
}

static FORCE_INLINE	bool	try_lock_spin(volatile int* mutex)	
{
	 return atomic_xchg_acq_rel_i32(&mutex, 1u) == 0u;
}

DLL_FNC(void)	spinlock_lock(volatile int* mutex)
{
	for (;;)
	{
		if(try_lock_spin(mutex))
			return;
		else
			thread_yield();
	}
}









volatile	int	gObjectMutex[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

DLL_FNC(void)	memory_lock(const void* i)
{
	spinlock_lock(&gObjectMutex[((uintptr_t)i) & 0x1F]);
}

DLL_FNC(void)	memory_unlock(const void* i)
{
	spinlock_unlock(&gObjectMutex[((uintptr_t)i) & 0x1F]);
}

volatile int gInterfacesMutex = 0;
DLL_FNC(void)	memory_lock_global()
{
	spinlock_lock(&gInterfacesMutex);
}

DLL_FNC(void)	memory_unlock_global()
{
	spinlock_unlock(&gInterfacesMutex);
}









