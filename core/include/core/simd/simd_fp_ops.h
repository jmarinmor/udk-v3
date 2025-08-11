// simd_fp_ops.h
#pragma once

#include <simd/simd_types.h>

namespace simd {

  // ==========================================================
  //  Declaraciones genéricas (ESCALAR)
  // ==========================================================
  // mod: fmod (hacia 0) -> a - b * trunc(a/b)
  template<typename T> SIMD_FORCEINLINE T mod (const T& a, const T& b);
  // modf: devuelve frac y escribe int en *iptr
  template<typename T> SIMD_FORCEINLINE T modf(const T& x, T* iptr);
  template<typename T> SIMD_FORCEINLINE T abs (const T& v);
  template<typename T> SIMD_FORCEINLINE T sign(const T& v); // -1, 0, +1 (como float/double)
  template<typename T> SIMD_FORCEINLINE T floor(const T& v);
  template<typename T> SIMD_FORCEINLINE T trunc(const T& v);
  template<typename T> SIMD_FORCEINLINE T ceil (const T& v);
  template<typename T> SIMD_FORCEINLINE T fract(const T& v); // v - floor(v)
  template<typename T> SIMD_FORCEINLINE T isnan(const T& v);   // 1.0 si NaN, 0.0 si no
  template<typename T> SIMD_FORCEINLINE T isinf(const T& v);   // 1.0 si +-inf
  template<typename T> SIMD_FORCEINLINE T isfinite(const T& v);// 1.0 si finito

  // ---------- ESCALAR: float/double ----------
  template<> SIMD_FORCEINLINE float  mod<float >(const float&  a, const float&  b) { return std::fmod(a,b); }
  template<> SIMD_FORCEINLINE double mod<double>(const double& a, const double& b) { return std::fmod(a,b); }

  template<> SIMD_FORCEINLINE float  modf<float >(const float&  x, float*  ip) { float  i; float  f = std::modf(x, &i); *ip = i; return f; }
  template<> SIMD_FORCEINLINE double modf<double>(const double& x, double* ip) { double i; double f = std::modf(x, &i); *ip = i; return f; }

  template<> SIMD_FORCEINLINE float  abs<float >(const float&  v) { return std::fabs(v); }
  template<> SIMD_FORCEINLINE double abs<double>(const double& v) { return std::fabs(v); }

  template<> SIMD_FORCEINLINE float  sign<float >(const float&  v) { return (v>0.f) - (v<0.f); }
  template<> SIMD_FORCEINLINE double sign<double>(const double& v) { return (v>0.0) - (v<0.0); }

  template<> SIMD_FORCEINLINE float  floor<float >(const float&  v) { return std::floor(v); }
  template<> SIMD_FORCEINLINE double floor<double>(const double& v) { return std::floor(v); }

  template<> SIMD_FORCEINLINE float  trunc<float >(const float&  v) { return std::trunc(v); }
  template<> SIMD_FORCEINLINE double trunc<double>(const double& v) { return std::trunc(v); }

  template<> SIMD_FORCEINLINE float  ceil<float >(const float&  v) { return std::ceil(v); }
  template<> SIMD_FORCEINLINE double ceil<double>(const double& v) { return std::ceil(v); }

  template<> SIMD_FORCEINLINE float  fract<float >(const float&  v) { return v - std::floor(v); }
  template<> SIMD_FORCEINLINE double fract<double>(const double& v) { return v - std::floor(v); }

  template<> SIMD_FORCEINLINE float  isnan<float >(const float&  v) { return std::isnan(v) ? 1.f : 0.f; }
  template<> SIMD_FORCEINLINE double isnan<double>(const double& v) { return std::isnan(v) ? 1.0 : 0.0; }

  template<> SIMD_FORCEINLINE float  isinf<float >(const float&  v) { return std::isinf(v) ? 1.f : 0.f; }
  template<> SIMD_FORCEINLINE double isinf<double>(const double& v) { return std::isinf(v) ? 1.0 : 0.0; }

  template<> SIMD_FORCEINLINE float  isfinite<float >(const float&  v) { return std::isfinite(v) ? 1.f : 0.f; }
  template<> SIMD_FORCEINLINE double isfinite<double>(const double& v) { return std::isfinite(v) ? 1.0 : 0.0; }

  // ==========================================================
  //  Ayudas SIMD (por plataforma) — sin ifdefs dentro
  // ==========================================================
  // --- SSE helpers (ps/pd)
#if defined(__SSE2__) || defined(_M_X64) || (defined(__SSE4_1__) || defined(_M_IX86_FP))
  // máscaras constantes
  static SIMD_FORCEINLINE __m128  sse_abs_ps(__m128 a){ const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)); return _mm_and_ps(a,mask); }
  static SIMD_FORCEINLINE __m128d sse_abs_pd(__m128d a){ const __m128d mask = _mm_castsi128_pd(_mm_set1_epi64x(0x7fffffffffffffffLL)); return _mm_and_pd(a,mask); }
  static SIMD_FORCEINLINE __m128  sse_isnan_ps(__m128 a){ return _mm_cmpunord_ps(a,a); } // NaN -> all ones
  static SIMD_FORCEINLINE __m128d sse_isnan_pd(__m128d a){ return _mm_cmpunord_pd(a,a); }
  static SIMD_FORCEINLINE __m128  sse_isinf_ps(__m128 a){
    const __m128 inf = _mm_castsi128_ps(_mm_set1_epi32(0x7f800000));
    return _mm_andnot_ps(_mm_cmpunord_ps(a,a), _mm_cmpeq_ps(sse_abs_ps(a), inf));
  }
  static SIMD_FORCEINLINE __m128d sse_isinf_pd(__m128d a){
    const __m128d inf = _mm_castsi128_pd(_mm_set1_epi64x(0x7ff0000000000000LL));
    return _mm_andnot_pd(_mm_cmpunord_pd(a,a), _mm_cmpeq_pd(sse_abs_pd(a), inf));
  }
  static SIMD_FORCEINLINE __m128  sse_isfinite_ps(__m128 a){
    // finite = !isnan && !isinf
    __m128 isn = sse_isnan_ps(a);
    __m128 isi = sse_isinf_ps(a);
    return _mm_andnot_ps(_mm_or_ps(isn, isi), _mm_castsi128_ps(_mm_set1_epi32(-1)));
  }
  static SIMD_FORCEINLINE __m128d sse_isfinite_pd(__m128d a){
    __m128d isn = sse_isnan_pd(a);
    __m128d isi = sse_isinf_pd(a);
    return _mm_andnot_pd(_mm_or_pd(isn, isi), _mm_castsi128_pd(_mm_set1_epi64x(-1)));
  }
#endif

  // --- NEON AArch64 helpers
#if defined(__aarch64__)
  static SIMD_FORCEINLINE float32x4_t neon_abs_f32x4(float32x4_t a){ return vabsq_f32(a); }
  static SIMD_FORCEINLINE float64x2_t neon_abs_f64x2(float64x2_t a){ return vabsq_f64(a); }
  static SIMD_FORCEINLINE uint32x4_t neon_isnan_f32x4(float32x4_t a){ return vmvnq_u32(vceqq_f32(a,a)); }
  static SIMD_FORCEINLINE uint64x2_t neon_isnan_f64x2(float64x2_t a){ return vmvnq_u64(vceqq_f64(a,a)); }
  static SIMD_FORCEINLINE uint32x4_t neon_isinf_f32x4(float32x4_t a){
    const float32x4_t inf = vdupq_n_f32(INFINITY);
    return vandq_u32(vmvnq_u32(vceqq_f32(a,a)), vceqq_f32(vabsq_f32(a), inf)); // (!NaN) & (|a|==inf)
  }
  static SIMD_FORCEINLINE uint64x2_t neon_isinf_f64x2(float64x2_t a){
    const float64x2_t inf = vdupq_n_f64(INFINITY);
    return vandq_u64(vmvnq_u64(vceqq_f64(a,a)), vceqq_f64(vabsq_f64(a), inf));
  }
  static SIMD_FORCEINLINE uint32x4_t neon_isfinite_f32x4(float32x4_t a){
    uint32x4_t isn = neon_isnan_f32x4(a), isi = neon_isinf_f32x4(a);
    return vmvnq_u32(vorrq_u32(isn, isi));
  }
  static SIMD_FORCEINLINE uint64x2_t neon_isfinite_f64x2(float64x2_t a){
    uint64x2_t isn = neon_isnan_f64x2(a), isi = neon_isinf_f64x2(a);
    return vmvnq_u64(vorrq_u64(isn, isi));
  }
#endif

  // ==========================================================
  //  Especializaciones SIMD: FLOAT4  (simd_pack_t<4,float>)
  // ==========================================================
#if defined(__aarch64__)
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> abs (const simd_pack_t<4,float>& v){ return simd_pack_t<4,float>(vabsq_f32(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> floor(const simd_pack_t<4,float>& v){ return simd_pack_t<4,float>(vrndmq_f32(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> trunc(const simd_pack_t<4,float>& v){ return simd_pack_t<4,float>(vrndq_f32 (v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> ceil (const simd_pack_t<4,float>& v){ return simd_pack_t<4,float>(vrndpq_f32(v.m)); }

  template<> SIMD_FORCEINLINE simd_pack_t<4,float> fract(const simd_pack_t<4,float>& v){
    float32x4_t fv = v.m;
    return simd_pack_t<4,float>( vsubq_f32(fv, vrndmq_f32(fv)) );
  }

  template<> SIMD_FORCEINLINE simd_pack_t<4,float> isnan(const simd_pack_t<4,float>& v){
    return simd_pack_t<4,float>( vcvtq_f32_u32(neon_isnan_f32x4(v.m)) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> isinf(const simd_pack_t<4,float>& v){
    return simd_pack_t<4,float>( vcvtq_f32_u32(neon_isinf_f32x4(v.m)) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> isfinite(const simd_pack_t<4,float>& v){
    return simd_pack_t<4,float>( vcvtq_f32_u32(neon_isfinite_f32x4(v.m)) );
  }

  // mod, modf, sign:
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> trunc_div(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
    // helper interno, no exportado: q = trunc(a/b)
    return simd_pack_t<4,float>( vrndq_f32(vdivq_f32(a.m, b.m)) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> mod(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
    float32x4_t q = vrndq_f32(vdivq_f32(a.m, b.m));
    return simd_pack_t<4,float>( vsubq_f32(a.m, vmulq_f32(b.m, q)) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> modf(const simd_pack_t<4,float>& x, simd_pack_t<4,float>* ip){
    float32x4_t i = vrndq_f32(x.m);
    *ip = simd_pack_t<4,float>(i);
    return simd_pack_t<4,float>( vsubq_f32(x.m, i) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> sign(const simd_pack_t<4,float>& v){
    float32x4_t gt0 = vcvtq_f32_u32( vcgtq_f32(v.m, vdupq_n_f32(0.0f)) );
    float32x4_t lt0 = vcvtq_f32_u32( vcltq_f32(v.m, vdupq_n_f32(0.0f)) );
    return simd_pack_t<4,float>( vsubq_f32(gt0, lt0) );
  }

#elif defined(__SSE2__) || defined(_M_X64)
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> abs (const simd_pack_t<4,float>& v){ return simd_pack_t<4,float>(sse_abs_ps(v.m)); }

  #if defined(__SSE4_1__) || defined(_M_X64)
    template<> SIMD_FORCEINLINE simd_pack_t<4,float> floor(const simd_pack_t<4,float>& v){ return simd_pack_t<4,float>(_mm_floor_ps(v.m)); }
    template<> SIMD_FORCEINLINE simd_pack_t<4,float> trunc(const simd_pack_t<4,float>& v){ return simd_pack_t<4,float>(_mm_round_ps(v.m, _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC)); }
    template<> SIMD_FORCEINLINE simd_pack_t<4,float> ceil (const simd_pack_t<4,float>& v){ return simd_pack_t<4,float>(_mm_ceil_ps (v.m)); }
  #endif

  template<> SIMD_FORCEINLINE simd_pack_t<4,float> fract(const simd_pack_t<4,float>& v){
  #if defined(__SSE4_1__) || defined(_M_X64)
    __m128 i = _mm_round_ps(v.m, _MM_FROUND_TO_NEG_INF|_MM_FROUND_NO_EXC);
    return simd_pack_t<4,float>(_mm_sub_ps(v.m, i));
  #else
    // fallback: escalar por lanes (si no hay SSE4.1)
    simd_pack_t<4,float> out; float tmp[4]; _mm_storeu_ps(tmp, v.m);
    for (int k=0;k<4;++k) tmp[k] = tmp[k] - std::floor(tmp[k]);
    out = simd_pack_t<4,float>(_mm_loadu_ps(tmp)); return out;
  #endif
  }

  template<> SIMD_FORCEINLINE simd_pack_t<4,float> isnan(const simd_pack_t<4,float>& v){
    return simd_pack_t<4,float>( _mm_and_ps( sse_isnan_ps(v.m), _mm_set1_ps(1.0f) ) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> isinf(const simd_pack_t<4,float>& v){
    return simd_pack_t<4,float>( _mm_and_ps( sse_isinf_ps(v.m), _mm_set1_ps(1.0f) ) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> isfinite(const simd_pack_t<4,float>& v){
    return simd_pack_t<4,float>( _mm_and_ps( sse_isfinite_ps(v.m), _mm_set1_ps(1.0f) ) );
  }

  template<> SIMD_FORCEINLINE simd_pack_t<4,float> mod(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  #if defined(__SSE4_1__) || defined(_M_X64)
    __m128 q = _mm_round_ps(_mm_div_ps(a.m,b.m), _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC);
    return simd_pack_t<4,float>(_mm_sub_ps(a.m, _mm_mul_ps(b.m, q)));
  #else
    // fallback escalar por lanes (sin redondeo vectorial hacia cero)
    simd_pack_t<4,float> out; float aa[4], bb[4];
    _mm_storeu_ps(aa, a.m); _mm_storeu_ps(bb, b.m);
    for (int k=0;k<4;++k) aa[k] = std::fmod(aa[k], bb[k]);
    out = simd_pack_t<4,float>(_mm_loadu_ps(aa)); return out;
  #endif
  }

  template<> SIMD_FORCEINLINE simd_pack_t<4,float> modf(const simd_pack_t<4,float>& x, simd_pack_t<4,float>* ip){
  #if defined(__SSE4_1__) || defined(_M_X64)
    __m128 i = _mm_round_ps(x.m, _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC);
    *ip = simd_pack_t<4,float>(i);
    return simd_pack_t<4,float>(_mm_sub_ps(x.m, i));
  #else
    float t[4]; _mm_storeu_ps(t, x.m);
    float i[4]; for(int k=0;k<4;++k){ float ii; t[k]=std::modf(t[k], &ii); i[k]=ii; }
    *ip = simd_pack_t<4,float>(_mm_loadu_ps(i));
    return simd_pack_t<4,float>(_mm_loadu_ps(t));
  #endif
  }

  template<> SIMD_FORCEINLINE simd_pack_t<4,float> sign(const simd_pack_t<4,float>& v){
    __m128 gt0 = _mm_castsi128_ps(_mm_cmplt_epi32(_mm_setzero_si128(), _mm_castps_si128(v.m))); // v>0
    __m128 lt0 = _mm_castsi128_ps(_mm_cmpgt_epi32(_mm_setzero_si128(), _mm_castps_si128(v.m))); // v<0
    return simd_pack_t<4,float>(_mm_sub_ps(_mm_and_ps(gt0,_mm_set1_ps(1.0f)), _mm_and_ps(lt0,_mm_set1_ps(1.0f))));
  }
#endif // SSE / AArch64

  // ==========================================================
  //  FLOAT2 (simd_pack_t<2,float>) — AArch64; otros: fallback
  // ==========================================================
#if defined(__aarch64__)
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> abs (const simd_pack_t<2,float>& v){ return simd_pack_t<2,float>(vabs_f32(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> floor(const simd_pack_t<2,float>& v){ return simd_pack_t<2,float>(vrndm_f32(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> trunc(const simd_pack_t<2,float>& v){ return simd_pack_t<2,float>(vrnd_f32 (v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> ceil (const simd_pack_t<2,float>& v){ return simd_pack_t<2,float>(vrndp_f32(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> fract(const simd_pack_t<2,float>& v){ return simd_pack_t<2,float>( vsub_f32(v.m, vrndm_f32(v.m)) ); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> isnan(const simd_pack_t<2,float>& v){ return simd_pack_t<2,float>( vcvt_f32_u32(vmvn_u32(vceq_f32(v.m,v.m))) ); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> isinf(const simd_pack_t<2,float>& v){
    const float32x2_t inf = vdup_n_f32(INFINITY);
    uint32x2_t m = vand_u32(vmvn_u32(vceq_f32(v.m,v.m)), vceq_f32(vabs_f32(v.m), inf));
    return simd_pack_t<2,float>( vcvt_f32_u32(m) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> isfinite(const simd_pack_t<2,float>& v){
    uint32x2_t m = vmvn_u32(vorr_u32( vmvn_u32(vceq_f32(v.m,v.m)),
                                       vceq_f32(vabs_f32(v.m), vdup_n_f32(INFINITY)) ));
    return simd_pack_t<2,float>( vcvt_f32_u32(m) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> mod(const simd_pack_t<2,float>& a, const simd_pack_t<2,float>& b){
    float32x2_t q = vrnd_f32(vdiv_f32(a.m,b.m));
    return simd_pack_t<2,float>( vsub_f32(a.m, vmul_f32(b.m, q)) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> modf(const simd_pack_t<2,float>& x, simd_pack_t<2,float>* ip){
    float32x2_t i = vrnd_f32(x.m); *ip = simd_pack_t<2,float>(i); return simd_pack_t<2,float>( vsub_f32(x.m, i) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,float> sign(const simd_pack_t<2,float>& v){
    float32x2_t gt0 = vcvt_f32_u32( vcgt_f32(v.m, vdup_n_f32(0.0f)) );
    float32x2_t lt0 = vcvt_f32_u32( vclt_f32(v.m, vdup_n_f32(0.0f)) );
    return simd_pack_t<2,float>( vsub_f32(gt0, lt0) );
  }
#endif

  // ==========================================================
  //  DOUBLE2 (simd_pack_t<2,double>) — AArch64 y SSE2
  // ==========================================================
#if defined(__aarch64__)
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> abs (const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>(vabsq_f64(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> floor(const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>(vrndmq_f64(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> trunc(const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>(vrndq_f64 (v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> ceil (const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>(vrndpq_f64(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> fract(const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>( vsubq_f64(v.m, vrndmq_f64(v.m)) ); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> isnan(const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>( vcvtq_f64_u64( vmvnq_u64(vceqq_f64(v.m,v.m)) ) ); }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> isinf(const simd_pack_t<2,double>& v){
    uint64x2_t m = vandq_u64( vmvnq_u64(vceqq_f64(v.m,v.m)), vceqq_f64(vabsq_f64(v.m), vdupq_n_f64(INFINITY)) );
    return simd_pack_t<2,double>( vcvtq_f64_u64(m) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> isfinite(const simd_pack_t<2,double>& v){
    uint64x2_t m = vmvnq_u64( vorrq_u64( vmvnq_u64(vceqq_f64(v.m,v.m)),
                                         vceqq_f64(vabsq_f64(v.m), vdupq_n_f64(INFINITY)) ) );
    return simd_pack_t<2,double>( vcvtq_f64_u64(m) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> mod(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
    float64x2_t q = vrndq_f64(vdivq_f64(a.m,b.m));
    return simd_pack_t<2,double>( vsubq_f64(a.m, vmulq_f64(b.m, q)) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> modf(const simd_pack_t<2,double>& x, simd_pack_t<2,double>* ip){
    float64x2_t i = vrndq_f64(x.m); *ip = simd_pack_t<2,double>(i); return simd_pack_t<2,double>( vsubq_f64(x.m, i) );
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> sign(const simd_pack_t<2,double>& v){
    float64x2_t gt0 = vcvtq_f64_u64( vcgtq_f64(v.m, vdupq_n_f64(0.0)) );
    float64x2_t lt0 = vcvtq_f64_u64( vcltq_f64(v.m, vdupq_n_f64(0.0)) );
    return simd_pack_t<2,double>( vsubq_f64(gt0, lt0) );
  }

#elif defined(__SSE2__) || defined(_M_X64)
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> abs (const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>(sse_abs_pd(v.m)); }
  // floor/ceil/trunc sólo SSE4.1. Si no, fallback escalar por lanes.
  #if defined(__SSE4_1__) || defined(_M_X64)
    template<> SIMD_FORCEINLINE simd_pack_t<2,double> floor(const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>(_mm_floor_pd(v.m)); }
    template<> SIMD_FORCEINLINE simd_pack_t<2,double> trunc(const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>(_mm_round_pd(v.m, _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC)); }
    template<> SIMD_FORCEINLINE simd_pack_t<2,double> ceil (const simd_pack_t<2,double>& v){ return simd_pack_t<2,double>(_mm_ceil_pd (v.m)); }
    template<> SIMD_FORCEINLINE simd_pack_t<2,double> fract(const simd_pack_t<2,double>& v){
      __m128d i = _mm_floor_pd(v.m); return simd_pack_t<2,double>(_mm_sub_pd(v.m, i));
    }
    template<> SIMD_FORCEINLINE simd_pack_t<2,double> mod(const simd_pack_t<2,double>& a, const simd_pack_t<2,double>& b){
      __m128d q = _mm_round_pd(_mm_div_pd(a.m,b.m), _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC);
      return simd_pack_t<2,double>(_mm_sub_pd(a.m, _mm_mul_pd(b.m, q)));
    }
    template<> SIMD_FORCEINLINE simd_pack_t<2,double> modf(const simd_pack_t<2,double>& x, simd_pack_t<2,double>* ip){
      __m128d i = _mm_round_pd(x.m, _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC);
      *ip = simd_pack_t<2,double>(i); return simd_pack_t<2,double>(_mm_sub_pd(x.m, i));
    }
  #endif
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> isnan(const simd_pack_t<2,double>& v){
    return simd_pack_t<2,double>(_mm_and_pd( sse_isnan_pd(v.m), _mm_set1_pd(1.0) ));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> isinf(const simd_pack_t<2,double>& v){
    return simd_pack_t<2,double>(_mm_and_pd( sse_isinf_pd(v.m), _mm_set1_pd(1.0) ));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> isfinite(const simd_pack_t<2,double>& v){
    return simd_pack_t<2,double>(_mm_and_pd( sse_isfinite_pd(v.m), _mm_set1_pd(1.0) ));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> sign(const simd_pack_t<2,double>& v){
    __m128d gt0 = _mm_castsi128_pd(_mm_cmpgt_epi32(_mm_castpd_si128(v.m), _mm_setzero_si128()));
    __m128d lt0 = _mm_castsi128_pd(_mm_cmplt_epi32(_mm_castpd_si128(v.m), _mm_setzero_si128()));
    return simd_pack_t<2,double>(_mm_sub_pd(_mm_and_pd(gt0,_mm_set1_pd(1.0)), _mm_and_pd(lt0,_mm_set1_pd(1.0))));
  }
#endif

  // ==========================================================
  //  DOUBLE4 (simd_pack_t<4,double>) — AVX
  // ==========================================================
#if defined(__AVX__) || defined(_M_AVX)
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> abs (const simd_pack_t<4,double>& v){
    const __m256d mask = _mm256_castsi256_pd(_mm256_set1_epi64x(0x7fffffffffffffffLL));
    return simd_pack_t<4,double>(_mm256_and_pd(v.m, mask));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> floor(const simd_pack_t<4,double>& v){ return simd_pack_t<4,double>(_mm256_floor_pd(v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> trunc(const simd_pack_t<4,double>& v){ return simd_pack_t<4,double>(_mm256_round_pd(v.m, _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> ceil (const simd_pack_t<4,double>& v){ return simd_pack_t<4,double>(_mm256_ceil_pd (v.m)); }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> fract(const simd_pack_t<4,double>& v){
    __m256d i = _mm256_floor_pd(v.m); return simd_pack_t<4,double>(_mm256_sub_pd(v.m, i));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> mod(const simd_pack_t<4,double>& a, const simd_pack_t<4,double>& b){
    __m256d q = _mm256_round_pd(_mm256_div_pd(a.m,b.m), _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC);
    return simd_pack_t<4,double>(_mm256_sub_pd(a.m, _mm256_mul_pd(b.m, q)));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> modf(const simd_pack_t<4,double>& x, simd_pack_t<4,double>* ip){
    __m256d i = _mm256_round_pd(x.m, _MM_FROUND_TO_ZERO|_MM_FROUND_NO_EXC);
    *ip = simd_pack_t<4,double>(i); return simd_pack_t<4,double>(_mm256_sub_pd(x.m, i));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> isnan(const simd_pack_t<4,double>& v){
    __m256d m = _mm256_cmp_pd(v.m, v.m, _CMP_UNORD_Q);
    return simd_pack_t<4,double>(_mm256_and_pd(m, _mm256_set1_pd(1.0)));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> isinf(const simd_pack_t<4,double>& v){
    const __m256d inf = _mm256_castsi256_pd(_mm256_set1_epi64x(0x7ff0000000000000LL));
    __m256d notnan = _mm256_cmp_pd(v.m, v.m, _CMP_ORD_Q);
    __m256d abse   = _mm256_and_pd(v.m, _mm256_castsi256_pd(_mm256_set1_epi64x(0x7fffffffffffffffLL)));
    __m256d eqinf  = _mm256_cmp_pd(abse, inf, _CMP_EQ_OQ);
    return simd_pack_t<4,double>(_mm256_and_pd(notnan, _mm256_and_pd(eqinf, _mm256_set1_pd(1.0))));
  }
  template<> SIMD_FORCEINLINE simd_pack_t<4,double> isfinite(const simd_pack_t<4,double>& v){
    __m256d isn = _mm256_cmp_pd(v.m, v.m, _CMP_UNORD_Q);
    const __m256d inf = _mm256_castsi256_pd(_mm256_set1_epi64x(0x7ff0000000000000LL));
    __m256d abse = _mm256_and_pd(v.m, _mm256_castsi256_pd(_mm256_set1_epi64x(0x7fffffffffffffffLL)));
    __m256d isi  = _mm256_cmp_pd(abse, inf, _CMP_EQ_OQ);
    __m256d no   = _mm256_andnot_pd(_mm256_or_pd(isn, isi), _mm256_set1_pd(1.0));
    return simd_pack_t<4,double>(no);
  }
#endif

} // namespace simd
