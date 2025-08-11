#pragma once
#include <simd/simd_types.h>


namespace simd {

	// ============================================================
	//  Declaraciones genéricas (plantillas base, escalar)
	// ============================================================
	template <typename T> SIMD_FORCEINLINE T sqrt(const T& v);
	template <typename T> SIMD_FORCEINLINE T rsq(const T& v); // 1/sqrt(v)

	// ---------------- Base escalar (fallback) -------------------
	template <typename T>
	SIMD_FORCEINLINE T sqrt(const T& v) {
		return static_cast<T>(std::sqrt(static_cast<double>(v)));
	}
	template <>
	SIMD_FORCEINLINE float sqrt<float>(const float& v) { return std::sqrt(v); }
	template <>
	SIMD_FORCEINLINE double sqrt<double>(const double& v) { return std::sqrt(v); }

	template <typename T>
	SIMD_FORCEINLINE T rsq(const T& v) {
		return static_cast<T>(1.0 / std::sqrt(static_cast<double>(v)));
	}
	template <>
	SIMD_FORCEINLINE float rsq<float>(const float& v) { return 1.0f / std::sqrt(v); }
	template <>
	SIMD_FORCEINLINE double rsq<double>(const double& v) { return 1.0 / std::sqrt(v); }


	// ============================================================
	//  Helpers por plataforma (sin ifdefs dentro de funciones)
	// ============================================================
	// ---- NEON ARMv7: refinamientos NR para rsqrt/rcp y sqrt via rsqrt
#if (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(__aarch64__)
	SIMD_FORCEINLINE float32x4_t neon_rsqrt_nr_f32x4(float32x4_t a) {
		float32x4_t x = vrsqrteq_f32(a);
		x = vmulq_f32(x, vrsqrtsq_f32(vmulq_f32(a, x), x));
		x = vmulq_f32(x, vrsqrtsq_f32(vmulq_f32(a, x), x));
		return x;
	}
	SIMD_FORCEINLINE float32x2_t neon_rsqrt_nr_f32x2(float32x2_t a) {
		float32x2_t x = vrsqrte_f32(a);
		x = vmul_f32(x, vrsqrts_f32(vmul_f32(a, x), x));
		x = vmul_f32(x, vrsqrts_f32(vmul_f32(a, x), x));
		return x;
	}
	SIMD_FORCEINLINE float32x4_t neon_sqrt_via_rsqrt_f32x4(float32x4_t a) {
		return vmulq_f32(a, neon_rsqrt_nr_f32x4(a));
	}
	SIMD_FORCEINLINE float32x2_t neon_sqrt_via_rsqrt_f32x2(float32x2_t a) {
		return vmul_f32(a, neon_rsqrt_nr_f32x2(a));
	}
#endif

	// ---- SSE: refinamientos NR para rsqrt/rcp (float)
#if defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
	SIMD_FORCEINLINE __m128 sse_rsqrt_nr_ps(__m128 a) {
		__m128 x = _mm_rsqrt_ps(a);
		const __m128 half = _mm_set1_ps(0.5f);
		const __m128 three = _mm_set1_ps(3.0f);
		__m128 axx = _mm_mul_ps(a, _mm_mul_ps(x, x));
		x = _mm_mul_ps(_mm_mul_ps(half, x), _mm_sub_ps(three, axx));
		axx = _mm_mul_ps(a, _mm_mul_ps(x, x));
		x = _mm_mul_ps(_mm_mul_ps(half, x), _mm_sub_ps(three, axx));
		return x;
	}
#endif

	// ============================================================
	//  Especializaciones SIMD — FLOAT4 (simd_pack_t<4,float>)
	// ============================================================
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
#elif defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	sqrt(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(_mm_sqrt_ps(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rsq(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(sse_rsqrt_nr_ps(v.m)); }
#else
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	sqrt(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z), std::sqrt(v.w)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rsq(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(1.f / std::sqrt(v.x), 1.f / std::sqrt(v.y), 1.f / std::sqrt(v.z), 1.f / std::sqrt(v.w)); }
#endif

	// ============================================================
	//  Especializaciones SIMD — FLOAT2 (simd_pack_t<2,float>)
	// ============================================================
#if defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	sqrt(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(vsqrt_f32(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rsq(const simd_pack_t<2, float>& v) {
		float32x2_t x = vrsqrte_f32(v.m);
		x = vmul_f32(x, vrsqrts_f32(vmul_f32(v.m, x), x));
		x = vmul_f32(x, vrsqrts_f32(vmul_f32(v.m, x), x));
		return simd_pack_t<2, float>(x);
	}
#elif (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	sqrt(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(neon_sqrt_via_rsqrt_f32x2(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rsq(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(neon_rsqrt_nr_f32x2(v.m)); }
#else
	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	sqrt(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(std::sqrt(v.x), std::sqrt(v.y)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rsq(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(1.f / std::sqrt(v.x), 1.f / std::sqrt(v.y)); }
#endif

	// ============================================================
	//  Especializaciones SIMD — DOUBLE2 (simd_pack_t<2,double>)
	// ============================================================
#if defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	sqrt(const simd_pack_t<2, double>& v) { return simd_pack_t<2, double>(vsqrtq_f64(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rsq(const simd_pack_t<2, double>& v) {
		float64x2_t s = vsqrtq_f64(v.m);
		float64x2_t one = vdupq_n_f64(1.0);
		return simd_pack_t<2, double>(vdivq_f64(one, s));
	}
#elif defined(__SSE2__) || defined(_M_X64)
	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	sqrt(const simd_pack_t<2, double>& v) { return simd_pack_t<2, double>(_mm_sqrt_pd(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rsq(const simd_pack_t<2, double>& v) {
		__m128d s = _mm_sqrt_pd(v.m);
		__m128d one = _mm_set1_pd(1.0);
		return simd_pack_t<2, double>(_mm_div_pd(one, s));
	}
#else
	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	sqrt(const simd_pack_t<2, double>& v) { return simd_pack_t<2, double>(std::sqrt(v.x), std::sqrt(v.y)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rsq(const simd_pack_t<2, double>& v) { return simd_pack_t<2, double>(1.0 / std::sqrt(v.x), 1.0 / std::sqrt(v.y)); }
#endif

	// ============================================================
	//  Especializaciones SIMD — DOUBLE4 (simd_pack_t<4,double>)
	// ============================================================
#if defined(__AVX__) || defined(_M_AVX)
	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	sqrt(const simd_pack_t<4, double>& v) { return simd_pack_t<4, double>(_mm256_sqrt_pd(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	rsq(const simd_pack_t<4, double>& v) {
		__m256d s = _mm256_sqrt_pd(v.m);
		__m256d one = _mm256_set1_pd(1.0);
		return simd_pack_t<4, double>(_mm256_div_pd(one, s));
	}
#else
	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	sqrt(const simd_pack_t<4, double>& v) { return simd_pack_t<4, double>(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z), std::sqrt(v.w)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	rsq(const simd_pack_t<4, double>& v) { return simd_pack_t<4, double>(1.0 / std::sqrt(v.x), 1.0 / std::sqrt(v.y), 1.0 / std::sqrt(v.z), 1.0 / std::sqrt(v.w)); }
#endif

} // namespace simd


namespace simd {


  // ============================
  // Núcleos polinomiales
  // ============================

  // --- exp2(f) en f ∈ [0,1) — rápido (grado 4-5)
  template<typename T>
  SIMD_FORCEINLINE T _poly_exp2_fast(T f){
    // Aproximación típica: 2^f ≈ 1 + f*ln2 + f^2*(ln2^2/2) + ...
    // Pero afinada con coeficientes “prácticos” para error bajo:
    // p(f) = 1 + c1 f + c2 f^2 + c3 f^3 + c4 f^4 (+ c5 f^5)
    const T c1 = T(0.69314718056);
    const T c2 = T(0.24022650696);
    const T c3 = T(0.05550410866);
    const T c4 = T(0.00961812911);
    const T c5 = T(0.00133335581);
    return (( ( (c5*f + c4)*f + c3)*f + c2)*f + c1)*f + T(1);
  }

  // --- exp2(f) más precisa en f ∈ [-0.5, 0.5] — minimax corto (grado 6)
  template<typename T>
  SIMD_FORCEINLINE T _poly_exp2_precise(T f){
    // Simétrica alrededor de 0 para mejor estabilidad
    const T c1 = T(0.6931471805599453);
    const T c2 = T(0.2402265069591007);
    const T c3 = T(0.0555041086648216);
    const T c4 = T(0.0096181291076285);
    const T c5 = T(0.0013333558146428);
    const T c6 = T(0.0001540341121616);
    T f2 = f*f;
    return T(1) + f*(c1 + f*(c2 + f*(c3 + f*(c4 + f*(c5 + f*c6)))));
  }

  // --- log2(m) en m ∈ [1,2) — rápido (grado 3-4)
  template<typename T>
  SIMD_FORCEINLINE T _poly_log2_fast(T m){
    // m = 1 + y, y∈[0,1)
    T y  = m - T(1);
    // Minimax breve para log2(1+y)
    const T c1 = T(1.4426950408889634);   // 1/ln(2)
    const T c2 = T(-0.7213475204444817);
    const T c3 = T(0.4808983469629878);
    const T c4 = T(-0.3606737602222408);
    return ((c4*y + c3)*y + c2)*y + c1;   // c1*y + c2*y^2 + ...
  }

  // --- log2(m) más precisa en m ∈ [√½,√2] — grado 6 impar sobre r=(m-1)/(m+1)
  template<typename T>
  SIMD_FORCEINLINE T _poly_log2_precise(T m){
    // Transformación de Halley: r = (m-1)/(m+1), log(m) ≈ 2[r + r^3/3 + r^5/5 + ...]
    T r  = (m - T(1)) / (m + T(1));
    T r2 = r*r, r3 = r*r2, r5 = r3*r2, r7 = r5*r2;
    T ln = T(2)*( r + r3/T(3) + r5/T(5) + r7/T(7) );
    return ln * _cst_<T>::inv_ln2;
  }

  // ============================
  // Reconstrucción 2^k vía bits
  // ============================
  SIMD_FORCEINLINE float  _exp2i_bits(int k){
    // Saturar a rango representable para evitar UB/INF prematuro
    k = clampi(k + 127, 0, 255);
    uint32_t bits = uint32_t(k) << 23;
    return _bit_cast<float>(bits);
  }
  SIMD_FORCEINLINE double _exp2i_bits_d(int k){
    k = clampi(k + 1023, 0, 2047);
    uint64_t bits = (uint64_t(k) << 52);
    return _bit_cast<double>(bits);
  }

  // ============================
  // FAST: exp2 / exp
  // ============================
  template<typename T> SIMD_FORCEINLINE T fast_exp2(const T& x);
  template<> SIMD_FORCEINLINE float  fast_exp2<float >(const float&  x){
    // x = k + f, f∈[0,1)
    int   k = (int)x; if (x < 0 && float(k) != x) --k; // floor
    float f = x - (float)k;
    float a = _poly_exp2_fast(f);
    return a * _exp2i_bits(k);
  }
  template<> SIMD_FORCEINLINE double fast_exp2<double>(const double& x){
    int    k = (int)x; if (x < 0 && double(k) != x) --k;
    double f = x - (double)k;
    double a = _poly_exp2_fast(f);
    return a * _exp2i_bits_d(k);
  }

  template<typename T> SIMD_FORCEINLINE T fast_exp(const T& x){
    // e^x = 2^(x * log2(e))
    return fast_exp2<T>( x * _cst_<T>::inv_ln2 );
  }

  // ============================
  // FAST_PRECISE: exp2 / exp
  // ============================
  template<typename T> SIMD_FORCEINLINE T fast_precise_exp2(const T& x);
  template<> SIMD_FORCEINLINE float  fast_precise_exp2<float >(const float&  x){
    // Reducción a f∈[-0.5,0.5] alrededor de la potencia entera más cercana
    float  kf = x >= 0 ? float(int(x+0.5f)) : float(int(x-0.5f));
    int    k  = (int)kf;
    float  f  = x - kf;
    float  a  = _poly_exp2_precise(f);
    return a * _exp2i_bits(k);
  }
  template<> SIMD_FORCEINLINE double fast_precise_exp2<double>(const double& x){
    double kf = x >= 0 ? double((long long)(x+0.5)) : double((long long)(x-0.5));
    int    k  = (int)kf;
    double f  = x - kf;
    double a  = _poly_exp2_precise(f);
    return a * _exp2i_bits_d(k);
  }

  template<typename T> SIMD_FORCEINLINE T fast_precise_exp(const T& x){
    // exp(x) = 2^(x/ln2) con reducción Cody–Waite usando ln2_hi + ln2_lo si quisiéramos hacerlo en base e
    return fast_precise_exp2<T>( x * _cst_<T>::inv_ln2 );
  }

  // ============================
  // FAST: log2 / log
  // ============================
  template<typename T> SIMD_FORCEINLINE T fast_log2(const T& x);
  template<> SIMD_FORCEINLINE float  fast_log2<float >(const float&  x){
    // Descompone x = m * 2^e, con m∈[1,2)
    uint32_t u = _bit_cast<uint32_t>(x);
    int  e = int((u >> 23) & 0xFFu) - 127;
    u = (u & 0x007FFFFFu) | (127u << 23);   // fuerza m en [1,2)
    float m = _bit_cast<float>(u);
    return float(e) + _poly_log2_fast(m);
  }
  template<> SIMD_FORCEINLINE double fast_log2<double>(const double& x){
    uint64_t u = _bit_cast<uint64_t>(x);
    int  e = int((u >> 52) & 0x7FFull) - 1023;
    u = (u & 0x000FFFFFFFFFFFFFull) | (uint64_t(1023) << 52);
    double m = _bit_cast<double>(u);
    return double(e) + _poly_log2_fast(m);
  }

  template<typename T> SIMD_FORCEINLINE T fast_log(const T& x){
    // ln(x) = log2(x) * ln(2)
    return fast_log2<T>(x) * _cst_<T>::ln2;
  }

  // ============================
  // FAST_PRECISE: log2 / log
  // ============================
  template<typename T> SIMD_FORCEINLINE T fast_precise_log2(const T& x);
  template<> SIMD_FORCEINLINE float  fast_precise_log2<float >(const float&  x){
    // Empuja m a ~[√½,√2] para mejorar polinomio
    uint32_t u = _bit_cast<uint32_t>(x);
    int  e = int((u >> 23) & 0xFFu) - 127;
    u = (u & 0x007FFFFFu) | (127u << 23);
    float m = _bit_cast<float>(u);

    // Si m < √2/2, duplica m y ajusta e--
    const float sqrt2_half = 0.70710678118654752440f; // ≈ √(1/2)
    if (m < (1.0f / 1.41421356237f)) { m *= 2.0f; --e; }
    return float(e) + _poly_log2_precise(m);
  }
  template<> SIMD_FORCEINLINE double fast_precise_log2<double>(const double& x){
    uint64_t u = _bit_cast<uint64_t>(x);
    int  e = int((u >> 52) & 0x7FFull) - 1023;
    u = (u & 0x000FFFFFFFFFFFFFull) | (uint64_t(1023) << 52);
    double m = _bit_cast<double>(u);

    if (m < (1.0 / 1.4142135623730951)) { m *= 2.0; --e; }
    return double(e) + _poly_log2_precise(m);
  }

  template<typename T> SIMD_FORCEINLINE T fast_precise_log(const T& x){
    return fast_precise_log2<T>(x) * _cst_<T>::ln2;
  }

  // ============================
  // Versiones SIMD (por-lane)
  // ============================
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fast_exp2(const simd_pack_t<D,T>& a){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = fast_exp2<T>((&a.x)[i]);
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fast_exp(const simd_pack_t<D,T>& a){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = fast_exp<T>((&a.x)[i]);
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fast_log2(const simd_pack_t<D,T>& a){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = fast_log2<T>((&a.x)[i]);
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fast_log(const simd_pack_t<D,T>& a){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = fast_log<T>((&a.x)[i]);
    return r;
  }

  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_exp2(const simd_pack_t<D,T>& a){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = fast_precise_exp2<T>((&a.x)[i]);
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_exp(const simd_pack_t<D,T>& a){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = fast_precise_exp<T>((&a.x)[i]);
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_log2(const simd_pack_t<D,T>& a){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = fast_precise_log2<T>((&a.x)[i]);
    return r;
  }
  template<int D, typename T>
  SIMD_FORCEINLINE simd_pack_t<D,T> fast_precise_log(const simd_pack_t<D,T>& a){
    simd_pack_t<D,T> r(T{});
    for (int i=0;i<D;++i) (&r.x)[i] = fast_precise_log<T>((&a.x)[i]);
    return r;
  }

} // namespace simd


namespace simd
{


// ======================================
// 7) ACTIVACIONES (estables y rápidas)
//  - sigmoid_fast / sigmoid_precise
//  - softplus_fast / softplus_precise
//  - relu
// Requiere fast_exp / fast_precise_exp / fast_log / fast_precise_log
// ======================================
template<typename T>
SIMD_FORCEINLINE T sigmoid_fast(const T& x){
  // versión estable simple
  if (x >= T(0)) {
    T e = fast_exp<T>(-x);
    return T(1) / (T(1) + e);
  } else {
    T e = fast_exp<T>(x);
    return e / (T(1) + e);
  }
}
template<typename T>
SIMD_FORCEINLINE T sigmoid_precise(const T& x){
  if (x >= T(0)) {
    T e = fast_precise_exp<T>(-x);
    return T(1) / (T(1) + e);
  } else {
    T e = fast_precise_exp<T>(x);
    return e / (T(1) + e);
  }
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> sigmoid_fast(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=sigmoid_fast<T>((&v.x)[i]); return r;
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> sigmoid_precise(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=sigmoid_precise<T>((&v.x)[i]); return r;
}

template<typename T> SIMD_FORCEINLINE T relu(const T& x){ return x<T(0)?T(0):x; }
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> relu(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i){ T x=(&v.x)[i]; (&r.x)[i]=x<T(0)?T(0):x; } return r;
}

template<typename T>
SIMD_FORCEINLINE T softplus_fast(const T& x){
  // log(1+exp(x)) estable: umbrales típicos ~20
  const T hi = T(20), lo = T(-20);
  if (x > hi) return x;
  if (x < lo) return fast_exp<T>(x);
  return fast_log<T>(T(1) + fast_exp<T>(x));
}
template<typename T>
SIMD_FORCEINLINE T softplus_precise(const T& x){
  const T hi = T(20), lo = T(-20);
  if (x > hi) return x;
  if (x < lo) return fast_precise_exp<T>(x);
  return fast_precise_log<T>(T(1) + fast_precise_exp<T>(x));
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> softplus_fast(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=softplus_fast<T>((&v.x)[i]); return r;
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> softplus_precise(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=softplus_precise<T>((&v.x)[i]); return r;
}





// ================== 7) Activaciones (float/double) ==================
// fast_log1p / fast_expm1 (usadas en softplus estable)
template<typename T> SIMD_FORCEINLINE T fast_expm1(const T& x){
  // aproximación estable: para |x| pequeño usa serie, si no usa fast_exp
  const T th = T(1e-3);
  if (x >  th) return fast_exp<T>(x) - T(1);
  if (x < -th) return -(fast_exp<T>(-x) - T(1));
  // serie: x + x^2/2 + x^3/6
  T x2 = x*x; return x + x2*T(0.5) + x2*x*T(1.0/6.0);
}
template<typename T> SIMD_FORCEINLINE T fast_log1p(const T& x){
  // |x| pequeño: log(1+x) ≈ x - x^2/2 + x^3/3
  const T th = T(1e-3);
  if (x >  th || x < -th) return fast_log<T>(T(1)+x);
  T x2 = x*x; return x - x2*T(0.5) + x2*x*T(1.0/3.0);
}

// Sigmoid estable (fast / precise)
template<typename T> SIMD_FORCEINLINE T sigmoid_fast(const T& x){
  if (x>=T(0)){ T e=fast_exp<T>(-x); return T(1)/(T(1)+e); }
  else        { T e=fast_exp<T>( x); return        e /(T(1)+e); }
}
template<typename T> SIMD_FORCEINLINE T sigmoid_precise(const T& x){
  if (x>=T(0)){ T e=fast_precise_exp<T>(-x); return T(1)/(T(1)+e); }
  else        { T e=fast_precise_exp<T>( x); return        e /(T(1)+e); }
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> sigmoid_fast(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=sigmoid_fast<T>((&v.x)[i]); return r;
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> sigmoid_precise(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=sigmoid_precise<T>((&v.x)[i]); return r;
}

// ReLU
template<typename T> SIMD_FORCEINLINE T relu(const T& x){ return x<T(0)?T(0):x; }
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> relu(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i){ T x=(&v.x)[i]; (&r.x)[i]=x<T(0)?T(0):x; } return r;
}

// Softplus estable: log(1+exp(x)) con clamps y log1p/expm1 rápidos
template<typename T> SIMD_FORCEINLINE T softplus_fast(const T& x){
  const T hi=T(20), lo=T(-20);
  if (x>hi) return x;
  if (x<lo) return fast_expm1<T>(x); // ~ e^x para x << 0
  return fast_log1p<T>( fast_exp<T>(x) );
}
template<typename T> SIMD_FORCEINLINE T softplus_precise(const T& x){
  const T hi=T(20), lo=T(-20);
  if (x>hi) return x;
  if (x<lo) return fast_expm1<T>(x);
  return fast_precise_log<T>( T(1) + fast_precise_exp<T>(x) );
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> softplus_fast(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=softplus_fast<T>((&v.x)[i]); return r;
}
template<int D, typename T>
SIMD_FORCEINLINE simd_pack_t<D,T> softplus_precise(const simd_pack_t<D,T>& v){
  simd_pack_t<D,T> r(T{}); for(int i=0;i<D;++i) (&r.x)[i]=softplus_precise<T>((&v.x)[i]); return r;
}
}