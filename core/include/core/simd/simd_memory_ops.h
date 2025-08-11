// simd_memory_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

  // ---------------- Fallback por-lane ----------------
  template<int D>
  SIMD_FORCEINLINE simd_pack_t<D,float> gather(const float* base, const int* idx){
    simd_pack_t<D,float> r(0.f);
    for (int i=0;i<D;++i) (&r.x)[i] = base[idx[i]];
    return r;
  }
  template<int D>
  SIMD_FORCEINLINE simd_pack_t<D,double> gather(const double* base, const int* idx){
    simd_pack_t<D,double> r(0.0);
    for (int i=0;i<D;++i) (&r.x)[i] = base[idx[i]];
    return r;
  }

  template<int D>
  SIMD_FORCEINLINE void scatter(float* base, const int* idx, const simd_pack_t<D,float>& v){
    for (int i=0;i<D;++i) base[idx[i]] = (&v.x)[i];
  }
  template<int D>
  SIMD_FORCEINLINE void scatter(double* base, const int* idx, const simd_pack_t<D,double>& v){
    for (int i=0;i<D;++i) base[idx[i]] = (&v.x)[i];
  }

  template<int D>
  SIMD_FORCEINLINE simd_pack_t<D,float> masked_load(const float* p, const simd_pack_t<D,float>& m01, float fallback = 0.f){
    simd_pack_t<D,float> r(fallback);
    for (int i=0;i<D;++i) if ((&m01.x)[i] != 0.0f) (&r.x)[i] = p[i];
    return r;
  }
  template<int D>
  SIMD_FORCEINLINE simd_pack_t<D,double> masked_load(const double* p, const simd_pack_t<D,double>& m01, double fallback = 0.0){
    simd_pack_t<D,double> r(fallback);
    for (int i=0;i<D;++i) if ((&m01.x)[i] != 0.0) (&r.x)[i] = p[i];
    return r;
  }

  template<int D>
  SIMD_FORCEINLINE void masked_store(float* p, const simd_pack_t<D,float>& m01, const simd_pack_t<D,float>& v){
    for (int i=0;i<D;++i) if ((&m01.x)[i] != 0.0f) p[i] = (&v.x)[i];
  }
  template<int D>
  SIMD_FORCEINLINE void masked_store(double* p, const simd_pack_t<D,double>& m01, const simd_pack_t<D,double>& v){
    for (int i=0;i<D;++i) if ((&m01.x)[i] != 0.0) p[i] = (&v.x)[i];
  }

  template<int D>
  SIMD_FORCEINLINE void stream_store(float* p, const simd_pack_t<D,float>& v){
    // Fallback: almacena normal
    for (int i=0;i<D;++i) p[i] = (&v.x)[i];
  }
  template<int D>
  SIMD_FORCEINLINE void stream_store(double* p, const simd_pack_t<D,double>& v){
    for (int i=0;i<D;++i) p[i] = (&v.x)[i];
  }

} // namespace simd

// ==================== ESPECIALIZACIONES SIMD ====================
// AVX2 tiene gather; scatter no (hasta AVX-512)
#if defined(__AVX2__) || defined(_M_AVX2)
template<> SIMD_FORCEINLINE simd_pack_t<8,float>  simd::gather<8>(const float* base,  const int* idx){
  return simd_pack_t<8,float>(_mm256_i32gather_ps(base, _mm256_loadu_si256((const __m256i*)idx), 4));
}
template<> SIMD_FORCEINLINE simd_pack_t<4,double> simd::gather<4>(const double* base, const int* idx){
  return simd_pack_t<4,double>(_mm256_i32gather_pd(base, _mm_loadu_si128((const __m128i*)idx), 8));
}
#endif

#if defined(__AVX__) || defined(_M_AVX)
// stream_store AVX
template<> SIMD_FORCEINLINE void simd::stream_store<8>(float* p,  const simd_pack_t<8,float>& v){ _mm256_stream_ps(p, v.m); }
template<> SIMD_FORCEINLINE void simd::stream_store<4>(double* p, const simd_pack_t<4,double>& v){ _mm256_stream_pd(p, v.m); }
#endif

#if defined(__SSE2__) || defined(_M_X64)
template<> SIMD_FORCEINLINE void simd::stream_store<4>(float* p,  const simd_pack_t<4,float>& v){ _mm_stream_ps(p, v.m); }
template<> SIMD_FORCEINLINE void simd::stream_store<2>(double* p, const simd_pack_t<2,double>& v){ _mm_stream_pd(p, v.m); }
#endif

// NEON: no hay stream store estándar; usamos fallback (ya definido)




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
