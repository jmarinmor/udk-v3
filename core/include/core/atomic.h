#pragma once

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct integer atomic operations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* ============================================================================
 * Variantes atómicas compactas por macros (32/64 bit)
 *
 * Cuándo usar:
 *   - *_relaxed: contadores/telemetría; sólo atomicidad.
 *   - *_acquire: lecturas de flag/lock (consumidor).
 *   - *_release: escrituras de flag/lock (productor).
 *   - *_acq_rel: RMW que “toman/publican” estado (xchg/cmpxchg/fetch_add como claim).
 *
 * Ejemplos:
 *   // publicar dato + flag
 *   data = X;
 *   atomic_store_release_u32(&ready, 1u);
 *   if (atomic_load_acquire_u32(&ready)) { use(data); }
 *
 *   // lock con xchg
 *   while (atomic_xchg_acq_rel_u32(&lock, 1u)) { // spin
 *   }
 *   atomic_store_release_u32(&lock, 0u);
 * ========================================================================== */

#if defined(_MSC_VER)
/* ======================= MSVC (Interlocked) ======================= */
#include <intrin.h>
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedXor)
#if defined(_M_X64) || defined(_M_ARM64)
#pragma intrinsic(_InterlockedExchange64)
#pragma intrinsic(_InterlockedExchangeAdd64)
#pragma intrinsic(_InterlockedCompareExchange64)
#pragma intrinsic(_InterlockedOr64)
#pragma intrinsic(_InterlockedAnd64)
#pragma intrinsic(_InterlockedXor64)
#endif

/* 32-bit (long) */
#define GEN_ATOMIC_OPS_MSC32(SFX, T)                                                          \
    static __forceinline T atomic_load_acquire_##SFX(volatile T *p)                           \
    {                                                                                         \
        return (T)_InterlockedExchangeAdd((volatile long *)p, 0);                             \
    }                                                                                         \
    static __forceinline void atomic_store_release_##SFX(volatile T *p, T v)                  \
    {                                                                                         \
        (void)_InterlockedExchange((volatile long *)p, (long)v);                              \
    }                                                                                         \
    static __forceinline T atomic_xchg_relaxed_##SFX(volatile T *p, T v)                      \
    {                                                                                         \
        return (T)_InterlockedExchange((volatile long *)p, (long)v);                          \
    }                                                                                         \
    static __forceinline T atomic_xchg_acq_rel_##SFX(volatile T *p, T v)                      \
    {                                                                                         \
        return (T)_InterlockedExchange((volatile long *)p, (long)v);                          \
    }                                                                                         \
    static __forceinline T atomic_fetch_add_relaxed_##SFX(volatile T *p, T v)                 \
    {                                                                                         \
        return (T)_InterlockedExchangeAdd((volatile long *)p, (long)v);                       \
    }                                                                                         \
    static __forceinline T atomic_fetch_sub_relaxed_##SFX(volatile T *p, T v)                 \
    {                                                                                         \
        return (T)_InterlockedExchangeAdd((volatile long *)p, -(long)v);                      \
    }                                                                                         \
    static __forceinline T atomic_fetch_or_relaxed_##SFX(volatile T *p, T v)                  \
    {                                                                                         \
        return (T)_InterlockedOr((volatile long *)p, (long)v);                                \
    }                                                                                         \
    static __forceinline T atomic_fetch_and_relaxed_##SFX(volatile T *p, T v)                 \
    {                                                                                         \
        return (T)_InterlockedAnd((volatile long *)p, (long)v);                               \
    }                                                                                         \
    static __forceinline T atomic_fetch_xor_relaxed_##SFX(volatile T *p, T v)                 \
    {                                                                                         \
        return (T)_InterlockedXor((volatile long *)p, (long)v);                               \
    }                                                                                         \
    static __forceinline T atomic_cmpxchg_acq_rel_##SFX(volatile T *p, T expd, T desired)     \
    {                                                                                         \
        return (T)_InterlockedCompareExchange((volatile long *)p, (long)desired, (long)expd); \
    }

/* 64-bit (__int64) */
#if defined(_M_X64) || defined(_M_ARM64)
#define GEN_ATOMIC_OPS_MSC64(SFX, T)                                                                     \
    static __forceinline T atomic_load_acquire_##SFX(volatile T *p)                                      \
    {                                                                                                    \
        return (T)_InterlockedExchangeAdd64((volatile __int64 *)p, 0);                                   \
    }                                                                                                    \
    static __forceinline void atomic_store_release_##SFX(volatile T *p, T v)                             \
    {                                                                                                    \
        (void)_InterlockedExchange64((volatile __int64 *)p, (__int64)v);                                 \
    }                                                                                                    \
    static __forceinline T atomic_xchg_relaxed_##SFX(volatile T *p, T v)                                 \
    {                                                                                                    \
        return (T)_InterlockedExchange64((volatile __int64 *)p, (__int64)v);                             \
    }                                                                                                    \
    static __forceinline T atomic_xchg_acq_rel_##SFX(volatile T *p, T v)                                 \
    {                                                                                                    \
        return (T)_InterlockedExchange64((volatile __int64 *)p, (__int64)v);                             \
    }                                                                                                    \
    static __forceinline T atomic_fetch_add_relaxed_##SFX(volatile T *p, T v)                            \
    {                                                                                                    \
        return (T)_InterlockedExchangeAdd64((volatile __int64 *)p, (__int64)v);                          \
    }                                                                                                    \
    static __forceinline T atomic_fetch_sub_relaxed_##SFX(volatile T *p, T v)                            \
    {                                                                                                    \
        return (T)_InterlockedExchangeAdd64((volatile __int64 *)p, -(__int64)v);                         \
    }                                                                                                    \
    static __forceinline T atomic_fetch_or_relaxed_##SFX(volatile T *p, T v)                             \
    {                                                                                                    \
        return (T)_InterlockedOr64((volatile __int64 *)p, (__int64)v);                                   \
    }                                                                                                    \
    static __forceinline T atomic_fetch_and_relaxed_##SFX(volatile T *p, T v)                            \
    {                                                                                                    \
        return (T)_InterlockedAnd64((volatile __int64 *)p, (__int64)v);                                  \
    }                                                                                                    \
    static __forceinline T atomic_fetch_xor_relaxed_##SFX(volatile T *p, T v)                            \
    {                                                                                                    \
        return (T)_InterlockedXor64((volatile __int64 *)p, (__int64)v);                                  \
    }                                                                                                    \
    static __forceinline T atomic_cmpxchg_acq_rel_##SFX(volatile T *p, T expd, T desired)                \
    {                                                                                                    \
        return (T)_InterlockedCompareExchange64((volatile __int64 *)p, (__int64)desired, (__int64)expd); \
    }
#endif /* 64-bit */

/* Generaciones MSVC */
GEN_ATOMIC_OPS_MSC32(i32, int32_t)
GEN_ATOMIC_OPS_MSC32(u32, uint32_t)
#if defined(_M_X64) || defined(_M_ARM64)
GEN_ATOMIC_OPS_MSC64(i64, int64_t)
GEN_ATOMIC_OPS_MSC64(u64, uint64_t)
#endif

#else /* =================== GCC/Clang (__atomic) ==================== */

#define GCC_INLINE static inline __attribute__((always_inline))

#define GEN_ATOMIC_OPS_GCC(SFX, T)                                                             \
    GCC_INLINE T atomic_load_acquire_##SFX(volatile T *p)                                      \
    {                                                                                          \
        return __atomic_load_n(p, __ATOMIC_ACQUIRE);                                           \
    }                                                                                          \
    GCC_INLINE void atomic_store_release_##SFX(volatile T *p, T v)                             \
    {                                                                                          \
        __atomic_store_n(p, v, __ATOMIC_RELEASE);                                              \
    }                                                                                          \
    GCC_INLINE T atomic_xchg_relaxed_##SFX(volatile T *p, T v)                                 \
    {                                                                                          \
        return __atomic_exchange_n(p, v, __ATOMIC_RELAXED);                                    \
    }                                                                                          \
    GCC_INLINE T atomic_xchg_acq_rel_##SFX(volatile T *p, T v)                                 \
    {                                                                                          \
        return __atomic_exchange_n(p, v, __ATOMIC_ACQ_REL);                                    \
    }                                                                                          \
    GCC_INLINE T atomic_fetch_add_relaxed_##SFX(volatile T *p, T v)                            \
    {                                                                                          \
        return __atomic_fetch_add(p, v, __ATOMIC_RELAXED);                                     \
    }                                                                                          \
    GCC_INLINE T atomic_fetch_sub_relaxed_##SFX(volatile T *p, T v)                            \
    {                                                                                          \
        return __atomic_fetch_sub(p, v, __ATOMIC_RELAXED);                                     \
    }                                                                                          \
    GCC_INLINE T atomic_fetch_or_relaxed_##SFX(volatile T *p, T v)                             \
    {                                                                                          \
        return __atomic_fetch_or(p, v, __ATOMIC_RELAXED);                                      \
    }                                                                                          \
    GCC_INLINE T atomic_fetch_and_relaxed_##SFX(volatile T *p, T v)                            \
    {                                                                                          \
        return __atomic_fetch_and(p, v, __ATOMIC_RELAXED);                                     \
    }                                                                                          \
    GCC_INLINE T atomic_fetch_xor_relaxed_##SFX(volatile T *p, T v)                            \
    {                                                                                          \
        return __atomic_fetch_xor(p, v, __ATOMIC_RELAXED);                                     \
    }                                                                                          \
    GCC_INLINE T atomic_cmpxchg_acq_rel_##SFX(volatile T *p, T expd, T desired)                \
    {                                                                                          \
        T prev = expd;                                                                         \
        __atomic_compare_exchange_n(p, &prev, desired, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE); \
        return prev;                                                                           \
    }

/* Generaciones GCC/Clang */
GEN_ATOMIC_OPS_GCC(i32, int32_t)
GEN_ATOMIC_OPS_GCC(u32, uint32_t)
GEN_ATOMIC_OPS_GCC(i64, int64_t)
GEN_ATOMIC_OPS_GCC(u64, uint64_t)

#endif /* _MSC_VER */
