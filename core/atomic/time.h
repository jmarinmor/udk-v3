#ifndef __RAV_CORE_ATOMIC_HEADER__
#define __RAV_CORE_ATOMIC_HEADER__

#include <pre/functions.h>

#ifdef __cplusplus
extern "C" {
#endif

DLL_FNC(unsigned long long)	get_clock_tick();
DLL_FNC(unsigned long long)	get_time();
DLL_FNC(unsigned long long)	get_ellapsed_nanoseconds(unsigned long long  i0,unsigned long long  i1);
DLL_FNC(unsigned long long)	time_to_nanoseconds(unsigned long long iTime);
	
#ifdef __cplusplus
}
#endif


#endif
