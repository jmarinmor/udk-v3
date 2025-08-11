// simd_fma_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

  // -------- Fallback por-lane --------
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fma(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b, const simd_pack_t<D,T>& c){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = (&a.x)[i]*(&b.x)[i] + (&c.x)[i];
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fmsub(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b, const simd_pack_t<D,T>& c){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = (&a.x)[i]*(&b.x)[i] - (&c.x)[i];
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fnmadd(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b, const simd_pack_t<D,T>& c){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = -((&a.x)[i]*(&b.x)[i]) + (&c.x)[i];
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fnmsub(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b, const simd_pack_t<D,T>& c){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = -((&a.x)[i]*(&b.x)[i]) - (&c.x)[i];
    return r;
  }

} // namespace simd

// ==================== ESPECIALIZACIONES SIMD ====================
#if defined(__FMA__) || (defined(__AVX2__) && defined(_MSC_VER)) || defined(_M_FMA)
// FMA3: 128 y 256 bits
template<> SIMD_FORCEINLINE simd_pack_t<8,float>  simd::fma  (const simd_pack_t<8,float>& a,  const simd_pack_t<8,float>& b,  const simd_pack_t<8,float>& c){ return simd_pack_t<8,float> ( _mm256_fmadd_ps (a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<8,float>  simd::fmsub(const simd_pack_t<8,float>& a,  const simd_pack_t<8,float>& b,  const simd_pack_t<8,float>& c){ return simd_pack_t<8,float> ( _mm256_fmsub_ps (a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<8,float>  simd::fnmadd(const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b, const simd_pack_t<8,float>& c){ return simd_pack_t<8,float> ( _mm256_fnmadd_ps(a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<8,float>  simd::fnmsub(const simd_pack_t<8,float>& a, const simd_pack_t<8,float>& b, const simd_pack_t<8,float>& c){ return simd_pack_t<8,float> ( _mm256_fnmsub_ps(a.m,b.m,c.m) ); }

template<> SIMD_FORCEINLINE simd_pack_t<4,double> simd::fma  (const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b, const simd_pack_t<4,double>& c){ return simd_pack_t<4,double>( _mm256_fmadd_pd (a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,double> simd::fmsub(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b, const simd_pack_t<4,double>& c){ return simd_pack_t<4,double>( _mm256_fmsub_pd (a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,double> simd::fnmadd(const simd_pack_t<4,double>& a,const simd_pack_t<4,double>& b,const simd_pack_t<4,double>& c){ return simd_pack_t<4,double>( _mm256_fnmadd_pd(a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,double> simd::fnmsub(const simd_pack_t<4,double>& a,const simd_pack_t<4,double>& b,const simd_pack_t<4,double>& c){ return simd_pack_t<4,double>( _mm256_fnmsub_pd(a.m,b.m,c.m) ); }

template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fma  (const simd_pack_t<4,float>& a,  const simd_pack_t<4,float>& b,  const simd_pack_t<4,float>& c){ return simd_pack_t<4,float> ( _mm_fmadd_ps (a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fmsub(const simd_pack_t<4,float>& a,  const simd_pack_t<4,float>& b,  const simd_pack_t<4,float>& c){ return simd_pack_t<4,float> ( _mm_fmsub_ps (a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<2,double> simd::fma  (const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b, const simd_pack_t<2,double>& c){ return simd_pack_t<2,double>( _mm_fmadd_pd (a.m,b.m,c.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<2,double> simd::fmsub(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b, const simd_pack_t<2,double>& c){ return simd_pack_t<2,double>( _mm_fmsub_pd (a.m,b.m,c.m) ); }
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
// NEON: AArch64 tiene VFMA/VFMS
#if defined(__aarch64__)
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fma  (const simd_pack_t<4,float>& a,  const simd_pack_t<4,float>& b,  const simd_pack_t<4,float>& c){ return simd_pack_t<4,float>(  vfmaq_f32(c.m, a.m, b.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fmsub(const simd_pack_t<4,float>& a,  const simd_pack_t<4,float>& b,  const simd_pack_t<4,float>& c){ return simd_pack_t<4,float>(  vsubq_f32( vfmaq_f32(c.m, a.m, b.m), vaddq_f32(c.m,c.m) ) ); /* c+a*b - c - c => a*b - c */ }
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fnmadd(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b, const simd_pack_t<4,float>& c){ return simd_pack_t<4,float>( vnegq_f32( vfmaq_f32(vnegq_f32(c.m), a.m, b.m) ) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fnmsub(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b, const simd_pack_t<4,float>& c){ return simd_pack_t<4,float>( vnegq_f32( vfmaq_f32(c.m, a.m, b.m) ) ); }

template<> SIMD_FORCEINLINE simd_pack_t<2,double> simd::fma  (const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b, const simd_pack_t<2,double>& c){ return simd_pack_t<2,double>( vfmaq_f64(c.m, a.m, b.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<2,double> simd::fmsub(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b, const simd_pack_t<2,double>& c){ return simd_pack_t<2,double>( vsubq_f64( vfmaq_f64(c.m, a.m, b.m), vaddq_f64(c.m,c.m) ) ); }
template<> SIMD_FORCEINLINE simd_pack_t<2,double> simd::fnmadd(const simd_pack_t<2,double>& a,const simd_pack_t<2,double>& b,const simd_pack_t<2,double>& c){ return simd_pack_t<2,double>( vnegq_f64( vfmaq_f64(vnegq_f64(c.m), a.m, b.m) ) ); }
template<> SIMD_FORCEINLINE simd_pack_t<2,double> simd::fnmsub(const simd_pack_t<2,double>& a,const simd_pack_t<2,double>& b,const simd_pack_t<2,double>& c){ return simd_pack_t<2,double>( vnegq_f64( vfmaq_f64(c.m, a.m, b.m) ) ); }
#else
// ARMv7 NEON: vmla/vmls no son "fused" pero sirven como fast fallback SIMD
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fma  (const simd_pack_t<4,float>& a,  const simd_pack_t<4,float>& b,  const simd_pack_t<4,float>& c){ return simd_pack_t<4,float>( vmlaq_f32(c.m, a.m, b.m) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fmsub(const simd_pack_t<4,float>& a,  const simd_pack_t<4,float>& b,  const simd_pack_t<4,float>& c){ return simd_pack_t<4,float>( vsubq_f32( vmulq_f32(a.m,b.m), c.m ) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fnmadd(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b, const simd_pack_t<4,float>& c){ return simd_pack_t<4,float>( vsubq_f32( c.m, vmulq_f32(a.m,b.m) ) ); }
template<> SIMD_FORCEINLINE simd_pack_t<4,float>  simd::fnmsub(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b, const simd_pack_t<4,float>& c){ return simd_pack_t<4,float>( vnegq_f32( vmlaq_f32(c.m, a.m, b.m) ) ); }
#endif
#endif
