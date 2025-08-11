// simd_trig_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

  // ----------------------------------------------------------
  // Escalares (float / double)
  // ----------------------------------------------------------
  template<typename T> SIMD_FORCEINLINE T radians(const T& degrees) { return degrees * (T(_pi_<T>::v) / T(180)); }
  template<typename T> SIMD_FORCEINLINE T degrees(const T& radians) { return radians * (T(180) / T(_pi_<T>::v)); }

  template<typename T> SIMD_FORCEINLINE T  cos (const T& a) { using std::cos;  return cos(a); }
  template<typename T> SIMD_FORCEINLINE T  sin (const T& a) { using std::sin;  return sin(a); }
  template<typename T> SIMD_FORCEINLINE T  tan (const T& a) { using std::tan;  return tan(a); }

  template<typename T> SIMD_FORCEINLINE T  cosh(const T& a) { using std::cosh; return cosh(a); }
  template<typename T> SIMD_FORCEINLINE T  sinh(const T& a) { using std::sinh; return sinh(a); }
  template<typename T> SIMD_FORCEINLINE T  tanh(const T& a) { using std::tanh; return tanh(a); }

  template<typename T> SIMD_FORCEINLINE T  acos(const T& a) { using std::acos; return acos(a); }
  template<typename T> SIMD_FORCEINLINE T  asin(const T& a) { using std::asin; return asin(a); }
  template<typename T> SIMD_FORCEINLINE T  atan(const T& a) { using std::atan; return atan(a); }
  template<typename T> SIMD_FORCEINLINE T  atan2(const T& y, const T& x) { using std::atan2; return atan2(y, x); }

  template<typename T> SIMD_FORCEINLINE T  acosh(const T& a) { using std::acosh; return acosh(a); }
  template<typename T> SIMD_FORCEINLINE T  asinh(const T& a) { using std::asinh; return asinh(a); }
  template<typename T> SIMD_FORCEINLINE T  atanh(const T& a) { using std::atanh; return atanh(a); }
}


namespace simd {

  // wrap [-π, π)
  template<typename T>
  SIMD_FORCEINLINE T wrap_angle(T a) {
    using std::fmod;
    T two_pi = T(2) * T(_pi_<T>::v);
    a = fmod(a + T(_pi_<T>::v), two_pi);
    if (a < T(0)) a += two_pi;
    return a - T(_pi_<T>::v);
  }

  // ----------------------------------------------------------
  // Packs genéricos (por-lane)
  // ----------------------------------------------------------
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> radians(const simd_pack_t<D,T>& deg) {
    simd_pack_t<D,T> r(T{});
    const T k = T(_pi_<T>::v) / T(180);
    for (int i=0;i<D;++i) (&r.x)[i] = (&deg.x)[i] * k;
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> degrees(const simd_pack_t<D,T>& rad) {
    simd_pack_t<D,T> r(T{});
    const T k = T(180) / T(_pi_<T>::v);
    for (int i=0;i<D;++i) (&r.x)[i] = (&rad.x)[i] * k;
    return r;
  }

  // trig / hyperbolic / inverse (element-wise)
  #define SIMD_TRIG_UNARY(name) \
    template<int D, typename T> \
    SIMD_FORCEINLINE simd_pack_t<D,T> name(const simd_pack_t<D,T>& a) { \
      simd_pack_t<D,T> r(T{}); \
      using std::name; \
      for (int i=0;i<D;++i) (&r.x)[i] = name((&a.x)[i]); \
      return r; \
    }

  SIMD_TRIG_UNARY(cos)
  SIMD_TRIG_UNARY(sin)
  SIMD_TRIG_UNARY(tan)
  SIMD_TRIG_UNARY(cosh)
  SIMD_TRIG_UNARY(sinh)
  SIMD_TRIG_UNARY(tanh)
  SIMD_TRIG_UNARY(acos)
  SIMD_TRIG_UNARY(asin)
  SIMD_TRIG_UNARY(atan)
  SIMD_TRIG_UNARY(acosh)
  SIMD_TRIG_UNARY(asinh)
  SIMD_TRIG_UNARY(atanh)

  #undef SIMD_TRIG_UNARY

  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> atan2(const simd_pack_t<D,T>& y, const simd_pack_t<D,T>& x) {
    simd_pack_t<D,T> r(T{});
    using std::atan2;
    for (int i=0;i<D;++i) (&r.x)[i] = atan2((&y.x)[i], (&x.x)[i]);
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> atan2(const simd_pack_t<D,T>& y, T x) {
    return atan2(y, splat<D,T>(x));
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> atan2(T y, const simd_pack_t<D,T>& x) {
    return atan2(splat<D,T>(y), x);
  }

  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> wrap_angle(const simd_pack_t<D,T>& a) {
    simd_pack_t<D,T> r(T{});
    using std::fmod;
    const T two_pi = T(2) * T(_pi_<T>::v);
    for (int i=0;i<D;++i) {
      T v = (&a.x)[i];
      v = fmod(v + T(_pi_<T>::v), two_pi);
      if (v < T(0)) v += two_pi;
      (&r.x)[i] = v - T(_pi_<T>::v);
    }
    return r;
  }

}






namespace simd {


// fmod sin <cmath> para wrap (suficiente para fast trig)
template<typename T>
SIMD_FORCEINLINE T _fast_fmod(T a, T t){
  T q = a / t;
  // round-to-zero entero:
  long long qi = (long long)q;
  // corrige hacia -inf si q negativo y tenía parte fracc.
  if (q < T(0) && T(qi) != q) --qi;
  return a - T(qi) * t;
}
template<typename T>
SIMD_FORCEINLINE T _wrap_pi(T a){
  T x = _fast_fmod(a + _pi_<T>::v, _two_pi_<T>::v);
  if (x < T(0)) x += _two_pi_<T>::v;
  return x - _pi_<T>::v;
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> _wrap_pi(const simd_pack_t<D,T>& a){
  simd_pack_t<D,T> r(T{});
  for(int i=0;i<D;++i) (&r.x)[i] = _wrap_pi<T>((&a.x)[i]);
  return r;
}

// “sqrt” escalar rápido (3 iter NR) para no depender de <cmath>
SIMD_FORCEINLINE float  _sqrt_fast(float  x){ if (x<=0.f) return 0.f;  float  g = x>1.f?x:1.f;  g=0.5f*(g+x/g); g=0.5f*(g+x/g); g=0.5f*(g+x/g); return g; }
SIMD_FORCEINLINE double _sqrt_fast(double x){ if (x<=0.0) return 0.0;  double g = x>1.0?x:1.0;  g=0.5 *(g+x/g); g=0.5 *(g+x/g); g=0.5 *(g+x/g); return g; }

// ==========================================================
// POLINOMIOS “FAST” (muy cortos)
// ==========================================================
// cos_52s clásico
template<typename T>
SIMD_FORCEINLINE T _poly_cos_52s(T x){
  T xx = x*x;
  return T(0.9999932946)
       + xx*(T(-0.4999124376)
       + xx*(T(0.0414877472)
       + xx* T(-0.0012712095)));
}
// sin7: x - x^3/6 + x^5/120 - x^7/5040
template<typename T>
SIMD_FORCEINLINE T _poly_sin7(T x){
  T x2=x*x, x3=x*x2, x5=x3*x2, x7=x5*x2;
  return x - x3*T(1.0/6.0) + x5*T(1.0/120.0) - x7*T(1.0/5040.0);
}
// asin serie corta (válida ~|x|<=0.8)
template<typename T>
SIMD_FORCEINLINE T _poly_asin9(T x){
  T x2=x*x, x3=x2*x, x5=x3*x2, x7=x5*x2, x9=x7*x2;
  return x + x3*T(0.166666667) + x5*T(0.075) + x7*T(0.0446428571) + x9*T(0.0303819444);
}
// atan impar con reducción
template<typename T>
SIMD_FORCEINLINE T _poly_atan_core(T x){
  T x2=x*x, x3=x2*x, x5=x3*x2, x7=x5*x2, x9=x7*x2, x11=x9*x2;
  return x - x3*T(0.333333333333)
           + x5*T(0.2)
           - x7*T(0.142857142857)
           + x9*T(0.111111111111)
           - x11*T(0.090909090909);
}

// ==========================================================
// POLINOMIOS “FAST_PRECISE” (minimax + Estrin) en [-π/4, π/4]
// ==========================================================
template<typename T> struct sin_coefs;
template<> struct sin_coefs<float>{
  static constexpr float c[4] = {
    -1.6666667163e-1f,   // x^3
     8.3333477378e-3f,   // x^5
    -1.9842604524e-4f,   // x^7
     2.7600126483e-6f    // x^9
  };
};
template<> struct sin_coefs<double>{
  static constexpr double c[4] = {
    -1.66666666666666324348e-1,
     8.33333333332248946124e-3,
    -1.98412698298579493134e-4,
     2.75573137070700676789e-6
  };
};
template<typename T> struct cos_coefs;
template<> struct cos_coefs<float>{
  static constexpr float c[4] = {
    -5.0000000000e-1f,   // x^2
     4.1666645683e-2f,   // x^4
    -1.3887316255e-3f,   // x^6
     2.4433157117e-5f    // x^8
  };
};
template<> struct cos_coefs<double>{
  static constexpr double c[4] = {
    -5.00000000000000000000e-1,
     4.16666666666665929218e-2,
    -1.38888888888730564116e-3,
     2.48015873015705523050e-5
  };
};

// Estrin compacto
template<typename T>
SIMD_FORCEINLINE T estrin_even4(T x2, const T* c){ // c0=1 implícito fuera
  T x4 = x2*x2;
  return c[0]*x2 + c[1]*x4 + (c[2]*x2 + c[3]*x4)*x4; // x^2 + x^4 + x^6 + x^8
}
template<typename T>
SIMD_FORCEINLINE T estrin_odd4(T x, T x2, const T* c){ // c son {c3,c5,c7,c9}
  T x3 = x*x2, x4=x2*x2, x5=x3*x2, x9=x5*x4;
  return c[0]*x3 + c[1]*x5 + (c[2]*x3 + c[3]*x5)*x4 + x; // + x al final
}

// Reducción Cody–Waite π/2 = hi + lo
template<typename T> struct _half_pi_split_;
template<> struct _half_pi_split_<float>  { static constexpr float  hi=1.5707962513f, lo=7.5497894159e-8f; };
template<> struct _half_pi_split_<double> { static constexpr double hi=1.5707963267948966, lo=6.123233995736766e-17; };

template<typename T>
SIMD_FORCEINLINE void reduce_pi_over_2(T a, int& quadrant, T& r){
  // k = round(a / (π/2))
  T inv = a / _half_pi_split_<T>::hi;
  T kf  = (inv >= T(0)) ? T((long long)(inv + T(0.5))) : T((long long)(inv - T(0.5)));
  quadrant = int((long long)kf & 3);
  r = a - kf * _half_pi_split_<T>::hi;
  r = r - kf * _half_pi_split_<T>::lo;
}

// ==========================================================
// FAMILIA “FAST” (máxima velocidad)
// ==========================================================
template<typename T> SIMD_FORCEINLINE T fast_cos(T a){
  T x = _wrap_pi(a); T ax = absT(x); bool flip=false;
  if (ax > _half_pi_<T>::v){ ax = _pi_<T>::v - ax; flip=true; }
  T c = _poly_cos_52s(ax);
  return flip ? -c : c;
}
template<typename T> SIMD_FORCEINLINE T fast_sin(T a){
  return fast_cos<T>(_half_pi_<T>::v - a);
}
template<typename T> SIMD_FORCEINLINE T fast_tan(T a){
  T c=fast_cos<T>(a), s=fast_sin<T>(a);
  T eps = (sizeof(T)==sizeof(float)) ? T(1e-6f) : T(1e-12);
  return s/((c>=T(0))?(c+eps):(c-eps));
}
template<typename T> SIMD_FORCEINLINE T fast_asin(T x){
  if (x>T(1)) x=T(1); if (x<T(-1)) x=T(-1);
  T ax=absT(x);
  if (ax<=T(0.8)) return _poly_asin9(x);
  T a = fast_acos(ax);
  return (x< T(0)) ? -(_half_pi_<T>::v - a) : (_half_pi_<T>::v - a);
}
template<typename T> SIMD_FORCEINLINE T fast_acos(T x){ return _half_pi_<T>::v - fast_asin(x); }
template<typename T> SIMD_FORCEINLINE T fast_atan(T x){
  T ax=absT(x);
  if (ax<=T(1)) return _poly_atan_core(x);
  T r = _poly_atan_core(T(1)/ax);
  r = _half_pi_<T>::v - r;
  return (x< T(0)) ? -r : r;
}
template<typename T> SIMD_FORCEINLINE T fast_atan(T y, T x){
  if (x> T(0)) return fast_atan(y/x);
  if (x< T(0)) return (y>=T(0)? fast_atan(y/x)+_pi_<T>::v
                              : fast_atan(y/x)-_pi_<T>::v);
  return (y> T(0) ? _half_pi_<T>::v : (y< T(0) ? -_half_pi_<T>::v : T(0)));
}

// Packs “FAST”
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_cos (const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_cos <T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_sin (const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_sin <T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_tan (const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_tan <T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_asin(const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_asin<T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_acos(const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_acos<T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_atan(const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_atan<T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_atan(const simd_pack_t<D,T>& y,const simd_pack_t<D,T>& x){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_atan<T>((&y.x)[i],(&x.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_atan(const simd_pack_t<D,T>& y, T x){ return fast_atan(y, splat<D,T>(x)); }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_atan(T y, const simd_pack_t<D,T>& x){ return fast_atan(splat<D,T>(y), x); }

// ==========================================================
// FAMILIA “FAST_PRECISE” (reducción CW + minimax + Estrin)
// ==========================================================
template<typename T>
SIMD_FORCEINLINE T fast_precise_cos(T a){
  int q; T x; reduce_pi_over_2<T>(a, q, x); T x2=x*x;
  T c = T(1) + estrin_even4<T>(x2, cos_coefs<T>::c);
  T s = x    + estrin_odd4 <T>(x, x2, sin_coefs<T>::c);
  switch(q){ case 0: return c; case 1: return -s; case 2: return -c; default: return s; }
}
template<typename T>
SIMD_FORCEINLINE T fast_precise_sin(T a){
  int q; T x; reduce_pi_over_2<T>(a, q, x); T x2=x*x;
  T c = T(1) + estrin_even4<T>(x2, cos_coefs<T>::c);
  T s = x    + estrin_odd4 <T>(x, x2, sin_coefs<T>::c);
  switch(q){ case 0: return s; case 1: return c; case 2: return -s; default: return -c; }
}
template<typename T>
SIMD_FORCEINLINE T fast_precise_tan(T a){
  // tan ≈ sin/cos con versiones precisas
  T c = fast_precise_cos<T>(a);
  T s = fast_precise_sin<T>(a);
  T eps = (sizeof(T)==sizeof(float)) ? T(1e-7f) : T(1e-14);
  return s/((c>=T(0))?(c+eps):(c-eps));
}

// asin/acos precisos: identidades robustas con sqrt rápida
template<typename T>
SIMD_FORCEINLINE T fast_precise_asin(T x){
  if (x>T(1)) x=T(1); if (x<T(-1)) x=T(-1);
  T one_m = T(1) - x*x;
  T s = _sqrt_fast(one_m);
  return fast_precise_atan( x, (s>T(0)? s : (sizeof(T)==sizeof(float)? T(1e-7f):T(1e-14))) ); // atan2-like
}
template<typename T>
SIMD_FORCEINLINE T fast_precise_acos(T x){
  return _half_pi_<T>::v - fast_precise_asin(x);
}

template<typename T>
SIMD_FORCEINLINE T fast_precise_atan(T x){
  // usa la misma reducción que “fast”, pero como parte de la familia precisa
  T ax=absT(x);
  if (ax<=T(1)) return _poly_atan_core(x);
  T r = _poly_atan_core(T(1)/ax);
  r = _half_pi_<T>::v - r;
  return (x< T(0)) ? -r : r;
}
template<typename T>
SIMD_FORCEINLINE T fast_precise_atan(T y, T x){
  // atan2 con cuadrantes, delegando al núcleo preciso
  if (x> T(0)) return fast_precise_atan(y/x);
  if (x< T(0)) return (y>=T(0)? fast_precise_atan(y/x)+_pi_<T>::v
                              : fast_precise_atan(y/x)-_pi_<T>::v);
  return (y> T(0) ? _half_pi_<T>::v : (y< T(0) ? -_half_pi_<T>::v : T(0)));
}

// Packs “FAST_PRECISE”
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_cos (const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_precise_cos <T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_sin (const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_precise_sin <T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_tan (const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_precise_tan <T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_asin(const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_precise_asin<T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_acos(const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_precise_acos<T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_atan(const simd_pack_t<D,T>& a){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_precise_atan<T>((&a.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_atan(const simd_pack_t<D,T>& y,const simd_pack_t<D,T>& x){ simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=fast_precise_atan<T>((&y.x)[i],(&x.x)[i]); return r; }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_atan(const simd_pack_t<D,T>& y, T x){ return fast_precise_atan(y, splat<D,T>(x)); }
template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_atan(T y, const simd_pack_t<D,T>& x){ return fast_precise_atan(splat<D,T>(y), x); }

} // namespace simd
