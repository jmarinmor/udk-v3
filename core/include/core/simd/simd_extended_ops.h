// simd_extended_ops.inl
#pragma once

#include <simd/simd_types.h>

namespace simd {

	// ============================================================
	//  Declaraciones genéricas (plantillas base, escalar)
	// ============================================================
	template <typename T> SIMD_FORCEINLINE T sqrt(const T& v);
	template <typename T> SIMD_FORCEINLINE T rsq(const T& v); // 1/sqrt(v)
	template <typename T> SIMD_FORCEINLINE T rcp(const T& v); // 1/v

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
	SIMD_FORCEINLINE __m128 sse_rcp_nr_ps(__m128 a) {
		__m128 x = _mm_rcp_ps(a);
		const __m128 two = _mm_set1_ps(2.0f);
		x = _mm_mul_ps(x, _mm_sub_ps(two, _mm_mul_ps(a, x)));
		x = _mm_mul_ps(x, _mm_sub_ps(two, _mm_mul_ps(a, x)));
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

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rcp(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(neon_rcp_nr_f32x4(v.m)); }
#elif defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	sqrt(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(_mm_sqrt_ps(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rsq(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(sse_rsqrt_nr_ps(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rcp(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(sse_rcp_nr_ps(v.m)); }
#else
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	sqrt(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z), std::sqrt(v.w)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rsq(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(1.f / std::sqrt(v.x), 1.f / std::sqrt(v.y), 1.f / std::sqrt(v.z), 1.f / std::sqrt(v.w)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, float>
	rcp(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(1.f / v.x, 1.f / v.y, 1.f / v.z, 1.f / v.w); }
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

	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rcp(const simd_pack_t<2, float>& v) {
		float32x2_t x = vrecpe_f32(v.m);
		x = vmul_f32(x, vrecps_f32(v.m, x));
		x = vmul_f32(x, vrecps_f32(v.m, x));
		return simd_pack_t<2, float>(x);
	}
#elif (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(__aarch64__)
	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	sqrt(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(neon_sqrt_via_rsqrt_f32x2(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rsq(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(neon_rsqrt_nr_f32x2(v.m)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rcp(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(neon_rcp_nr_f32x2(v.m)); }
#else
	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	sqrt(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(std::sqrt(v.x), std::sqrt(v.y)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rsq(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(1.f / std::sqrt(v.x), 1.f / std::sqrt(v.y)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	rcp(const simd_pack_t<2, float>& v) { return simd_pack_t<2, float>(1.f / v.x, 1.f / v.y); }
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

	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rcp(const simd_pack_t<2, double>& v) {
		float64x2_t one = vdupq_n_f64(1.0);
		return simd_pack_t<2, double>(vdivq_f64(one, v.m));
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

	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rcp(const simd_pack_t<2, double>& v) {
		__m128d one = _mm_set1_pd(1.0);
		return simd_pack_t<2, double>(_mm_div_pd(one, v.m));
	}
#else
	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	sqrt(const simd_pack_t<2, double>& v) { return simd_pack_t<2, double>(std::sqrt(v.x), std::sqrt(v.y)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rsq(const simd_pack_t<2, double>& v) { return simd_pack_t<2, double>(1.0 / std::sqrt(v.x), 1.0 / std::sqrt(v.y)); }

	template <> SIMD_FORCEINLINE simd_pack_t<2, double>
	rcp(const simd_pack_t<2, double>& v) { return simd_pack_t<2, double>(1.0 / v.x, 1.0 / v.y); }
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

	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	rcp(const simd_pack_t<4, double>& v) {
		__m256d one = _mm256_set1_pd(1.0);
		return simd_pack_t<4, double>(_mm256_div_pd(one, v.m));
	}
#else
	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	sqrt(const simd_pack_t<4, double>& v) { return simd_pack_t<4, double>(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z), std::sqrt(v.w)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	rsq(const simd_pack_t<4, double>& v) { return simd_pack_t<4, double>(1.0 / std::sqrt(v.x), 1.0 / std::sqrt(v.y), 1.0 / std::sqrt(v.z), 1.0 / std::sqrt(v.w)); }

	template <> SIMD_FORCEINLINE simd_pack_t<4, double>
	rcp(const simd_pack_t<4, double>& v) { return simd_pack_t<4, double>(1.0 / v.x, 1.0 / v.y, 1.0 / v.z, 1.0 / v.w); }
#endif

} // namespace simd
