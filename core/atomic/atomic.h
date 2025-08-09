#ifndef __RAV_CORE_ATOMIC_HEADER__
#define __RAV_CORE_ATOMIC_HEADER__

#include <pre/compiler.h>
#include <atomic/critical_section.h>

#if defined(_MSC_VER) || defined(__BORLANDC__)

	__forceinline	int atomic_xchg_int(volatile int *p, int val)
	{
		// TODO: ver como se hace esto bien
		int aux;
	#pragma omp critical
		aux = *p;
		*p = val;
	#pragma omp end critical
		return aux;
	}

	__forceinline	unsigned int atomic_xchg_uint(volatile unsigned int *p, unsigned int val)
	{
		// TODO: ver como se hace esto bien
		unsigned int aux;
#pragma omp critical
		aux = *p;
		*p = val;
#pragma omp end critical
		return aux;
	}

	__forceinline	int atomic_inc_int(volatile int *p)
	{
#pragma omp atomic
		return (*p)++;
	}
	__forceinline	int atomic_dec_int(volatile int *p)
	{
#pragma omp atomic
		return (*p)--;
	}

	__forceinline	unsigned long atomic_inc_ulong(volatile unsigned long *p)
	{
#pragma omp atomic
		return (*p)++;
	}
	__forceinline	unsigned long atomic_dec_ulong(volatile unsigned long *p)
	{
#pragma omp atomic
		return (*p)--;
	}

#ifdef __cplusplus
	__forceinline	int atomic_xchg(volatile int *p, int val)
	{
		return atomic_xchg_int(p, val);
	}

	__forceinline	unsigned int atomic_xchg(volatile unsigned int *p, unsigned int val)
	{
		return atomic_xchg_uint(p, val);
	}

	__forceinline	int atomic_inc(volatile int *p)
	{
		return atomic_inc_int(p);
	}
	__forceinline	int atomic_dec(volatile int *p)
	{
		return atomic_dec_int(p);
	}

	__forceinline	unsigned long atomic_inc(volatile unsigned long *p)
	{
		return atomic_inc_ulong(p);
	}
	__forceinline	unsigned long atomic_dec(volatile unsigned long *p)
	{
		return atomic_dec_ulong(p);
	}
#endif

#else

#ifdef __cplusplus
#define ATOMIC_INC_PER_TYPE(_Type, _Extension) \
	inline	_Type atomic_inc_##_Extension(volatile _Type *p)							__attribute__((always_inline)); \
	inline	_Type atomic_inc_##_Extension(volatile _Type *p)							{ return __sync_fetch_and_add(p, 1); } \
	inline	_Type atomic_dec_##_Extension(volatile _Type *p)							__attribute__((always_inline));\
	inline	_Type atomic_dec_##_Extension(volatile _Type *p)							{ return __sync_fetch_and_sub(p, 1); } \
	inline	_Type atomic_xchg_##_Extension(volatile _Type *p, _Type val)				__attribute__((always_inline)); \
	inline	_Type atomic_xchg_##_Extension(volatile _Type *p, _Type val)				{ return __sync_lock_test_and_set(p, val); } \
	inline	_Type atomic_cmpxchg_##_Extension(volatile _Type *p, _Type cmp, _Type val)	__attribute__((always_inline)); \
	inline	_Type atomic_cmpxchg_##_Extension(volatile _Type *p, _Type cmp, _Type val)	{ return __sync_val_compare_and_swap(p, cmp, val); } \
	inline	_Type atomic_add_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_add_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_add(p, val); } \
	inline	_Type atomic_sub_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_sub_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_sub(p, val); } \
	inline	_Type atomic_or_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_or_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_or(p, val); } \
	inline	_Type atomic_and_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_and_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_and(p, val); } \
	inline	_Type atomic_xor_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_xor_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_xor(p, val); } \
	inline	_Type atomic_nand_##_Extension(volatile _Type *p, _Type val)				__attribute__((always_inline)); \
	inline	_Type atomic_inc(volatile _Type *p)								__attribute__((always_inline)); \
	inline	_Type atomic_inc(volatile _Type *p)								{ return __sync_fetch_and_add(p, 1); } \
	inline	_Type atomic_dec(volatile _Type *p)								__attribute__((always_inline));\
	inline	_Type atomic_dec(volatile _Type *p)								{ return __sync_fetch_and_sub(p, 1); } \
	inline	_Type atomic_xchg(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_xchg(volatile _Type *p, _Type val)					{ return __sync_lock_test_and_set(p, val); } \
	inline	_Type atomic_cmpxchg(volatile _Type *p, _Type cmp, _Type val)	__attribute__((always_inline)); \
	inline	_Type atomic_cmpxchg(volatile _Type *p, _Type cmp, _Type val)	{ return __sync_val_compare_and_swap(p, cmp, val); } \
	inline	_Type atomic_add(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_add(volatile _Type *p, _Type val)					{ return __sync_fetch_and_add(p, val); } \
	inline	_Type atomic_sub(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_sub(volatile _Type *p, _Type val)					{ return __sync_fetch_and_sub(p, val); } \
	inline	_Type atomic_or(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_or(volatile _Type *p, _Type val)					{ return __sync_fetch_and_or(p, val); } \
	inline	_Type atomic_and(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_and(volatile _Type *p, _Type val)					{ return __sync_fetch_and_and(p, val); } \
	inline	_Type atomic_xor(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_xor(volatile _Type *p, _Type val)					{ return __sync_fetch_and_xor(p, val); } \
	inline	_Type atomic_nand(volatile _Type *p, _Type val)					__attribute__((always_inline));

#else
#define ATOMIC_INC_PER_TYPE(_Type, _Extension) \
	inline	_Type atomic_inc_##_Extension(volatile _Type *p)								__attribute__((always_inline)); \
	inline	_Type atomic_inc_##_Extension(volatile _Type *p)								{ return __sync_fetch_and_add(p, 1); } \
	inline	_Type atomic_dec_##_Extension(volatile _Type *p)								__attribute__((always_inline));\
	inline	_Type atomic_dec_##_Extension(volatile _Type *p)								{ return __sync_fetch_and_sub(p, 1); } \
	inline	_Type atomic_xchg_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_xchg_##_Extension(volatile _Type *p, _Type val)					{ return __sync_lock_test_and_set(p, val); } \
	inline	_Type atomic_cmpxchg_##_Extension(volatile _Type *p, _Type cmp, _Type val)	__attribute__((always_inline)); \
	inline	_Type atomic_cmpxchg_##_Extension(volatile _Type *p, _Type cmp, _Type val)	{ return __sync_val_compare_and_swap(p, cmp, val); } \
	inline	_Type atomic_add_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_add_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_add(p, val); } \
	inline	_Type atomic_sub_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_sub_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_sub(p, val); } \
	inline	_Type atomic_or_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_or_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_or(p, val); } \
	inline	_Type atomic_and_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_and_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_and(p, val); } \
	inline	_Type atomic_xor_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline)); \
	inline	_Type atomic_xor_##_Extension(volatile _Type *p, _Type val)					{ return __sync_fetch_and_xor(p, val); } \
//	inline	_Type atomic_nand_##_Extension(volatile _Type *p, _Type val)					__attribute__((always_inline));
#endif

	ATOMIC_INC_PER_TYPE(unsigned long, ulong)
	ATOMIC_INC_PER_TYPE(int, int)
	ATOMIC_INC_PER_TYPE(unsigned int, uint)

	//	inline	int atomic_nand (volatile int *p, int val)				{return __sync_fetch_and_nand(p,val);}
#endif


#ifdef __cplusplus
namespace rav
{
	class spinlock_t
	{
		void exit_from_thread();
		volatile int mMutex;
	public:
		inline	spinlock_t():mMutex(0){}
		inline	~spinlock_t(){}
		
		FORCE_INLINE	bool	try_lock()	{ return atomic_xchg(&mMutex, 1) == 0; }
		FORCE_INLINE	void	lock() 		{ spinlock_lock(&mMutex); }
		FORCE_INLINE	void	unlock()	{ mMutex=0; }
	};
	
	struct spinlock_guard_t 
	{
		explicit inline spinlock_guard_t(spinlock_t &aMutex):mMutex(aMutex) { mMutex.lock(); }
		explicit inline spinlock_guard_t(const spinlock_t &aMutex):mMutex(const_cast<spinlock_t&>(aMutex)) { mMutex.lock(); }
		inline ~spinlock_guard_t() { mMutex.unlock(); }

		private:
		spinlock_t& mMutex;
	};
}

#endif

#endif
