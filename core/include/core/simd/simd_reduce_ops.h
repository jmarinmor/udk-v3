// simd_reduce_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

  template<int D, typename T> SIMD_FORCEINLINE
  T hadd(const simd_pack_t<D,T>& v) {
    T s = T(0);
    for (int i=0;i<D;++i) s += (&v.x)[i];
    return s;
  }

  template<int D, typename T> SIMD_FORCEINLINE
  T hmin(const simd_pack_t<D,T>& v) {
    T m = (&v.x)[0];
    for (int i=1;i<D;++i) if ((&v.x)[i] < m) m = (&v.x)[i];
    return m;
  }

  template<int D, typename T> SIMD_FORCEINLINE
  T hmax(const simd_pack_t<D,T>& v) {
    T m = (&v.x)[0];
    for (int i=1;i<D;++i) if ((&v.x)[i] > m) m = (&v.x)[i];
    return m;
  }

  // ======================================================================
// ESPECIALIZACIONES SIMD (SSE/AVX/NEON) — sin #if dentro de las funciones
// ======================================================================

#if defined(__AVX__) || defined(_M_AVX)
// --------------------- AVX -------------------------
template<> SIMD_FORCEINLINE float  hadd<8,float >(const simd_pack_t<8,float >& v){
  __m256 x = v.m;
  __m128 hi = _mm256_extractf128_ps(x,1);
  __m128 lo = _mm256_castps256_ps128(x);
  __m128 s  = _mm_add_ps(hi, lo);                    // 4 parciales
  __m128 sh = _mm_movehdup_ps(s);                    // [y y w w]
  __m128 s1 = _mm_add_ps(s, sh);                     // [x+y, y+y, z+w, w+w]
  sh = _mm_movehl_ps(sh, s1);                        // [z+w, w+w]
  __m128 s2 = _mm_add_ss(s1, sh);
  return _mm_cvtss_f32(s2);
}
template<> SIMD_FORCEINLINE double hadd<4,double>(const simd_pack_t<4,double>& v){
  __m256d x = v.m;
  __m128d hi = _mm256_extractf128_pd(x,1);
  __m128d lo = _mm256_castpd256_pd128(x);
  __m128d s  = _mm_add_pd(hi, lo);                   // 2 parciales
  __m128d sh = _mm_unpackhi_pd(s, s);
  __m128d s2 = _mm_add_sd(s, sh);
  return _mm_cvtsd_f64(s2);
}

template<> SIMD_FORCEINLINE float  hmin<8,float >(const simd_pack_t<8,float >& v){
  __m256 x=v.m; __m128 hi=_mm256_extractf128_ps(x,1), lo=_mm256_castps256_ps128(x);
  __m128 m=_mm_min_ps(hi,lo);
  __m128 sh=_mm_movehdup_ps(m); m=_mm_min_ps(m,sh);
  sh=_mm_movehl_ps(sh,m); m=_mm_min_ss(m,sh);
  return _mm_cvtss_f32(m);
}
template<> SIMD_FORCEINLINE double hmin<4,double>(const simd_pack_t<4,double>& v){
  __m256d x=v.m; __m128d hi=_mm256_extractf128_pd(x,1), lo=_mm256_castpd256_pd128(x);
  __m128d m=_mm_min_pd(hi,lo);
  __m128d sh=_mm_unpackhi_pd(m,m); m=_mm_min_sd(m,sh);
  return _mm_cvtsd_f64(m);
}

template<> SIMD_FORCEINLINE float  hmax<8,float >(const simd_pack_t<8,float >& v){
  __m256 x=v.m; __m128 hi=_mm256_extractf128_ps(x,1), lo=_mm256_castps256_ps128(x);
  __m128 m=_mm_max_ps(hi,lo);
  __m128 sh=_mm_movehdup_ps(m); m=_mm_max_ps(m,sh);
  sh=_mm_movehl_ps(sh,m); m=_mm_max_ss(m,sh);
  return _mm_cvtss_f32(m);
}
template<> SIMD_FORCEINLINE double hmax<4,double>(const simd_pack_t<4,double>& v){
  __m256d x=v.m; __m128d hi=_mm256_extractf128_pd(x,1), lo=_mm256_castpd256_pd128(x);
  __m128d m=_mm_max_pd(hi,lo);
  __m128d sh=_mm_unpackhi_pd(m,m); m=_mm_max_sd(m,sh);
  return _mm_cvtsd_f64(m);
}
#endif // AVX


#if defined(__SSE2__) || defined(_M_X64) || (defined(__SSE__) || (defined(_M_IX86_FP) && _M_IX86_FP>=1))
// --------------------- SSE -------------------------
template<> SIMD_FORCEINLINE float  hadd<4,float >(const simd_pack_t<4,float >& v){
  __m128 s=v.m;
  __m128 sh=_mm_movehdup_ps(s);           // [y y w w]
  __m128 s1=_mm_add_ps(s,sh);             // [x+y, y+y, z+w, w+w]
  sh=_mm_movehl_ps(sh,s1);                // [z+w, w+w]
  __m128 s2=_mm_add_ss(s1,sh);            // sum lane0
  return _mm_cvtss_f32(s2);
}
template<> SIMD_FORCEINLINE double hadd<2,double>(const simd_pack_t<2,double>& v){
  __m128d s=v.m;
  __m128d sh=_mm_unpackhi_pd(s,s);
  __m128d s2=_mm_add_sd(s,sh);
  return _mm_cvtsd_f64(s2);
}

template<> SIMD_FORCEINLINE float  hmin<4,float >(const simd_pack_t<4,float >& v){
  __m128 m=v.m;
  __m128 sh=_mm_movehdup_ps(m); m=_mm_min_ps(m,sh);
  sh=_mm_movehl_ps(sh,m);      m=_mm_min_ss(m,sh);
  return _mm_cvtss_f32(m);
}
template<> SIMD_FORCEINLINE double hmin<2,double>(const simd_pack_t<2,double>& v){
  __m128d m=v.m;
  __m128d sh=_mm_unpackhi_pd(m,m); m=_mm_min_sd(m,sh);
  return _mm_cvtsd_f64(m);
}

template<> SIMD_FORCEINLINE float  hmax<4,float >(const simd_pack_t<4,float >& v){
  __m128 m=v.m;
  __m128 sh=_mm_movehdup_ps(m); m=_mm_max_ps(m,sh);
  sh=_mm_movehl_ps(sh,m);      m=_mm_max_ss(m,sh);
  return _mm_cvtss_f32(m);
}
template<> SIMD_FORCEINLINE double hmax<2,double>(const simd_pack_t<2,double>& v){
  __m128d m=v.m;
  __m128d sh=_mm_unpackhi_pd(m,m); m=_mm_max_sd(m,sh);
  return _mm_cvtsd_f64(m);
}
#endif // SSE


#if defined(__ARM_NEON) || defined(__ARM_NEON__)
// --------------------- NEON -------------------------
template<> SIMD_FORCEINLINE float  hadd<4,float >(const simd_pack_t<4,float >& v){
#if defined(__aarch64__)
  return vaddvq_f32(v.m);
#else
  float32x2_t s2 = vadd_f32(vget_low_f32(v.m), vget_high_f32(v.m));
  float32x2_t s1 = vpadd_f32(s2, s2);
  return vget_lane_f32(s1,0);
#endif
}
template<> SIMD_FORCEINLINE float  hmin<4,float >(const simd_pack_t<4,float >& v){
#if defined(__aarch64__)
  return vminvq_f32(v.m);
#else
  float32x2_t lo=vget_low_f32(v.m), hi=vget_high_f32(v.m);
  float32x2_t m2=vmin_f32(lo,hi);
  float32x2_t m1=vpmin_f32(m2,m2);
  return vget_lane_f32(m1,0);
#endif
}
template<> SIMD_FORCEINLINE float  hmax<4,float >(const simd_pack_t<4,float >& v){
#if defined(__aarch64__)
  return vmaxvq_f32(v.m);
#else
  float32x2_t lo=vget_low_f32(v.m), hi=vget_high_f32(v.m);
  float32x2_t m2=vmax_f32(lo,hi);
  float32x2_t m1=vpmax_f32(m2,m2);
  return vget_lane_f32(m1,0);
#endif
}

#if defined(__aarch64__)
// double2 (AArch64)
template<> SIMD_FORCEINLINE double hadd<2,double>(const simd_pack_t<2,double>& v){
  float64x2_t s=v.m;
  return vgetq_lane_f64(vaddq_f64(s, vextq_f64(s,s,1)),0);
}
template<> SIMD_FORCEINLINE double hmin<2,double>(const simd_pack_t<2,double>& v){
  float64x2_t m = vminq_f64(v.m, vextq_f64(v.m, v.m, 1));
  return vgetq_lane_f64(m,0);
}
template<> SIMD_FORCEINLINE double hmax<2,double>(const simd_pack_t<2,double>& v){
  float64x2_t m = vmaxq_f64(v.m, vextq_f64(v.m, v.m, 1));
  return vgetq_lane_f64(m,0);
}
#endif // aarch64
#endif // NEON

} // namespace simd
