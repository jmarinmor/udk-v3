// simd_curve_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

	// ==========================================================
	// Helpers
	// ==========================================================
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> splat(T v) { return simd_pack_t<D, T>(v); }

	// Horner para polinomio cúbico: (((a3*t) + a2)*t + a1)*t + a0
	template<typename T>
	SIMD_FORCEINLINE T horner_cubic(T t, T a3, T a2, T a1, T a0) {
		return ((a3 * t + a2) * t + a1) * t + a0;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> horner_cubic(const simd_pack_t<D, T>& t,
		const simd_pack_t<D, T>& a3,
		const simd_pack_t<D, T>& a2,
		const simd_pack_t<D, T>& a1,
		const simd_pack_t<D, T>& a0)
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) {
			T ti = (&t.x)[i];
			T c3 = (&a3.x)[i], c2 = (&a2.x)[i], c1 = (&a1.x)[i], c0 = (&a0.x)[i];
			(&r.x)[i] = ((c3 * ti + c2) * ti + c1) * ti + c0;
		}
		return r;
	}

	// ==========================================================
	// Hermite cúbica
	// p(t) = (2t^3-3t^2+1)p0 + (t^3-2t^2+t)m0 + (-2t^3+3t^2)p1 + (t^3-t^2)m1
	// ==========================================================
	template<typename T>
	SIMD_FORCEINLINE T hermite_cubic(T p0, T p1, T m0, T m1, T t) {
		T t2 = t * t, t3 = t2 * t;
		return ((T(2) * t3 - T(3) * t2 + T(1)) * p0)
			+ ((t3 - T(2) * t2 + t) * m0)
			+ ((-T(2) * t3 + T(3) * t2) * p1)
			+ ((t3 - t2) * m1);
	}

	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> hermite_cubic(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& m0,
		const simd_pack_t<D, T>& m1,
		const simd_pack_t<D, T>& t)
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) {
			T p0i = (&p0.x)[i], p1i = (&p1.x)[i], m0i = (&m0.x)[i], m1i = (&m1.x)[i], ti = (&t.x)[i];
			T t2 = ti * ti, t3 = t2 * ti;
			(&r.x)[i] =
				((T(2) * t3 - T(3) * t2 + T(1)) * p0i) +
				((t3 - T(2) * t2 + ti) * m0i) +
				((-T(2) * t3 + T(3) * t2) * p1i) +
				((t3 - t2) * m1i);
		}
		return r;
	}

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
		T tau = T(0.5))
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) {
			T p0i = (&p0.x)[i], p1i = (&p1.x)[i], p2i = (&p2.x)[i], p3i = (&p3.x)[i], ti = (&t.x)[i];
			T m1 = tau * (p2i - p0i);
			T m2 = tau * (p3i - p1i);
			T t2 = ti * ti, t3 = t2 * ti;
			(&r.x)[i] =
				((T(2) * t3 - T(3) * t2 + T(1)) * p1i) +
				((t3 - T(2) * t2 + ti) * m1) +
				((-T(2) * t3 + T(3) * t2) * p2i) +
				((t3 - t2) * m2);
		}
		return r;
	}

	// ==========================================================
	// Bézier cuadrática (Horner)
	// B2(t) = ((p0 - 2p1 + p2) t + (-2p0 + 2p1)) t + p0
	// ==========================================================
	template<typename T>
	SIMD_FORCEINLINE T bezier_quadratic(T p0, T p1, T p2, T t) {
		T c2 = p0 - T(2) * p1 + p2;
		T c1 = -T(2) * p0 + T(2) * p1;
		T c0 = p0;
		return (c2 * t + c1) * t + c0;
	}

	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> bezier_quadratic(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& t)
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) {
			T p0i = (&p0.x)[i], p1i = (&p1.x)[i], p2i = (&p2.x)[i], ti = (&t.x)[i];
			T c2 = p0i - T(2) * p1i + p2i;
			T c1 = -T(2) * p0i + T(2) * p1i;
			T c0 = p0i;
			(&r.x)[i] = (c2 * ti + c1) * ti + c0;
		}
		return r;
	}

	// ==========================================================
	// Bézier cúbica (Horner)
	// B3(t) = (((-p0+3p1-3p2+p3)t + (3p0-6p1+3p2))t + (-3p0+3p1))t + p0
	// ==========================================================
	template<typename T>
	SIMD_FORCEINLINE T bezier_cubic(T p0, T p1, T p2, T p3, T t) {
		T c3 = -p0 + T(3) * p1 - T(3) * p2 + p3;
		T c2 = T(3) * p0 - T(6) * p1 + T(3) * p2;
		T c1 = -T(3) * p0 + T(3) * p1;
		T c0 = p0;
		return horner_cubic(t, c3, c2, c1, c0);
	}

	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> bezier_cubic(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& p3,
		const simd_pack_t<D, T>& t)
	{
		simd_pack_t<D, T> c3(T{}), c2(T{}), c1(T{}), c0(T{});
		for (int i = 0;i < D;++i) {
			T p0i = (&p0.x)[i], p1i = (&p1.x)[i], p2i = (&p2.x)[i], p3i = (&p3.x)[i];
			(&c3.x)[i] = -p0i + T(3) * p1i - T(3) * p2i + p3i;
			(&c2.x)[i] = T(3) * p0i - T(6) * p1i + T(3) * p2i;
			(&c1.x)[i] = -T(3) * p0i + T(3) * p1i;
			(&c0.x)[i] = p0i;
		}
		return horner_cubic(t, c3, c2, c1, c0);
	}

	// ==========================================================
	// TCB (Kochanek–Bartels) — versión escalar
	// ==========================================================
	template<typename T>
	SIMD_FORCEINLINE T tcb(T p0, T p1, T p2, T p3, T t,
		T tension = T(0),
		T continuity = T(0),
		T bias = T(0))
	{
		T m0 = ((T(1) - tension) * (T(1) + continuity) * (T(1) + bias) * T(0.5)) * (p1 - p0)
			+ ((T(1) - tension) * (T(1) - continuity) * (T(1) - bias) * T(0.5)) * (p2 - p1);

		T m1 = ((T(1) - tension) * (T(1) - continuity) * (T(1) + bias) * T(0.5)) * (p2 - p1)
			+ ((T(1) - tension) * (T(1) + continuity) * (T(1) - bias) * T(0.5)) * (p3 - p2);

		return hermite_cubic(p1, p2, m0, m1, t);
	}

	// ==========================================================
	// TCB — parámetros T/C/B escalares (pack de puntos)
	// ==========================================================
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> tcb(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& p3,
		const simd_pack_t<D, T>& t,
		T tension = T(0),
		T continuity = T(0),
		T bias = T(0))
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0; i < D; ++i)
		{
			T p0i = (&p0.x)[i], p1i = (&p1.x)[i], p2i = (&p2.x)[i], p3i = (&p3.x)[i], ti = (&t.x)[i];

			T m0 = ((T(1) - tension) * (T(1) + continuity) * (T(1) + bias) * T(0.5)) * (p1i - p0i)
				+ ((T(1) - tension) * (T(1) - continuity) * (T(1) - bias) * T(0.5)) * (p2i - p1i);

			T m1 = ((T(1) - tension) * (T(1) - continuity) * (T(1) + bias) * T(0.5)) * (p2i - p1i)
				+ ((T(1) - tension) * (T(1) + continuity) * (T(1) - bias) * T(0.5)) * (p3i - p2i);

			T t2 = ti * ti, t3 = t2 * ti;
			(&r.x)[i] =
				((T(2) * t3 - T(3) * t2 + T(1)) * p1i) +
				((t3 - T(2) * t2 + ti) * m0) +
				((-T(2) * t3 + T(3) * t2) * p2i) +
				((t3 - t2) * m1);
		}
		return r;
	}

	// ==========================================================
	// TCB — parámetros T/C/B vectoriales (por componente)
	// ==========================================================
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> tcb(const simd_pack_t<D, T>& p0,
		const simd_pack_t<D, T>& p1,
		const simd_pack_t<D, T>& p2,
		const simd_pack_t<D, T>& p3,
		const simd_pack_t<D, T>& t,
		const simd_pack_t<D, T>& tension,
		const simd_pack_t<D, T>& continuity,
		const simd_pack_t<D, T>& bias)
	{
		simd_pack_t<D, T> r(T{});
		for (int i = 0; i < D; ++i)
		{
			T p0i = (&p0.x)[i], p1i = (&p1.x)[i], p2i = (&p2.x)[i], p3i = (&p3.x)[i], ti = (&t.x)[i];
			T Te = (&tension.x)[i], Co = (&continuity.x)[i], Bi = (&bias.x)[i];

			T m0 = ((T(1) - Te) * (T(1) + Co) * (T(1) + Bi) * T(0.5)) * (p1i - p0i)
				+ ((T(1) - Te) * (T(1) - Co) * (T(1) - Bi) * T(0.5)) * (p2i - p1i);

			T m1 = ((T(1) - Te) * (T(1) - Co) * (T(1) + Bi) * T(0.5)) * (p2i - p1i)
				+ ((T(1) - Te) * (T(1) + Co) * (T(1) - Bi) * T(0.5)) * (p3i - p2i);

			T t2 = ti * ti, t3 = t2 * ti;
			(&r.x)[i] =
				((T(2) * t3 - T(3) * t2 + T(1)) * p1i) +
				((t3 - T(2) * t2 + ti) * m0) +
				((-T(2) * t3 + T(3) * t2) * p2i) +
				((t3 - t2) * m1);
		}
		return r;
	}


} // namespace simd
