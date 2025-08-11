// simd_store.inl
#pragma once

#include <simd/simd_types.h>


namespace simd {

  // Fallback escalar
  template<int D, typename T>
  SIMD_FORCEINLINE void store(T* p, const simd_pack_t<D,T>& v) {
    for (int i = 0; i < D; ++i) p[i] = (&v.x)[i];
  }

  // float4
  template<>
  SIMD_FORCEINLINE void store<4,float>(float* p, const simd_pack_t<4,float>& v) {
  #if defined(__ARM_NEON) || defined(__ARM_NEON__)
    vst1q_f32(p, v.m);
  #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    _mm_storeu_ps(p, v.m);
  #else
    for (int i = 0; i < 4; ++i) p[i] = (&v.x)[i];
  #endif
  }

  // float2
  template<>
  SIMD_FORCEINLINE void store<2,float>(float* p, const simd_pack_t<2,float>& v) {
  #if defined(__ARM_NEON) || defined(__ARM_NEON__)
    vst1_f32(p, v.m);
  #else
    for (int i = 0; i < 2; ++i) p[i] = (&v.x)[i];
  #endif
  }

  // double2
  template<>
  SIMD_FORCEINLINE void store<2,double>(double* p, const simd_pack_t<2,double>& v) {
  #if defined(__aarch64__) || (defined(__ARM_NEON) && defined(__ARM_FEATURE_FP64))
    vst1q_f64(p, v.m);
  #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    _mm_storeu_pd(p, v.m);
  #else
    for (int i = 0; i < 2; ++i) p[i] = (&v.x)[i];
  #endif
  }

  // double4 (AVX)
  template<>
  SIMD_FORCEINLINE void store<4,double>(double* p, const simd_pack_t<4,double>& v) {
  #if defined(__AVX__) || defined(_M_AVX)
    _mm256_storeu_pd(p, v.m);
  #else
    for (int i = 0; i < 4; ++i) p[i] = (&v.x)[i];
  #endif
  }

  // i32/u32x4
  namespace store_detail {
    template<typename T>
    SIMD_FORCEINLINE void i32x4(T* p, const simd_pack_t<4,T>& v) {
      static_assert(sizeof(T) == 4, "store i32x4 expects 32-bit type");
    #if defined(__ARM_NEON) || defined(__ARM_NEON__)
      if constexpr (std::is_unsigned<T>::value) {
        vst1q_u32(reinterpret_cast<uint32_t*>(p), v.m);
      } else {
        vst1q_s32(reinterpret_cast<int32_t*>(p), v.m);
      }
    #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
      _mm_storeu_si128(reinterpret_cast<__m128i*>(p), v.m);
    #else
      for (int i = 0; i < 4; ++i) p[i] = (&v.x)[i];
    #endif
    }
  }

  template<> SIMD_FORCEINLINE void store<4,int>(int* p, const simd_pack_t<4,int>& v)                 { store_detail::i32x4(p, v); }
  template<> SIMD_FORCEINLINE void store<4,unsigned>(unsigned* p, const simd_pack_t<4,unsigned>& v)   { store_detail::i32x4(p, v); }

} // namespace simd
