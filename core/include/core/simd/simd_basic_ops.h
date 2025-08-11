// simd_basic_ops.inl
#pragma once

#include <simd/simd_types.h>

namespace simd {

  // ==========================================================
  //  Declaraciones base (fallback escalar)
  // ==========================================================
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> add(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b);

  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> sub(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b);

  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> mul(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b);

  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> div(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b);

  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> min(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b);

  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> max(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b);

  // Fallbacks escalares (genéricos)
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> add(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = (&a.x)[i] + (&b.x)[i];
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> sub(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = (&a.x)[i] - (&b.x)[i];
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> mul(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = (&a.x)[i] * (&b.x)[i];
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> div(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = (&a.x)[i] / (&b.x)[i];
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> min(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) { const T ax=(&a.x)[i], bx=(&b.x)[i]; (&r.x)[i] = ax < bx ? ax : bx; }
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> max(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) { const T ax=(&a.x)[i], bx=(&b.x)[i]; (&r.x)[i] = ax > bx ? ax : bx; }
    return r;
  }

  // ==========================================================
  //  Helpers por plataforma (sin ifdefs dentro de funciones)
  // ==========================================================
  // NEON ARMv7: div aproximada para float (vrecpe + 2x NR)
#if (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(__aarch64__)
  SIMD_FORCEINLINE float32x4_t neon_div_f32x4(float32x4_t a, float32x4_t b) {
    float32x4_t x = vrecpeq_f32(b);
    x = vmulq_f32(x, vrecpsq_f32(b, x));
    x = vmulq_f32(x, vrecpsq_f32(b, x));
    return vmulq_f32(a, x);
  }
  SIMD_FORCEINLINE float32x2_t neon_div_f32x2(float32x2_t a, float32x2_t b) {
    float32x2_t x = vrecpe_f32(b);
    x = vmul_f32(x, vrecps_f32(b, x));
    x = vmul_f32(x, vrecps_f32(b, x));
    return vmul_f32(a, x);
  }
#endif

  // ==========================================================
  //  Especializaciones SIMD — FLOAT4 (simd_pack_t<4,float>)
  // ==========================================================
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  add<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(vaddq_f32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  sub<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(vsubq_f32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  mul<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(vmulq_f32(a.m, b.m)); }

  #if defined(__aarch64__)
    template<> SIMD_FORCEINLINE simd_pack_t<4,float>
    div<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(vdivq_f32(a.m, b.m)); }
  #else
    template<> SIMD_FORCEINLINE simd_pack_t<4,float>
    div<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(neon_div_f32x4(a.m, b.m)); }
  #endif

  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  min<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(vminq_f32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  max<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(vmaxq_f32(a.m, b.m)); }

#elif defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  add<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(_mm_add_ps(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  sub<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(_mm_sub_ps(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  mul<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(_mm_mul_ps(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  div<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(_mm_div_ps(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  min<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(_mm_min_ps(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float>
  max<4,float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b) { return simd_pack_t<4,float>(_mm_max_ps(a.m, b.m)); }
#endif

  // ==========================================================
  //  Especializaciones SIMD — FLOAT2 (simd_pack_t<2,float>)
  // ==========================================================
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  template<> SIMD_FORCEINLINE simd_pack_t<2,float>
  add<2,float>(const simd_pack_t<2,float>& a, const simd_pack_t<2,float>& b) { return simd_pack_t<2,float>(vadd_f32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float>
  sub<2,float>(const simd_pack_t<2,float>& a, const simd_pack_t<2,float>& b) { return simd_pack_t<2,float>(vsub_f32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float>
  mul<2,float>(const simd_pack_t<2,float>& a, const simd_pack_t<2,float>& b) { return simd_pack_t<2,float>(vmul_f32(a.m, b.m)); }

  #if defined(__aarch64__)
    template<> SIMD_FORCEINLINE simd_pack_t<2,float>
    div<2,float>(const simd_pack_t<2,float>& a, const simd_pack_t<2,float>& b) { return simd_pack_t<2,float>(vdiv_f32(a.m, b.m)); }
  #else
    template<> SIMD_FORCEINLINE simd_pack_t<2,float>
    div<2,float>(const simd_pack_t<2,float>& a, const simd_pack_t<2,float>& b) { return simd_pack_t<2,float>(neon_div_f32x2(a.m, b.m)); }
  #endif

  template<> SIMD_FORCEINLINE simd_pack_t<2,float>
  min<2,float>(const simd_pack_t<2,float>& a, const simd_pack_t<2,float>& b) { return simd_pack_t<2,float>(vmin_f32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float>
  max<2,float>(const simd_pack_t<2,float>& a, const simd_pack_t<2,float>& b) { return simd_pack_t<2,float>(vmax_f32(a.m, b.m)); }
#endif
  // (En x86 no hay vec2 nativo: usar fallback escalar ya definido)

  // ==========================================================
  //  Especializaciones SIMD — DOUBLE2 (simd_pack_t<2,double>)
  // ==========================================================
#if defined(__aarch64__)
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  add<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(vaddq_f64(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  sub<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(vsubq_f64(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  mul<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(vmulq_f64(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  div<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(vdivq_f64(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  min<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(vminq_f64(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  max<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(vmaxq_f64(a.m, b.m)); }

#elif defined(__SSE2__) || defined(_M_X64)
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  add<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(_mm_add_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  sub<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(_mm_sub_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  mul<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(_mm_mul_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  div<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(_mm_div_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  min<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(_mm_min_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double>
  max<2,double>(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b) { return simd_pack_t<2,double>(_mm_max_pd(a.m, b.m)); }
#endif

  // ==========================================================
  //  Especializaciones SIMD — DOUBLE4 (simd_pack_t<4,double>, AVX)
  // ==========================================================
#if defined(__AVX__) || defined(_M_AVX)
  template<> SIMD_FORCEINLINE simd_pack_t<4,double>
  add<4,double>(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b) { return simd_pack_t<4,double>(_mm256_add_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double>
  sub<4,double>(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b) { return simd_pack_t<4,double>(_mm256_sub_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double>
  mul<4,double>(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b) { return simd_pack_t<4,double>(_mm256_mul_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double>
  div<4,double>(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b) { return simd_pack_t<4,double>(_mm256_div_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double>
  min<4,double>(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b) { return simd_pack_t<4,double>(_mm256_min_pd(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double>
  max<4,double>(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b) { return simd_pack_t<4,double>(_mm256_max_pd(a.m, b.m)); }
#endif

  // ==========================================================
  //  Especializaciones SIMD — INT32x4 / UINT32x4
  // ==========================================================
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  template<> SIMD_FORCEINLINE simd_pack_t<4,int>
  add<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(vaddq_s32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
  add<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(vaddq_u32(a.m, b.m)); }

  template<> SIMD_FORCEINLINE simd_pack_t<4,int>
  sub<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(vsubq_s32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
  sub<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(vsubq_u32(a.m, b.m)); }

  template<> SIMD_FORCEINLINE simd_pack_t<4,int>
  mul<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(vmulq_s32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
  mul<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(vmulq_u32(a.m, b.m)); }

  // div enteros -> fallback escalar (sin intrínseco)
  // min/max enteros
  template<> SIMD_FORCEINLINE simd_pack_t<4,int>
  min<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(vminq_s32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,int>
  max<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(vmaxq_s32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
  min<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(vminq_u32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
  max<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(vmaxq_u32(a.m, b.m)); }

#elif defined(__SSE2__) || defined(_M_X64)
  template<> SIMD_FORCEINLINE simd_pack_t<4,int>
  add<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(_mm_add_epi32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
  add<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(_mm_add_epi32(a.m, b.m)); }

  template<> SIMD_FORCEINLINE simd_pack_t<4,int>
  sub<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(_mm_sub_epi32(a.m, b.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
  sub<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(_mm_sub_epi32(a.m, b.m)); }

  // mul requiere SSE4.1 para _mm_mullo_epi32
  #if defined(__SSE4_1__) || defined(_M_X64)
    template<> SIMD_FORCEINLINE simd_pack_t<4,int>
    mul<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(_mm_mullo_epi32(a.m, b.m)); }
    template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
    mul<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(_mm_mullo_epi32(a.m, b.m)); }
  #endif

  // min/max requieren SSE4.1 en 32-bit
  #if defined(__SSE4_1__) || defined(_M_X64)
    template<> SIMD_FORCEINLINE simd_pack_t<4,int>
    min<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(_mm_min_epi32(a.m, b.m)); }
    template<> SIMD_FORCEINLINE simd_pack_t<4,int>
    max<4,int>(const simd_pack_t<4,int>& a, const simd_pack_t<4,int>& b) { return simd_pack_t<4,int>(_mm_max_epi32(a.m, b.m)); }
    template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
    min<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(_mm_min_epu32(a.m, b.m)); }
    template<> SIMD_FORCEINLINE simd_pack_t<4,unsigned>
    max<4,unsigned>(const simd_pack_t<4,unsigned>& a, const simd_pack_t<4,unsigned>& b) { return simd_pack_t<4,unsigned>(_mm_max_epu32(a.m, b.m)); }
  #endif
#endif

  // ==========================================================
  //  Operadores (vector-vector y vector-escalar)
  // ==========================================================
  // Helpers: splat escalar a pack
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> splat(T value) { return simd_pack_t<D,T>(value); }

  // Binarios vector-vector
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator+(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) { return add<D,T>(a,b); }
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator-(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) { return sub<D,T>(a,b); }
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator*(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) { return mul<D,T>(a,b); }
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator/(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) { return div<D,T>(a,b); }

  // Binarios vector-escalar (ambos órdenes)
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator+(const simd_pack_t<D,T>& a, T b) { return add<D,T>(a, splat<D,T>(b)); }
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator+(T a, const simd_pack_t<D,T>& b) { return add<D,T>(splat<D,T>(a), b); }

  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator-(const simd_pack_t<D,T>& a, T b) { return sub<D,T>(a, splat<D,T>(b)); }
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator-(T a, const simd_pack_t<D,T>& b) { return sub<D,T>(splat<D,T>(a), b); }

  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator*(const simd_pack_t<D,T>& a, T b) { return mul<D,T>(a, splat<D,T>(b)); }
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator*(T a, const simd_pack_t<D,T>& b) { return mul<D,T>(splat<D,T>(a), b); }

  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator/(const simd_pack_t<D,T>& a, T b) { return div<D,T>(a, splat<D,T>(b)); }
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator/(T a, const simd_pack_t<D,T>& b) { return div<D,T>(splat<D,T>(a), b); }

  // Unario negación
  template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> operator-(const simd_pack_t<D,T>& v) { return mul<D,T>(v, splat<D,T>(T(-1))); }


	template <typename T> SIMD_FORCEINLINE T rcp(const T& v); // 1/v

	template <typename T>
	SIMD_FORCEINLINE T rcp(const T& v) {
		return static_cast<T>(1.0 / static_cast<double>(v));
	}
	template <>
	SIMD_FORCEINLINE float rcp<float>(const float& v) { return 1.0f / v; }
	template <>
	SIMD_FORCEINLINE double rcp<double>(const double& v) { return 1.0 / v; }


	// ============================================================
	//  Helpers por plataforma (sin ifdefs dentro de funciones)
	// ============================================================
	// ---- NEON ARMv7: refinamientos NR para rsqrt/rcp y sqrt via rsqrt
#if (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(__aarch64__)
	SIMD_FORCEINLINE float32x4_t neon_rcp_nr_f32x4(float32x4_t a) {
		float32x4_t x = vrecpeq_f32(a);
		x = vmulq_f32(x, vrecpsq_f32(a, x));
		x = vmulq_f32(x, vrecpsq_f32(a, x));
		return x;
	}
	SIMD_FORCEINLINE float32x2_t neon_rcp_nr_f32x2(float32x2_t a) {
		float32x2_t x = vrecpe_f32(a);
		x = vmul_f32(x, vrecps_f32(a, x));
		x = vmul_f32(x, vrecps_f32(a, x));
		return x;
	}
#endif

#if defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
	SIMD_FORCEINLINE __m128 sse_rcp_nr_ps(__m128 a) {
		__m128 x = _mm_rcp_ps(a);
		const __m128 two = _mm_set1_ps(2.0f);
		x = _mm_mul_ps(x, _mm_sub_ps(two, _mm_mul_ps(a, x)));
		x = _mm_mul_ps(x, _mm_sub_ps(two, _mm_mul_ps(a, x)));
		return x;
	}
#endif

#if defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	sqrt(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(vsqrtq_f32(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rsq(const simd_pack_t<4, float>& v) {
		float32x4_t x = vrsqrteq_f32(v.m);
		x = vmulq_f32(x, vrsqrtsq_f32(vmulq_f32(v.m, x), x));
		x = vmulq_f32(x, vrsqrtsq_f32(vmulq_f32(v.m, x), x));
		return simd_pack_t<4, float>(x);
	}

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rcp(const simd_pack_t<4, float>& v) {
		float32x4_t x = vrecpeq_f32(v.m);
		x = vmulq_f32(x, vrecpsq_f32(v.m, x));
		x = vmulq_f32(x, vrecpsq_f32(v.m, x));
		return simd_pack_t<4, float>(x);
	}
#elif (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	sqrt(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(neon_sqrt_via_rsqrt_f32x4(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rsq(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(neon_rsqrt_nr_f32x4(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rcp(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(neon_rcp_nr_f32x4(v.m)); }
#elif defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rcp(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(sse_rcp_nr_ps(v.m)); }
#else
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rcp(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(1.f / v.x, 1.f / v.y, 1.f / v.z, 1.f / v.w); }
#endif


#if defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rcp(const simd_pack_t<2, float>& v) {
		float32x2_t x = vrecpe_f32(v.m);
		x = vmul_f32(x, vrecps_f32(v.m, x));
		x = vmul_f32(x, vrecps_f32(v.m, x));
		return simd_pack_t<2, float>(x);
	}
#elif (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rcp(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(neon_rcp_nr_f32x2(v.m)); }
#else
	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rcp(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(1.f / v.x, 1.f / v.y); }
#endif


	// ============================================================
	//  Especializaciones SIMD — DOUBLE2 (simd_pack_t<2,double>)
	// ============================================================
#if defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rcp(const simd_pack_t<2, double>& v) {
		float64x2_t one = vdupq_n_f64(1.0);
		return simd_pack_t<2, double>(vdivq_f64(one, v.m));
	}
#elif defined(__SSE2__) || defined(_M_X64)
	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rcp(const simd_pack_t<2, double>& v) {
		__m128d one = _mm_set1_pd(1.0);
		return simd_pack_t<2, double>(_mm_div_pd(one, v.m));
	}
#else
	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rcp(const simd_pack_t<2, double>& v) { return simd_pack_t<2, double>(1.0 / v.x, 1.0 / v.y); }
#endif

	// ============================================================
	//  Especializaciones SIMD — DOUBLE4 (simd_pack_t<4,double>)
	// ============================================================
#if defined(__AVX__) || defined(_M_AVX)
	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	rcp(const simd_pack_t<4, double>& v) {
		__m256d one = _mm256_set1_pd(1.0);
		return simd_pack_t<4, double>(_mm256_div_pd(one, v.m));
	}
#else
	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	rcp(const simd_pack_t<4, double>& v) { return simd_pack_t<4, double>(1.0 / v.x, 1.0 / v.y, 1.0 / v.z, 1.0 / v.w); }
#endif

} // namespace simd
