// simd_mask_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

  // ------------------------------------------------------------
  // Comparaciones → máscara 0/1 (mismo T que el dato)
  // ------------------------------------------------------------
  template<int D, typename T> SIMD_FORCEINLINE
  simd_pack_t<D,T> cmp_eq(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = ((&a.x)[i] == (&b.x)[i]) ? T(1) : T(0);
    return r;
  }
  template<int D, typename T> SIMD_FORCEINLINE
  simd_pack_t<D,T> cmp_ne(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = ((&a.x)[i] != (&b.x)[i]) ? T(1) : T(0);
    return r;
  }
  template<int D, typename T> SIMD_FORCEINLINE
  simd_pack_t<D,T> cmp_lt(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = ((&a.x)[i] <  (&b.x)[i]) ? T(1) : T(0);
    return r;
  }
  template<int D, typename T> SIMD_FORCEINLINE
  simd_pack_t<D,T> cmp_le(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = ((&a.x)[i] <= (&b.x)[i]) ? T(1) : T(0);
    return r;
  }
  template<int D, typename T> SIMD_FORCEINLINE
  simd_pack_t<D,T> cmp_gt(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = ((&a.x)[i] >  (&b.x)[i]) ? T(1) : T(0);
    return r;
  }
  template<int D, typename T> SIMD_FORCEINLINE
  simd_pack_t<D,T> cmp_ge(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = ((&a.x)[i] >= (&b.x)[i]) ? T(1) : T(0);
    return r;
  }

  // ------------------------------------------------------------
  // select (blend): m!=0 ? a : b
  // ------------------------------------------------------------
  template<int D, typename Tm, typename T> SIMD_FORCEINLINE
  simd_pack_t<D,T> select(const simd_pack_t<D,Tm>& m,
                          const simd_pack_t<D,T>& a,
                          const simd_pack_t<D,T>& b)
  {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = ((&m.x)[i] != Tm(0)) ? (&a.x)[i] : (&b.x)[i];
    return r;
  }

  // ------------------------------------------------------------
  // any / all sobre máscara (m!=0)
  // ------------------------------------------------------------
  template<int D, typename Tm> SIMD_FORCEINLINE
  bool any(const simd_pack_t<D,Tm>& m) {
    for (int i=0;i<D;++i) if ((&m.x)[i] != Tm(0)) return true;
    return false;
  }
  template<int D, typename Tm> SIMD_FORCEINLINE
  bool all(const simd_pack_t<D,Tm>& m) {
    for (int i=0;i<D;++i) if ((&m.x)[i] == Tm(0)) return false;
    return true;
  }

  // ------------------------------------------------------------
  // movemask genérico: bit i = (m[i] != 0)
  // ------------------------------------------------------------
  template<int D, typename Tm> SIMD_FORCEINLINE
  unsigned movemask(const simd_pack_t<D,Tm>& m) {
    unsigned bits = 0u;
    for (int i=0;i<D;++i) if ((&m.x)[i] != Tm(0)) bits |= (1u << i);
    return bits;
  }



  // ======================================================================
// ESPECIALIZACIONES SIMD (SSE/AVX/NEON) — sin #if dentro de las funciones
// ======================================================================

#if defined(__AVX__) || defined(_M_AVX)
// --------------------- AVX (float8 / double4) -------------------------
template<> SIMD_FORCEINLINE
simd_pack_t<8,float> cmp_eq(const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b){
  __m256 m = _mm256_cmp_ps(a.m, b.m, _CMP_EQ_OQ);                   // 0xFFFFFFFF / 0x00000000
  __m256 ones = _mm256_set1_ps(1.0f);
  return simd_pack_t<8,float>(_mm256_and_ps(m, ones));               // 1.0f / 0.0f
}
template<> SIMD_FORCEINLINE
simd_pack_t<8,float> cmp_lt(const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b){
  __m256 m = _mm256_cmp_ps(a.m, b.m, _CMP_LT_OQ);
  __m256 ones = _mm256_set1_ps(1.0f);
  return simd_pack_t<8,float>(_mm256_and_ps(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<8,float> cmp_le(const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b){
  __m256 m = _mm256_cmp_ps(a.m, b.m, _CMP_LE_OQ);
  __m256 ones = _mm256_set1_ps(1.0f);
  return simd_pack_t<8,float>(_mm256_and_ps(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<8,float> cmp_gt(const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b){
  __m256 m = _mm256_cmp_ps(a.m, b.m, _CMP_GT_OQ);
  __m256 ones = _mm256_set1_ps(1.0f);
  return simd_pack_t<8,float>(_mm256_and_ps(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<8,float> cmp_ge(const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b){
  __m256 m = _mm256_cmp_ps(a.m, b.m, _CMP_GE_OQ);
  __m256 ones = _mm256_set1_ps(1.0f);
  return simd_pack_t<8,float>(_mm256_and_ps(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<8,float> cmp_ne(const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b){
  __m256 m = _mm256_cmp_ps(a.m, b.m, _CMP_NEQ_OQ);
  __m256 ones = _mm256_set1_ps(1.0f);
  return simd_pack_t<8,float>(_mm256_and_ps(m, ones));
}

template<> SIMD_FORCEINLINE
simd_pack_t<8,float> select(const simd_pack_t<8,float>& m01, const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b){
  // m01 es 0.0/1.0 → conviértelo a máscara completa con cmp_ne(m,0)
  __m256 mask = _mm256_cmp_ps(m01.m, _mm256_setzero_ps(), _CMP_NEQ_OQ);
  return simd_pack_t<8,float>(_mm256_blendv_ps(b.m, a.m, mask));
}

template<> SIMD_FORCEINLINE
bool any(const simd_pack_t<8,float>& m01){
  __m256 mask = _mm256_cmp_ps(m01.m, _mm256_setzero_ps(), _CMP_NEQ_OQ);
  return _mm256_movemask_ps(mask) != 0;
}
template<> SIMD_FORCEINLINE
bool all(const simd_pack_t<8,float>& m01){
  __m256 mask = _mm256_cmp_ps(m01.m, _mm256_setzero_ps(), _CMP_NEQ_OQ);
  return _mm256_movemask_ps(mask) == 0xFF;
}
template<> SIMD_FORCEINLINE
unsigned movemask(const simd_pack_t<8,float>& m01){
  __m256 mask = _mm256_cmp_ps(m01.m, _mm256_setzero_ps(), _CMP_NEQ_OQ);
  return (unsigned)_mm256_movemask_ps(mask);
}

// double4
template<> SIMD_FORCEINLINE
simd_pack_t<4,double> cmp_eq(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b){
  __m256d m = _mm256_cmp_pd(a.m, b.m, _CMP_EQ_OQ);
  __m256d ones = _mm256_set1_pd(1.0);
  return simd_pack_t<4,double>(_mm256_and_pd(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,double> cmp_lt(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b){
  __m256d m = _mm256_cmp_pd(a.m, b.m, _CMP_LT_OQ);
  __m256d ones = _mm256_set1_pd(1.0);
  return simd_pack_t<4,double>(_mm256_and_pd(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,double> cmp_le(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b){
  __m256d m = _mm256_cmp_pd(a.m, b.m, _CMP_LE_OQ);
  __m256d ones = _mm256_set1_pd(1.0);
  return simd_pack_t<4,double>(_mm256_and_pd(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,double> cmp_gt(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b){
  __m256d m = _mm256_cmp_pd(a.m, b.m, _CMP_GT_OQ);
  __m256d ones = _mm256_set1_pd(1.0);
  return simd_pack_t<4,double>(_mm256_and_pd(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,double> cmp_ge(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b){
  __m256d m = _mm256_cmp_pd(a.m, b.m, _CMP_GE_OQ);
  __m256d ones = _mm256_set1_pd(1.0);
  return simd_pack_t<4,double>(_mm256_and_pd(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,double> cmp_ne(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b){
  __m256d m = _mm256_cmp_pd(a.m, b.m, _CMP_NEQ_OQ);
  __m256d ones = _mm256_set1_pd(1.0);
  return simd_pack_t<4,double>(_mm256_and_pd(m, ones));
}

template<> SIMD_FORCEINLINE
simd_pack_t<4,double> select(const simd_pack_t<4,double>& m01, const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b){
  __m256d mask = _mm256_cmp_pd(m01.m, _mm256_setzero_pd(), _CMP_NEQ_OQ);
  return simd_pack_t<4,double>(_mm256_blendv_pd(b.m, a.m, mask));
}

template<> SIMD_FORCEINLINE
bool any(const simd_pack_t<4,double>& m01){
  __m256d mask = _mm256_cmp_pd(m01.m, _mm256_setzero_pd(), _CMP_NEQ_OQ);
  return _mm256_movemask_pd(mask) != 0;
}
template<> SIMD_FORCEINLINE
bool all(const simd_pack_t<4,double>& m01){
  __m256d mask = _mm256_cmp_pd(m01.m, _mm256_setzero_pd(), _CMP_NEQ_OQ);
  return _mm256_movemask_pd(mask) == 0xF;
}
template<> SIMD_FORCEINLINE
unsigned movemask(const simd_pack_t<4,double>& m01){
  __m256d mask = _mm256_cmp_pd(m01.m, _mm256_setzero_pd(), _CMP_NEQ_OQ);
  return (unsigned)_mm256_movemask_pd(mask);
}
#endif // AVX


#if defined(__SSE2__) || defined(_M_X64) || (defined(__SSE__) || (defined(_M_IX86_FP) && _M_IX86_FP>=1))
// --------------------- SSE (float4 / double2) --------------------------
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_eq(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 m = _mm_cmpeq_ps(a.m, b.m);
  __m128 ones = _mm_set1_ps(1.0f);
  return simd_pack_t<4,float>(_mm_and_ps(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_lt(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 m = _mm_cmplt_ps(a.m, b.m);
  return simd_pack_t<4,float>(_mm_and_ps(m, _mm_set1_ps(1.0f)));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_le(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 m = _mm_cmple_ps(a.m, b.m);
  return simd_pack_t<4,float>(_mm_and_ps(m, _mm_set1_ps(1.0f)));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_gt(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 m = _mm_cmpgt_ps(a.m, b.m);
  return simd_pack_t<4,float>(_mm_and_ps(m, _mm_set1_ps(1.0f)));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_ge(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 m = _mm_cmpge_ps(a.m, b.m);
  return simd_pack_t<4,float>(_mm_and_ps(m, _mm_set1_ps(1.0f)));
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_ne(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 m = _mm_cmpneq_ps(a.m, b.m);
  return simd_pack_t<4,float>(_mm_and_ps(m, _mm_set1_ps(1.0f)));
}

template<> SIMD_FORCEINLINE
simd_pack_t<4,float> select(const simd_pack_t<4,float>& m01, const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 mask = _mm_cmpneq_ps(m01.m, _mm_setzero_ps());               // 0xFFFFFFFF/0
  return simd_pack_t<4,float>(_mm_or_ps(_mm_and_ps(mask, a.m),
                                        _mm_andnot_ps(mask, b.m)));
}

template<> SIMD_FORCEINLINE
bool any(const simd_pack_t<4,float>& m01){
  __m128 mask = _mm_cmpneq_ps(m01.m, _mm_setzero_ps());
  return _mm_movemask_ps(mask) != 0;
}
template<> SIMD_FORCEINLINE
bool all(const simd_pack_t<4,float>& m01){
  __m128 mask = _mm_cmpneq_ps(m01.m, _mm_setzero_ps());
  return _mm_movemask_ps(mask) == 0xF;
}
template<> SIMD_FORCEINLINE
unsigned movemask(const simd_pack_t<4,float>& m01){
  __m128 mask = _mm_cmpneq_ps(m01.m, _mm_setzero_ps());
  return (unsigned)_mm_movemask_ps(mask);
}

// double2
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_eq(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  __m128d m = _mm_cmpeq_pd(a.m, b.m);
  __m128d ones = _mm_set1_pd(1.0);
  return simd_pack_t<2,double>(_mm_and_pd(m, ones));
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_lt(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  __m128d m = _mm_cmplt_pd(a.m, b.m);
  return simd_pack_t<2,double>(_mm_and_pd(m, _mm_set1_pd(1.0)));
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_le(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  __m128d m = _mm_cmple_pd(a.m, b.m);
  return simd_pack_t<2,double>(_mm_and_pd(m, _mm_set1_pd(1.0)));
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_gt(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  __m128d m = _mm_cmpgt_pd(a.m, b.m);
  return simd_pack_t<2,double>(_mm_and_pd(m, _mm_set1_pd(1.0)));
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_ge(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  __m128d m = _mm_cmpge_pd(a.m, b.m);
  return simd_pack_t<2,double>(_mm_and_pd(m, _mm_set1_pd(1.0)));
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_ne(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  __m128d m = _mm_cmpneq_pd(a.m, b.m);
  return simd_pack_t<2,double>(_mm_and_pd(m, _mm_set1_pd(1.0)));
}

template<> SIMD_FORCEINLINE
simd_pack_t<2,double> select(const simd_pack_t<2,double>& m01, const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  __m128d mask = _mm_cmpneq_pd(m01.m, _mm_setzero_pd());
  return simd_pack_t<2,double>(_mm_or_pd(_mm_and_pd(mask, a.m),
                                         _mm_andnot_pd(mask, b.m)));
}

template<> SIMD_FORCEINLINE
bool any(const simd_pack_t<2,double>& m01){
  __m128d mask = _mm_cmpneq_pd(m01.m, _mm_setzero_pd());
  return _mm_movemask_pd(mask) != 0;
}
template<> SIMD_FORCEINLINE
bool all(const simd_pack_t<2,double>& m01){
  __m128d mask = _mm_cmpneq_pd(m01.m, _mm_setzero_pd());
  return _mm_movemask_pd(mask) == 0x3;
}
template<> SIMD_FORCEINLINE
unsigned movemask(const simd_pack_t<2,double>& m01){
  __m128d mask = _mm_cmpneq_pd(m01.m, _mm_setzero_pd());
  return (unsigned)_mm_movemask_pd(mask);
}
#endif // SSE/SSE2


#if defined(__ARM_NEON) || defined(__ARM_NEON__)
// --------------------- NEON (float4 / (AArch64: double2)) --------------
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_eq(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  uint32x4_t m = vceqq_f32(a.m, b.m);                         // 0xFFFFFFFF/0
  float32x4_t ones = vdupq_n_f32(1.0f);
  return simd_pack_t<4,float>( vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_lt(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  uint32x4_t m = vcltq_f32(a.m, b.m);
  float32x4_t ones = vdupq_n_f32(1.0f);
  return simd_pack_t<4,float>( vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_le(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  uint32x4_t m = vcleq_f32(a.m, b.m);
  float32x4_t ones = vdupq_n_f32(1.0f);
  return simd_pack_t<4,float>( vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_gt(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  uint32x4_t m = vcgtq_f32(a.m, b.m);
  float32x4_t ones = vdupq_n_f32(1.0f);
  return simd_pack_t<4,float>( vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_ge(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  uint32x4_t m = vcgeq_f32(a.m, b.m);
  float32x4_t ones = vdupq_n_f32(1.0f);
  return simd_pack_t<4,float>( vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<4,float> cmp_ne(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  uint32x4_t m = vmvnq_u32(vceqq_f32(a.m, b.m));
  float32x4_t ones = vdupq_n_f32(1.0f);
  return simd_pack_t<4,float>( vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(ones), m)) );
}

template<> SIMD_FORCEINLINE
simd_pack_t<4,float> select(const simd_pack_t<4,float>& m01, const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  // m01 es 0.0/1.0 → conviértelo a máscara entera
  uint32x4_t m = vcgtq_f32(m01.m, vdupq_n_f32(0.0f));         // 0xFFFFFFFF/0
  return simd_pack_t<4,float>( vbslq_f32(m, a.m, b.m) );
}

template<> SIMD_FORCEINLINE
bool any(const simd_pack_t<4,float>& m01){
  uint32x4_t m = vcgtq_f32(m01.m, vdupq_n_f32(0.0f));
#if defined(__aarch64__)
  return vaddvq_u32(m) != 0u;
#else
  uint64x2_t p = vreinterpretq_u64_u32(m);
  return (vgetq_lane_u64(p,0) | vgetq_lane_u64(p,1)) != 0ull;
#endif
}
template<> SIMD_FORCEINLINE
bool all(const simd_pack_t<4,float>& m01){
  uint32x4_t m = vcgtq_f32(m01.m, vdupq_n_f32(0.0f));
#if defined(__aarch64__)
  return vaddvq_u32(vmvnq_u32(m)) == 0u;
#else
  uint32x4_t n = vmvnq_u32(m);
  uint64x2_t p = vreinterpretq_u64_u32(n);
  return (vgetq_lane_u64(p,0) | vgetq_lane_u64(p,1)) == 0ull;
#endif
}
template<> SIMD_FORCEINLINE
unsigned movemask(const simd_pack_t<4,float>& m01){
  // Extrae bit de signo simulando movemask: usa comparación y empaqueta
  uint32x4_t m = vcgtq_f32(m01.m, vdupq_n_f32(0.0f));
  uint16x4_t p0 = vmovn_u32(m);
  uint8x8_t  p1 = vmovn_u16(vcombine_u16(p0,p0));
  uint8x8_t  sh = {1,2,4,8,0,0,0,0};
  uint8x8_t  b  = vand_u8(p1, sh);
  return (unsigned)(vget_lane_u8(b,0) | vget_lane_u8(b,1) | vget_lane_u8(b,2) | vget_lane_u8(b,3));
}

#if defined(__aarch64__)
// double2 en AArch64
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_lt(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  uint64x2_t m = vcltq_f64(a.m, b.m);
  float64x2_t ones = vdupq_n_f64(1.0);
  return simd_pack_t<2,double>( vreinterpretq_f64_u64(vandq_u64(vreinterpretq_u64_f64(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_le(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  uint64x2_t m = vcleq_f64(a.m, b.m);
  float64x2_t ones = vdupq_n_f64(1.0);
  return simd_pack_t<2,double>( vreinterpretq_f64_u64(vandq_u64(vreinterpretq_u64_f64(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_gt(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  uint64x2_t m = vcgtq_f64(a.m, b.m);
  float64x2_t ones = vdupq_n_f64(1.0);
  return simd_pack_t<2,double>( vreinterpretq_f64_u64(vandq_u64(vreinterpretq_u64_f64(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_ge(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  uint64x2_t m = vcgeq_f64(a.m, b.m);
  float64x2_t ones = vdupq_n_f64(1.0);
  return simd_pack_t<2,double>( vreinterpretq_f64_u64(vandq_u64(vreinterpretq_u64_f64(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_eq(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  uint64x2_t m = vceqq_f64(a.m, b.m);
  float64x2_t ones = vdupq_n_f64(1.0);
  return simd_pack_t<2,double>( vreinterpretq_f64_u64(vandq_u64(vreinterpretq_u64_f64(ones), m)) );
}
template<> SIMD_FORCEINLINE
simd_pack_t<2,double> cmp_ne(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  uint64x2_t m = vmvnq_u64(vceqq_f64(a.m, b.m));
  float64x2_t ones = vdupq_n_f64(1.0);
  return simd_pack_t<2,double>( vreinterpretq_f64_u64(vandq_u64(vreinterpretq_u64_f64(ones), m)) );
}

template<> SIMD_FORCEINLINE
simd_pack_t<2,double> select(const simd_pack_t<2,double>& m01, const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
  uint64x2_t m = vcgtq_f64(m01.m, vdupq_n_f64(0.0));
  return simd_pack_t<2,double>( vbslq_f64(m, a.m, b.m) );
}
template<> SIMD_FORCEINLINE
bool any(const simd_pack_t<2,double>& m01){
  uint64x2_t m = vcgtq_f64(m01.m, vdupq_n_f64(0.0));
  return (vgetq_lane_u64(m,0) | vgetq_lane_u64(m,1)) != 0ull;
}
template<> SIMD_FORCEINLINE
bool all(const simd_pack_t<2,double>& m01){
  uint64x2_t m = vcgtq_f64(m01.m, vdupq_n_f64(0.0));
  return (vgetq_lane_u64(vmvnq_u64(m),0) | vgetq_lane_u64(vmvnq_u64(m),1)) == 0ull;
}
template<> SIMD_FORCEINLINE
unsigned movemask(const simd_pack_t<2,double>& m01){
  // empaqueta dos bits
  uint64x2_t m = vcgtq_f64(m01.m, vdupq_n_f64(0.0));
  return (unsigned)((vgetq_lane_u64(m,0)?1u:0u) | (vgetq_lane_u64(m,1)?2u:0u));
}
#endif // AArch64 double
#endif // NEON



} // namespace simd


