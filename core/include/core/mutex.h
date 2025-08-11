#pragma once

#include <pre/compiler.h>
#include <pre/lang.h>
#include <core/atomic.h>


void	enter_to_critical_section();
void	exit_from_critical_section();

void	thread_yield();

FAST_FNC(void)	spinlock_init(volatile int*);
FAST_FNC(void)	spinlock_lock(volatile int*);
FAST_FNC(void)	spinlock_unlock(volatile int*);

FAST_FNC(void)	memory_lock(const void*);
FAST_FNC(void)	memory_unlock(const void*);
FAST_FNC(void)	memory_lock();
FAST_FNC(void)	memory_unlock();



class spinlock_t
{
	void exit_from_thread();
	volatile int _lock;
public:
	inline	spinlock_t():_lock(0){}
	inline	~spinlock_t(){}
	
	FORCE_INLINE	bool	try_lock()	{ return atomic_xchg_acq_rel_i32(&_lock, 1u) == 0u; }
	inline			void	lock()
	{
		for (;;)
		{
			if(try_lock())
				return;
			else
				thread_yield();
		}
	}
	FORCE_INLINE	void	unlock()	{ atomic_store_release_i32(&_lock, 0u); }
};

struct spinlock_guard_t 
{
	explicit inline spinlock_guard_t(spinlock_t &locker):_locker(locker) { _locker.lock(); }
	explicit inline spinlock_guard_t(const spinlock_t &locker):_locker(const_cast<spinlock_t&>(locker)) { _locker.lock(); }
	inline ~spinlock_guard_t() { _locker.unlock(); }

	private:
	spinlock_t& _locker;
};