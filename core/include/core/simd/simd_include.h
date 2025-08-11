#pragma once

// SSE / SSE2
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 2))
  #include <xmmintrin.h>   // SSE
  #include <emmintrin.h>   // SSE2
#endif

// SSE4.1 (si se usa)
#if defined(__SSE4_1__) || defined(_M_X64)
  #include <smmintrin.h>   // SSE4.1
#endif

// AVX
#if defined(__AVX__) || defined(_M_AVX) || defined(__AVX2__) || defined(_M_AVX2)
  #include <immintrin.h>
#endif

// ===============================================
// ARM / ARM64
// ===============================================
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  #include <arm_neon.h>    // NEON
#endif
