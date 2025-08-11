// simd_load.inl
#pragma once

#include <simd/simd_types.h>

namespace simd {

  // Fallback escalar
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> load(const T* p) {
    simd_pack_t<D,T> out;
    for (int i = 0; i < D; ++i) (&out.x)[i] = p[i];
    return out;
  }

  // float4
  template<>
  SIMD_FORCEINLINE simd_pack_t<4,float> load<4,float>(const float* p) {
    simd_pack_t<4,float> out;
  #if defined(__ARM_NEON) || defined(__ARM_NEON__)
    out.m = vld1q_f32(p);
  #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    out.m = _mm_loadu_ps(p);
  #else
    for (int i = 0; i < 4; ++i) (&out.x)[i] = p[i];
  #endif
    return out;
  }

  // float2
  template<>
  SIMD_FORCEINLINE simd_pack_t<2,float> load<2,float>(const float* p) {
    simd_pack_t<2,float> out;
  #if defined(__ARM_NEON) || defined(__ARM_NEON__)
    out.m = vld1_f32(p);
  #else
    for (int i = 0; i < 2; ++i) (&out.x)[i] = p[i];
  #endif
    return out;
  }

  // double2
  template<>
  SIMD_FORCEINLINE simd_pack_t<2,double> load<2,double>(const double* p) {
    simd_pack_t<2,double> out;
  #if defined(__aarch64__) || (defined(__ARM_NEON) && defined(__ARM_FEATURE_FP64))
    out.m = vld1q_f64(p);
  #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    out.m = _mm_loadu_pd(p);
  #else
    for (int i = 0; i < 2; ++i) (&out.x)[i] = p[i];
  #endif
    return out;
  }

  // double4 (AVX)
  template<>
  SIMD_FORCEINLINE simd_pack_t<4,double> load<4,double>(const double* p) {
    simd_pack_t<4,double> out;
  #if defined(__AVX__) || defined(_M_AVX)
    out.m = _mm256_loadu_pd(p);
  #else
    for (int i = 0; i < 4; ++i) (&out.x)[i] = p[i];
  #endif
    return out;
  }

  // i32/u32x4
  namespace load_detail {
    template<typename T>
    SIMD_FORCEINLINE simd_pack_t<4,T> i32x4(const T* p) {
      static_assert(sizeof(T) == 4, "load i32x4 expects 32-bit type");
      simd_pack_t<4,T> out;
    #if defined(__ARM_NEON) || defined(__ARM_NEON__)
      if constexpr (std::is_unsigned<T>::value) {
        out.m = vld1q_u32(reinterpret_cast<const uint32_t*>(p));
      } else {
        out.m = vld1q_s32(reinterpret_cast<const int32_t*>(p));
      }
    #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
      out.m = _mm_loadu_si128(reinterpret_cast<const __m128i*>(p));
    #else
      for (int i = 0; i < 4; ++i) (&out.x)[i] = p[i];
    #endif
      return out;
    }
  }

  template<> SIMD_FORCEINLINE simd_pack_t<4,int>      load<4,int>(const int* p)           { return load_detail::i32x4(p); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned> load<4,unsigned>(const unsigned* p) { return load_detail::i32x4(p); }

} // namespace simd




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
