// simd_interp_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

	// ==========================================================
	// Helpers genéricos
	// ==========================================================

	template<typename T> SIMD_FORCEINLINE T clamp(T x, T lo, T hi) {return x < lo ? lo : (x > hi ? hi : x);}
	SIMD_FORCEINLINE int clampi(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

	template<typename T> SIMD_FORCEINLINE T saturate(T x) { return clamp<T>(x, T(0), T(1)); }

	template<typename T> SIMD_FORCEINLINE T step(T edge, T x) { return x < edge ? T(0) : T(1); }

	template<typename T> SIMD_FORCEINLINE T mix(T a, T b, T t) { return a + (b - a) * t; }
	template<typename T> SIMD_FORCEINLINE T lerp(T a, T b, T t) { return mix(a, b, t); }

	template<typename T> SIMD_FORCEINLINE T inverse_lerp(T a, T b, T x) { return (x - a) / (b - a); }

	template<typename T> SIMD_FORCEINLINE T smoothstep(T e0, T e1, T x) {T t = saturate((x - e0) / (e1 - e0));return t * t * (T(3) - T(2) * t);}
	template<typename T> SIMD_FORCEINLINE T smootherstep(T e0, T e1, T x) {T t = saturate((x - e0) / (e1 - e0));return t * t * t * (t * (T(6) * t - T(15)) + T(10));}
	template<typename T> SIMD_FORCEINLINE T remap(T x, T a0, T a1, T b0, T b1) {return mix(b0, b1, inverse_lerp(a0, a1, x));}

	// ==========================================================
	// Genéricos para simd_pack_t<D,T> (fallback por bucle)
	//  - No dependen de otros headers ni intrínsecos
	// ==========================================================
	template<int D, typename T>	SIMD_FORCEINLINE simd_pack_t<D, T> clamp(const simd_pack_t<D, T>& x, const simd_pack_t<D, T>& lo, const simd_pack_t<D, T>& hi)
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) {
			T xi = (&x.x)[i], l = (&lo.x)[i], h = (&hi.x)[i];
			(&r.x)[i] = xi < l ? l : (xi > h ? h : xi);
		}
		return r;
	}

	// overloads con límites escalares
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> clamp(const simd_pack_t<D, T>& x, T lo, T hi) {
		return clamp(x, splat<D, T>(lo), splat<D, T>(hi));
	}

	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> saturate(const simd_pack_t<D, T>& x) {
		return clamp(x, T(0), T(1));
	}

	// step: genérico por bucle (vector-edge, vector-x)
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> step(const simd_pack_t<D, T>& edge, const simd_pack_t<D, T>& x) {
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = ((&x.x)[i] < (&edge.x)[i]) ? T(0) : T(1);
		return r;
	}
	// overloads: edge escalar / x escalar
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> step(T edge, const simd_pack_t<D, T>& x) {
		return step(splat<D, T>(edge), x);
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> step(const simd_pack_t<D, T>& edge, T x) {
		return step(edge, splat<D, T>(x));
	}

	// mix / lerp (vector-vector con t vector)
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> mix(const simd_pack_t<D, T>& a,
		const simd_pack_t<D, T>& b,
		const simd_pack_t<D, T>& t)
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = (&a.x)[i] + ((&b.x)[i] - (&a.x)[i]) * (&t.x)[i];
		return r;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> lerp(const simd_pack_t<D, T>& a,
		const simd_pack_t<D, T>& b,
		const simd_pack_t<D, T>& t)
	{
		return mix(a, b, t);
	}

	// mix / lerp con 't' escalar
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> mix(const simd_pack_t<D, T>& a,
		const simd_pack_t<D, T>& b, T t)
	{
		return mix(a, b, splat<D, T>(t));
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> lerp(const simd_pack_t<D, T>& a,
		const simd_pack_t<D, T>& b, T t)
	{
		return mix(a, b, t);
	}

	// inverse_lerp / smoothstep / smootherstep / remap
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> inverse_lerp(const simd_pack_t<D, T>& a,
		const simd_pack_t<D, T>& b,
		const simd_pack_t<D, T>& x)
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = ((&x.x)[i] - (&a.x)[i]) / ((&b.x)[i] - (&a.x)[i]);
		return r;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> smoothstep(const simd_pack_t<D, T>& e0,
		const simd_pack_t<D, T>& e1,
		const simd_pack_t<D, T>& x)
	{
		simd_pack_t<D, T> t = clamp((x - e0) / (e1 - e0), T(0), T(1));
		return t * t * (splat<D, T>(T(3)) - splat<D, T>(T(2)) * t);
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> smootherstep(const simd_pack_t<D, T>& e0,
		const simd_pack_t<D, T>& e1,
		const simd_pack_t<D, T>& x)
	{
		simd_pack_t<D, T> t = clamp((x - e0) / (e1 - e0), T(0), T(1));
		return t * t * t * (t * (splat<D, T>(T(6)) * t - splat<D, T>(T(15))) + splat<D, T>(T(10)));
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> remap(const simd_pack_t<D, T>& x,
		const simd_pack_t<D, T>& a0,
		const simd_pack_t<D, T>& a1,
		const simd_pack_t<D, T>& b0,
		const simd_pack_t<D, T>& b1)
	{
		return mix(b0, b1, inverse_lerp(a0, a1, x));
	}
	// overloads con extremos escalares
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> smoothstep(T e0, T e1, const simd_pack_t<D, T>& x) {
		return smoothstep(splat<D, T>(e0), splat<D, T>(e1), x);
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> smootherstep(T e0, T e1, const simd_pack_t<D, T>& x) {
		return smootherstep(splat<D, T>(e0), splat<D, T>(e1), x);
	}

	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> smoothstep_noclamp(const simd_pack_t<D, T>& e0,
		const simd_pack_t<D, T>& e1,
		const simd_pack_t<D, T>& x)
	{
		simd_pack_t<D, T> t = (x - e0) / (e1 - e0);
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) {
			T ti = (&t.x)[i];
			(&r.x)[i] = ti * ti * (T(3) - T(2) * ti);
		}
		return r;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> smootherstep_noclamp(const simd_pack_t<D, T>& e0,
		const simd_pack_t<D, T>& e1,
		const simd_pack_t<D, T>& x)
	{
		simd_pack_t<D, T> t = (x - e0) / (e1 - e0);
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) {
			T ti = (&t.x)[i];
			(&r.x)[i] = ti * ti * ti * (ti * (T(6) * ti - T(15)) + T(10));
		}
		return r;
	}

	// Remap (con y sin clamp del t intermedio, si te interesa)
	template<typename T> SIMD_FORCEINLINE T remap(T x, T a0, T a1, T b0, T b1) {
		return b0 + (b1 - b0) * ((x - a0) / (a1 - a0));
	}

	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> remap(const simd_pack_t<D, T>& x,
		T a0, T a1, T b0, T b1)
	{
		return remap(x, splat<D, T>(a0), splat<D, T>(a1), splat<D, T>(b0), splat<D, T>(b1));
	}

	// ==========================================================
	// Especializaciones de 'step' por plataforma (más rápidas)
	//  - Sin ifdefs dentro de funciones
	//  - Devuelven 0.0 / 1.0 por lane
	// ==========================================================

	// -------- float4 (NEON / SSE) --------
#if defined(__aarch64__)
	template<> SIMD_FORCEINLINE simd_pack_t<4, float>
	step<4, float>(const simd_pack_t<4, float>& edge, const simd_pack_t<4, float>& x) {
		auto m = vcgeq_f32(x.m, edge.m);                 // x >= edge
		return simd_pack_t<4, float>(vcvtq_f32_u32(m)); // 1.0 donde true
	}
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
	template<> SIMD_FORCEINLINE simd_pack_t<4, float>
	step<4, float>(const simd_pack_t<4, float>& edge, const simd_pack_t<4, float>& x) {
		// ARMv7: compara por lanes → máscara U32 → a float
		uint32x4_t m = vcgeq_f32(x.m, edge.m);
		return simd_pack_t<4, float>(vcvtq_f32_u32(m));
	}
#elif defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
	template<> SIMD_FORCEINLINE simd_pack_t<4, float>
	step<4, float>(const simd_pack_t<4, float>& edge, const simd_pack_t<4, float>& x) {
		__m128 m = _mm_cmpge_ps(x.m, edge.m);
		return simd_pack_t<4, float>(_mm_and_ps(m, _mm_set1_ps(1.0f)));
	}
#endif

	// -------- float2 (NEON AArch64) --------
#if defined(__aarch64__)
	template<> SIMD_FORCEINLINE simd_pack_t<2, float>
	step<2, float>(const simd_pack_t<2, float>& edge, const simd_pack_t<2, float>& x) {
		auto m = vcge_f32(x.m, edge.m);
		return simd_pack_t<2, float>(vcvt_f32_u32(m));
	}
#endif

	// -------- double2 (AArch64 / SSE2) --------
#if defined(__aarch64__)
	template<> SIMD_FORCEINLINE simd_pack_t<2, double>
	step<2, double>(const simd_pack_t<2, double>& edge, const simd_pack_t<2, double>& x) {
		auto m = vcgeq_f64(x.m, edge.m);
		return simd_pack_t<2, double>(vcvtq_f64_u64(m));
	}
#elif defined(__SSE2__) || defined(_M_X64)
	template<> SIMD_FORCEINLINE simd_pack_t<2, double>
	step<2, double>(const simd_pack_t<2, double>& edge, const simd_pack_t<2, double>& x) {
		__m128d m = _mm_cmpge_pd(x.m, edge.m);
		return simd_pack_t<2, double>(_mm_and_pd(m, _mm_set1_pd(1.0)));
	}
#endif

	// -------- double4 (AVX) --------
#if defined(__AVX__) || defined(_M_AVX)
	template<> SIMD_FORCEINLINE simd_pack_t<4, double>
	step<4, double>(const simd_pack_t<4, double>& edge, const simd_pack_t<4, double>& x) {
		__m256d m = _mm256_cmp_pd(x.m, edge.m, _CMP_GE_OQ);
		return simd_pack_t<4, double>(_mm256_and_pd(m, _mm256_set1_pd(1.0)));
	}
#endif

	// ==========================================================
	// Azúcar: variantes "por componentes" con escalar 't'
	// (ya cubiertas arriba, pero dejo alias por legibilidad)
	// ==========================================================
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> mix(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b, const T& t_scalar) {
		return mix(a, b, splat<D, T>(t_scalar));
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> lerp(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b, const T& t_scalar) {
		return mix(a, b, t_scalar);
	}

} // namespace simd



namespace simd {

	// ==========================================================
	// Helpers
	// ==========================================================

	// Horner para polinomio cúbico: (((a3*t) + a2)*t + a1)*t + a0
	template<typename T>
	SIMD_FORCEINLINE T horner_cubic(T t, T a3, T a2, T a1, T a0) {
		return ((a3 * t + a2) * t + a1) * t + a0;
	}
	template<int D, typename T> simd_pack_t<D, T> horner_cubic(const simd_pack_t<D, T>& t,
		const simd_pack_t<D, T>& a3,
		const simd_pack_t<D, T>& a2,
		const simd_pack_t<D, T>& a1,
		const simd_pack_t<D, T>& a0);

	// ==========================================================
	// Hermite cúbica
	// p(t) = (2t^3-3t^2+1)p0 + (t^3-2t^2+t)m0 + (-2t^3+3t^2)p1 + (t^3-t^2)m1
	// ==========================================================
	template<typename T> T hermite_cubic(T p0, T p1, T m0, T m1, T t) ;

	template<int D, typename T> simd_pack_t<D, T> hermite_cubic(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& m0,
		const simd_pack_t<D, T>& m1,
		const simd_pack_t<D, T>& t);
	// ==========================================================
	// Catmull–Rom (τ = 0.5 por defecto)
	// m1 = τ*(p2 - p0), m2 = τ*(p3 - p1)
	// ==========================================================
	template<typename T>
	SIMD_FORCEINLINE T catmull_rom(T p0, T p1, T p2, T p3, T t, T tau = T(0.5)) {
		T m1 = tau * (p2 - p0);
		T m2 = tau * (p3 - p1);
		return hermite_cubic(p1, p2, m1, m2, t);
	}

	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> catmull_rom(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& p3,
		const simd_pack_t<D, T>& t,
		T tau = T(0.5));

	// ==========================================================
	// Bézier cuadrática (Horner)
	// B2(t) = ((p0 - 2p1 + p2) t + (-2p0 + 2p1)) t + p0
	// ==========================================================
	template<typename T>
	SIMD_FORCEINLINE T bezier_quadratic(T p0, T p1, T p2, T t);

	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> bezier_quadratic(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& t);

	// ==========================================================
	// Bézier cúbica (Horner)
	// B3(t) = (((-p0+3p1-3p2+p3)t + (3p0-6p1+3p2))t + (-3p0+3p1))t + p0
	// ==========================================================
	template<typename T>
	SIMD_FORCEINLINE T bezier_cubic(T p0, T p1, T p2, T p3, T t);

	template<int D, typename T> simd_pack_t<D, T> bezier_cubic(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& p3,
		const simd_pack_t<D, T>& t);

	// ==========================================================
	// TCB (Kochanek–Bartels) — versión escalar
	// ==========================================================
	template<typename T> T tcb(T p0, T p1, T p2, T p3, T t,
		T tension = T(0),
		T continuity = T(0),
		T bias = T(0));
	// ==========================================================
	// TCB — parámetros T/C/B escalares (pack de puntos)
	// ==========================================================
	template<int D, typename T> simd_pack_t<D, T> tcb(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& p3,
		const simd_pack_t<D, T>& t,
		T tension = T(0),
		T continuity = T(0),
		T bias = T(0));

	// ==========================================================
	// TCB — parámetros T/C/B vectoriales (por componente)
	// ==========================================================
	template<int D, typename T>	simd_pack_t<D, T> tcb(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& p3,
		const simd_pack_t<D, T>& t,
		const simd_pack_t<D, T>& tension,
		const simd_pack_t<D, T>& continuity,
		const simd_pack_t<D, T>& bias);


} // namespace simd
