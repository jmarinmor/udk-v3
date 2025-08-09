#define BUILD_DLL

#include <udk/core.h>


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

