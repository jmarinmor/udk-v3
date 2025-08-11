// simd_geom_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

// ============================================================
// Helpers genéricos (sin cmath)
// ============================================================
SIMD_FORCEINLINE float  scalar_sqrt(float  x) { if (x <= 0.0f) return 0.0f; float g = x > 1.0f ? x : 1.0f; // 3 iter.
  g = 0.5f*(g + x/g); g = 0.5f*(g + x/g); g = 0.5f*(g + x/g); return g; }
SIMD_FORCEINLINE double scalar_sqrt(double x) { if (x <= 0.0)  return 0.0;  double g = x > 1.0  ? x : 1.0;  // 3 iter.
  g = 0.5*(g + x/g);  g = 0.5*(g + x/g);  g = 0.5*(g + x/g);  return g; }

// Splat
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> splat(T v) { return simd_pack_t<D,T>(v); }

// ============================================================
// Fallback GENÉRICO por componentes (cubre cualquier D, T)
// ============================================================
template<int D, typename T>
SIMD_FORCEINLINE T dot(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
  T r = T(0);
  for (int i=0;i<D;++i) r += (&a.x)[i] * (&b.x)[i];
  return r;
}

template<typename T>
SIMD_FORCEINLINE T dot3(const simd_pack_t<3,T>& a, const simd_pack_t<3,T>& b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}
template<typename T>
SIMD_FORCEINLINE T dot3(const simd_pack_t<4,T>& a, const simd_pack_t<4,T>& b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

template<int D, typename T>
SIMD_FORCEINLINE T length(const simd_pack_t<D,T>& v) {
  T s = dot(v,v);
  if constexpr (std::is_same<T,float>::value)  return scalar_sqrt(s);
  if constexpr (std::is_same<T,double>::value) return scalar_sqrt(s);
  return s; // para tipos no flotantes (si los usas)
}

template<int D, typename T>
SIMD_FORCEINLINE T distance(const simd_pack_t<D,T>& a, const simd_pack_t<D,T>& b) {
  return length(b - a);
}

template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> normalize(const simd_pack_t<D,T>& v) {
  T len = length(v);
  return v / len;
}

// ============================================================
// faceforward / reflect / refract (en base a dot genérico)
// ============================================================
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> faceforward(const simd_pack_t<D,T>& N,
                                              const simd_pack_t<D,T>& I,
                                              const simd_pack_t<D,T>& Nref)
{
  return (dot(Nref, I) < T(0)) ? N : -N;
}

template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> reflect(const simd_pack_t<D,T>& I,
                                          const simd_pack_t<D,T>& N)
{
  return I - (splat<D,T>(T(2)) * dot(N, I)) * N;
}

template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> refract(const simd_pack_t<D,T>& I,
                                          const simd_pack_t<D,T>& N,
                                          T eta)
{
  T d = dot(N, I);
  T k = T(1) - eta*eta*(T(1) - d*d);
  if (k < T(0)) return simd_pack_t<D,T>(T(0));
  T t = (eta*d + (std::is_same<T,float>::value ? T(scalar_sqrt((double)k)) : T(scalar_sqrt(k))));
  return eta*I - t*N;
}

// ============================================================
// ESPECIALIZACIONES OPTIMIZADAS (sin ifdefs dentro)
// ============================================================

// ----------------------- NEON -----------------------
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  #include <arm_neon.h>

  // dot float4
  template<> SIMD_FORCEINLINE float dot<4,float>(const simd_pack_t<4,float>& a,
                                                 const simd_pack_t<4,float>& b) {
    float32x4_t p = vmulq_f32(a.m, b.m);
    float32x2_t s2 = vadd_f32(vget_low_f32(p), vget_high_f32(p));
    float32x2_t s1 = vpadd_f32(s2, s2);
    return vget_lane_f32(s1, 0);
  }

  // length float4
  template<> SIMD_FORCEINLINE float length<4,float>(const simd_pack_t<4,float>& v) {
    float s = dot(v,v);
  #if defined(__aarch64__)
    return vgetq_lane_f32(vsqrtq_f32(vdupq_n_f32(s)), 0); // usar sqrt hw para precisión
  #else
    // rsqrt + NR -> sqrt = s * rsqrt(s)
    float32x4_t ss = vdupq_n_f32(s);
    float32x4_t x = vrsqrteq_f32(ss);
    x = vmulq_f32(x, vrsqrtsq_f32(vmulq_f32(ss,x), x));
    x = vmulq_f32(x, vrsqrtsq_f32(vmulq_f32(ss,x), x));
    float32x4_t r = vmulq_f32(ss, x);
    return vgetq_lane_f32(r, 0);
  #endif
  }

  // normalize float4
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> normalize<4,float>(const simd_pack_t<4,float>& v) {
    // invlen por rsqrt
    float32x4_t p = vmulq_f32(v.m, v.m);
    float32x2_t s2 = vadd_f32(vget_low_f32(p), vget_high_f32(p));
    float32x2_t s1 = vpadd_f32(s2, s2);
    float32x4_t len2 = vdupq_n_f32(vget_lane_f32(s1,0));
  #if defined(__aarch64__)
    float32x4_t inv = vrsqrteq_f32(len2);
    inv = vmulq_f32(inv, vrsqrtsq_f32(vmulq_f32(len2,inv), inv));
    inv = vmulq_f32(inv, vrsqrtsq_f32(vmulq_f32(len2,inv), inv));
    return simd_pack_t<4,float>( vmulq_f32(v.m, inv) );
  #else
    float32x4_t inv = vrsqrteq_f32(len2);
    inv = vmulq_f32(inv, vrsqrtsq_f32(vmulq_f32(len2,inv), inv));
    inv = vmulq_f32(inv, vrsqrtsq_f32(vmulq_f32(len2,inv), inv));
    return simd_pack_t<4,float>( vmulq_f32(v.m, inv) );
  #endif
  }

  // dot float2 (AArch64 tiene f64; para float2 usamos lanes)
  #if defined(__aarch64__)
    template<> SIMD_FORCEINLINE float dot<2,float>(const simd_pack_t<2,float>& a,
                                                   const simd_pack_t<2,float>& b) {
      float32x2_t p = vmul_f32(a.m, b.m);
      float32x2_t s = vpadd_f32(p, p);
      return vget_lane_f32(s, 0);
    }
    template<> SIMD_FORCEINLINE float length<2,float>(const simd_pack_t<2,float>& v) {
      float s = dot(v,v);
      return vgetq_lane_f32(vsqrtq_f32(vdupq_n_f32(s)), 0);
    }
    template<> SIMD_FORCEINLINE simd_pack_t<2,float> normalize<2,float>(const simd_pack_t<2,float>& v) {
      float s = dot(v,v);
      float32x2_t ss = vdup_n_f32(s);
      float32x2_t inv = vrsqrte_f32(ss);
      inv = vmul_f32(inv, vrsqrts_f32(vmul_f32(ss,inv), inv));
      inv = vmul_f32(inv, vrsqrts_f32(vmul_f32(ss,inv), inv));
      return simd_pack_t<2,float>( vmul_f32(v.m, inv) );
    }
  #endif

  // dot/length/normalize double2 (AArch64)
  #if defined(__aarch64__)
    template<> SIMD_FORCEINLINE double dot<2,double>(const simd_pack_t<2,double>& a,
                                                     const simd_pack_t<2,double>& b) {
      float64x2_t p = vmulq_f64(a.m, b.m);
      float64x2_t s = vpaddq_f64(p, p);
      return vgetq_lane_f64(s, 0);
    }
    template<> SIMD_FORCEINLINE double length<2,double>(const simd_pack_t<2,double>& v) {
      double s = dot(v,v);
      float64x2_t r = vsqrtq_f64(vdupq_n_f64(s));
      return vgetq_lane_f64(r, 0);
    }
    template<> SIMD_FORCEINLINE simd_pack_t<2,double> normalize<2,double>(const simd_pack_t<2,double>& v) {
      double s = dot(v,v);
      float64x2_t ss = vdupq_n_f64(s);
      // invlen = 1/sqrt(s)
      float64x2_t inv = vdupq_n_f64(1.0);
      inv = vdivq_f64(inv, vsqrtq_f64(ss));
      return simd_pack_t<2,double>( vmulq_f64(v.m, inv) );
    }
  #endif
#endif // NEON

// ----------------------- SSE / SSE4.1 -----------------------
#if defined(__SSE2__) || defined(_M_X64) || (defined(__SSE__) || (defined(_M_IX86_FP) && _M_IX86_FP>=1))
  // dot float4
  #if defined(__SSE4_1__) || defined(_M_X64)
    template<> SIMD_FORCEINLINE float dot<4,float>(const simd_pack_t<4,float>& a,
                                                   const simd_pack_t<4,float>& b) {
      __m128 d = _mm_dp_ps(a.m, b.m, 0xF1); // sum all, result in lane0
      return _mm_cvtss_f32(d);
    }
  #else
    template<> SIMD_FORCEINLINE float dot<4,float>(const simd_pack_t<4,float>& a,
                                                   const simd_pack_t<4,float>& b) {
      __m128 p = _mm_mul_ps(a.m, b.m);
      __m128 sh = _mm_movehdup_ps(p);     // [y y w w]
      __m128 s1 = _mm_add_ps(p, sh);      // [x+y, y+y, z+w, w+w]
      sh = _mm_movehl_ps(sh, s1);         // [z+w, w+w]
      __m128 s2 = _mm_add_ss(s1, sh);     // sum in lane0
      return _mm_cvtss_f32(s2);
    }
  #endif

  // length float4
  template<> SIMD_FORCEINLINE float length<4,float>(const simd_pack_t<4,float>& v) {
    float s = dot(v,v);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(s)));
  }

  // normalize float4
  template<> SIMD_FORCEINLINE simd_pack_t<4,float> normalize<4,float>(const simd_pack_t<4,float>& v) {
    float s = dot(v,v);
    __m128 inv = _mm_rsqrt_ps(_mm_set_ps1(s));
    // 2x Newton-Raphson para precisión
    const __m128 half = _mm_set1_ps(0.5f);
    const __m128 three= _mm_set1_ps(3.0f);
    __m128 axx = _mm_mul_ps(_mm_set1_ps(s), _mm_mul_ps(inv, inv));
    inv = _mm_mul_ps(_mm_mul_ps(half, inv), _mm_sub_ps(three, axx));
    axx = _mm_mul_ps(_mm_set1_ps(s), _mm_mul_ps(inv, inv));
    inv = _mm_mul_ps(_mm_mul_ps(half, inv), _mm_sub_ps(three, axx));
    return simd_pack_t<4,float>( _mm_mul_ps(v.m, inv) );
  }

  // dot double2
  template<> SIMD_FORCEINLINE double dot<2,double>(const simd_pack_t<2,double>& a,
                                                   const simd_pack_t<2,double>& b) {
    __m128d p = _mm_mul_pd(a.m, b.m);
    __m128d sh = _mm_unpackhi_pd(p, p);
    __m128d s = _mm_add_sd(p, sh);
    return _mm_cvtsd_f64(s);
  }

  // length double2
  template<> SIMD_FORCEINLINE double length<2,double>(const simd_pack_t<2,double>& v) {
    double s = dot(v,v);
    __m128d r = _mm_sqrt_sd(_mm_setzero_pd(), _mm_set_sd(s));
    return _mm_cvtsd_f64(r);
  }

  // normalize double2
  template<> SIMD_FORCEINLINE simd_pack_t<2,double> normalize<2,double>(const simd_pack_t<2,double>& v) {
    double s = dot(v,v);
    __m128d inv = _mm_div_sd(_mm_set_sd(1.0), _mm_sqrt_sd(_mm_setzero_pd(), _mm_set_sd(s)));
    return simd_pack_t<2,double>( _mm_mul_pd(v.m, _mm_unpacklo_pd(inv, inv)) );
  }
#endif // SSE

// ----------------------- AVX -----------------------
#if defined(__AVX__) || defined(_M_AVX)

  // dot double4
  template<> SIMD_FORCEINLINE double dot<4,double>(const simd_pack_t<4,double>& a,
                                                   const simd_pack_t<4,double>& b) {
    __m256d p = _mm256_mul_pd(a.m, b.m);
    __m128d hi = _mm256_extractf128_pd(p, 1);
    __m128d lo = _mm256_castpd256_pd128(p);
    __m128d s1 = _mm_add_pd(hi, lo);           // two partial sums
    __m128d sh = _mm_unpackhi_pd(s1, s1);
    __m128d s2 = _mm_add_sd(s1, sh);
    return _mm_cvtsd_f64(s2);
  }

  template<> SIMD_FORCEINLINE double length<4,double>(const simd_pack_t<4,double>& v) {
    double s = dot(v,v);
    __m128d r = _mm_sqrt_sd(_mm_setzero_pd(), _mm_set_sd(s));
    return _mm_cvtsd_f64(r);
  }

  template<> SIMD_FORCEINLINE simd_pack_t<4,double> normalize<4,double>(const simd_pack_t<4,double>& v) {
    double s = dot(v,v);
    __m128d inv1 = _mm_div_sd(_mm_set_sd(1.0), _mm_sqrt_sd(_mm_setzero_pd(), _mm_set_sd(s)));
    __m256d inv = _mm256_broadcast_sd(&s); // reuse s to build vector then invert via division
    // inv_len as 1/sqrt(s) replicated to 256-bit
    double inv_scalar = 1.0 / scalar_sqrt(s);
    __m256d invv = _mm256_set1_pd(inv_scalar);
    return simd_pack_t<4,double>( _mm256_mul_pd(v.m, invv) );
  }
#endif // AVX

} // namespace simd


namespace simd
{


// ======================================
// 8) GEOMETRÍA EXTRA
//  - cross(3D/4D), normalize_safe, project, orthonormalize3
// ======================================
template<typename T>
SIMD_FORCEINLINE T dot3(const simd_pack_t<3,T>& a, const simd_pack_t<3,T>& b){
  return a.x*b.x + a.y*b.y + a.z*b.z;
}
template<typename T>
SIMD_FORCEINLINE T length3(const simd_pack_t<3,T>& a){
  return simd::sqrt<T>( dot3(a,a) ); // usa tu sqrt<T>
}

// cross 3D
template<typename T>
SIMD_FORCEINLINE simd_pack_t<3,T>
cross(const simd_pack_t<3,T>& a, const simd_pack_t<3,T>& b){
  return simd_pack_t<3,T>( a.y*b.z - a.z*b.y,
                           a.z*b.x - a.x*b.z,
                           a.x*b.y - a.y*b.x );
}
// cross 4D => xyz cruzado, w=0
template<typename T>
SIMD_FORCEINLINE simd_pack_t<4,T>
cross(const simd_pack_t<4,T>& a, const simd_pack_t<4,T>& b){
  simd_pack_t<4,T> r(T{});
  r.x = a.y*b.z - a.z*b.y;
  r.y = a.z*b.x - a.x*b.z;
  r.z = a.x*b.y - a.y*b.x;
  r.w = T(0);
  return r;
}

template<typename T>
SIMD_FORCEINLINE simd_pack_t<3,T>
normalize_safe(const simd_pack_t<3,T>& v, T eps = T(1e-6)){
  T len = length3(v);
  if (len <= eps) return simd_pack_t<3,T>(T(0));
  T inv = T(1)/len;
  return simd_pack_t<3,T>( v.x*inv, v.y*inv, v.z*inv );
}

template<typename T>
SIMD_FORCEINLINE simd_pack_t<3,T>
project(const simd_pack_t<3,T>& a, const simd_pack_t<3,T>& b){
  T bb = dot3(b,b); if (bb==T(0)) return simd_pack_t<3,T>(T(0));
  T s = dot3(a,b) / bb;
  return simd_pack_t<3,T>( b.x*s, b.y*s, b.z*s );
}

template<typename T>
SIMD_FORCEINLINE void
orthonormalize3(simd_pack_t<3,T>& u, simd_pack_t<3,T>& v, simd_pack_t<3,T>& w, T eps=T(1e-6)){
  u = normalize_safe(u, eps);
  { T d = dot3(v,u); v.x -= d*u.x; v.y -= d*u.y; v.z -= d*u.z; v = normalize_safe(v, eps); }
  { T d1= dot3(w,u), d2= dot3(w,v);
    w.x -= d1*u.x + d2*v.x; w.y -= d1*u.y + d2*v.y; w.z -= d1*u.z + d2*v.z;
    w = normalize_safe(w, eps);
  }
}	
}

namespace simd
{


// ---------- Geometría fast-path (cross float3 via SSE/NEON) ----------
#if defined(__SSE__) || defined(__SSE2__) || defined(_M_X64)
// cross 3D usando shuffles sobre float4 (se ignora w)
template<> inline simd::simd_pack_t<4,float>
simd::cross<float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 a_yzx = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(3,0,2,1));
  __m128 b_yzx = _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(3,0,2,1));
  __m128 c     = _mm_sub_ps(_mm_mul_ps(a.m, b_yzx), _mm_mul_ps(a_yzx, b.m));
  __m128 r     = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3,0,2,1));
  r = _mm_blend_ps(r, _mm_set_ss(0.0f), 0x8); // w=0
  return simd_pack_t<4,float>(r);
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
template<> inline simd::simd_pack_t<4,float>
simd::cross<float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  float32x4_t a_yzx = vextq_f32(a.m, a.m, 1);
  float32x4_t b_yzx = vextq_f32(b.m, b.m, 1);
  float32x4_t c     = vsubq_f32(vmulq_f32(a.m, b_yzx), vmulq_f32(a_yzx, b.m));
  float32x4_t r     = vextq_f32(c, c, 1);
  r = vsetq_lane_f32(0.0f, r, 3);
  return simd_pack_t<4,float>(r);
}
#endif


// ================== 8) Geometría extra ==================
template<typename T> SIMD_FORCEINLINE T dot3(const simd_pack_t<3,T>& a, const simd_pack_t<3,T>& b){
  return a.x*b.x + a.y*b.y + a.z*b.z;
}
template<typename T> SIMD_FORCEINLINE T length3(const simd_pack_t<3,T>& a){
  return simd::sqrt<T>( dot3(a,a) ); // usa tu sqrt<T>
}

// cross 3D
template<typename T>
SIMD_FORCEINLINE simd_pack_t<3,T> cross(const simd_pack_t<3,T>& a, const simd_pack_t<3,T>& b){
  return simd_pack_t<3,T>( a.y*b.z - a.z*b.y,
                           a.z*b.x - a.x*b.z,
                           a.x*b.y - a.y*b.x );
}
// cross 4D (w=0)
template<typename T>
SIMD_FORCEINLINE simd_pack_t<4,T> cross(const simd_pack_t<4,T>& a, const simd_pack_t<4,T>& b){
  simd_pack_t<4,T> r(T{});
  r.x = a.y*b.z - a.z*b.y;
  r.y = a.z*b.x - a.x*b.z;
  r.z = a.x*b.y - a.y*b.x;
  r.w = T(0);
  return r;
}

template<typename T>
SIMD_FORCEINLINE simd_pack_t<3,T> normalize_safe(const simd_pack_t<3,T>& v, T eps=T(1e-6)){
  T len = length3(v);
  if (len<=eps) return simd_pack_t<3,T>(T(0));
  T inv=T(1)/len; return simd_pack_t<3,T>(v.x*inv, v.y*inv, v.z*inv);
}
template<typename T>
SIMD_FORCEINLINE simd_pack_t<3,T> project(const simd_pack_t<3,T>& a, const simd_pack_t<3,T>& b){
  T bb = dot3(b,b); if (bb==T(0)) return simd_pack_t<3,T>(T(0));
  T s = dot3(a,b)/bb; return simd_pack_t<3,T>(b.x*s,b.y*s,b.z*s);
}
template<typename T>
SIMD_FORCEINLINE void orthonormalize3(simd_pack_t<3,T>& u, simd_pack_t<3,T>& v, simd_pack_t<3,T>& w, T eps=T(1e-6)){
  u = normalize_safe(u, eps);
  { T d = dot3(v,u); v.x-=d*u.x; v.y-=d*u.y; v.z-=d*u.z; v=normalize_safe(v,eps); }
  { T du=dot3(w,u), dv=dot3(w,v);
    w.x-=du*u.x+dv*v.x; w.y-=du*u.y+dv*v.y; w.z-=du*u.z+dv*v.z; w=normalize_safe(w,eps);
  }
}





// ------------- Geometría fast-path (cross float4) -------------
#if defined(__SSE__) || defined(__SSE2__) || defined(_M_X64)
template<> inline simd::simd_pack_t<4,float>
simd::cross<float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  __m128 a_yzx = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(3,0,2,1));
  __m128 b_yzx = _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(3,0,2,1));
  __m128 c = _mm_sub_ps(_mm_mul_ps(a.m, b_yzx), _mm_mul_ps(a_yzx, b.m));
  __m128 r = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3,0,2,1));
  r = _mm_blend_ps(r, _mm_set_ss(0.0f), 0x8); // w=0
  return simd_pack_t<4,float>(r);
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
template<> inline simd::simd_pack_t<4,float>
simd::cross<float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,float>& b){
  float32x4_t a_yzx = vextq_f32(a.m, a.m, 1);
  float32x4_t b_yzx = vextq_f32(b.m, b.m, 1);
  float32x4_t c     = vsubq_f32(vmulq_f32(a.m, b_yzx), vmulq_f32(a_yzx, b.m));
  float32x4_t r     = vextq_f32(c, c, 1);
  r = vsetq_lane_f32(0.0f, r, 3);
  return simd_pack_t<4,float>(r);
}
#endif


}