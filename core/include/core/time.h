#pragma once


#include <stdint.h>
#include <pre/lang.h>

uint64_t	get_time();

// -----------------------------------------------------------
// get_cpu_tick(): devuelve el contador de ciclos de CPU
// -----------------------------------------------------------
FORCE_INLINE uint64_t get_cpu_tick(void) {
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
    return __rdtsc(); // intrínseco de MSVC para RDTSC
#elif defined(__i386__) || defined(__x86_64__)
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#elif defined(__aarch64__)
    uint64_t val;
    asm volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
#elif defined(__arm__)
    uint32_t val;
    asm volatile("mrc p15, 0, %0, c15, c12, 0" : "=r"(val));
    return (uint64_t)val;
#else
    // Fallback: no hay contador de ciclos accesible
    return 0;
#endif
}