#ifndef __RAV_CRITICAL_SECTION_HEADER__
#define __RAV_CRITICAL_SECTION_HEADER__

#include <pre/functions.h>

#ifdef __cplusplus
extern "C" {
#endif

	DLL_FNC(void)	enter_to_critical_section();
	DLL_FNC(void)	exit_from_critical_section();
	DLL_FNC(void)	atomic_yield();
	
	DLL_FNC(void)	spinlock_init(volatile int*);
	DLL_FNC(void)	spinlock_lock(volatile int*);
	DLL_FNC(void)	spinlock_unlock(volatile int*);

#ifdef __cplusplus
}
#endif

#endif