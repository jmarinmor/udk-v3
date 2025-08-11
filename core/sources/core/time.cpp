#include <core/time.h>

#if defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <intrin.h>
#elif defined(__APPLE__)
  #include <mach/mach_time.h>
#else
  #include <time.h>
#endif



// -----------------------------------------------------------
// get_time(): devuelve un tiempo estable en nanosegundos
// -----------------------------------------------------------
uint64_t get_time(void) {
#if defined(_WIN32)
    static uint64_t freq = 0;
    if (!freq) {
        LARGE_INTEGER li;
        QueryPerformanceFrequency(&li);
        freq = (uint64_t)li.QuadPart;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (uint64_t)((now.QuadPart * 1000000000ull) / freq);
#elif defined(__APPLE__)
    static mach_timebase_info_data_t tb = {0,0};
    if (tb.denom == 0) {
        mach_timebase_info(&tb);
    }
    uint64_t t = mach_absolute_time();
    return (t * tb.numer) / tb.denom;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + ts.tv_nsec;
#endif
}
