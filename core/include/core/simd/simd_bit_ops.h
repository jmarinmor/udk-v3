#pragma once
#include <simd/simd_types.h>

namespace simd
{
	template <typename T>	SIMD_FORCEINLINE T	bit_and(const T& a, const T& b) { return a & b; }
	template <typename T>	SIMD_FORCEINLINE T	bit_or(const T& a, const T& b) { return a | b; }
	template <typename T>	SIMD_FORCEINLINE T	bit_xor(const T& a, const T& b) { return a ^ b; }
	template <typename T>	SIMD_FORCEINLINE T	bit_not(const T& v) { return !v;}
}


namespace simd {

	// -------- Fallback genérico por-lane (cubre cualquier D) --------
	template<int D>
	SIMD_FORCEINLINE simd_pack_t<D, float> bit_and(const simd_pack_t<D, float>& a, const simd_pack_t<D, float>& b) {
		simd_pack_t<D, float> r(0.f);
		for (int i = 0;i < D;++i) {
			uint32_t ua = _bit_cast<uint32_t>((&a.x)[i]), ub = _bit_cast<uint32_t>((&b.x)[i]);
			(&r.x)[i] = _bit_cast<float>(ua & ub);
		}
		return r;
	}
	template<int D>
	SIMD_FORCEINLINE simd_pack_t<D, float> bit_or(const simd_pack_t<D, float>& a, const simd_pack_t<D, float>& b) {
		simd_pack_t<D, float> r(0.f);
		for (int i = 0;i < D;++i) {
			uint32_t ua = _bit_cast<uint32_t>((&a.x)[i]), ub = _bit_cast<uint32_t>((&b.x)[i]);
			(&r.x)[i] = _bit_cast<float>(ua | ub);
		}
		return r;
	}
	template<int D>
	SIMD_FORCEINLINE simd_pack_t<D, float> bit_xor(const simd_pack_t<D, float>& a, const simd_pack_t<D, float>& b) {
		simd_pack_t<D, float> r(0.f);
		for (int i = 0;i < D;++i) {
			uint32_t ua = _bit_cast<uint32_t>((&a.x)[i]), ub = _bit_cast<uint32_t>((&b.x)[i]);
			(&r.x)[i] = _bit_cast<float>(ua ^ ub);
		}
		return r;
	}
	template<int D>
	SIMD_FORCEINLINE simd_pack_t<D, float> bit_not(const simd_pack_t<D, float>& a) {
		simd_pack_t<D, float> r(0.f);
		for (int i = 0;i < D;++i) {
			uint32_t ua = _bit_cast<uint32_t>((&a.x)[i]);
			(&r.x)[i] = _bit_cast<float>(~ua);
		}
		return r;
	}

	template<int D>
	SIMD_FORCEINLINE simd_pack_t<D, double> bit_and(const simd_pack_t<D, double>& a, const simd_pack_t<D, double>& b) {
		simd_pack_t<D, double> r(0.0);
		for (int i = 0;i < D;++i) {
			uint64_t ua = _bit_cast<uint64_t>((&a.x)[i]), ub = _bit_cast<uint64_t>((&b.x)[i]);
			(&r.x)[i] = _bit_cast<double>(ua & ub);
		}
		return r;
	}
	template<int D>
	SIMD_FORCEINLINE simd_pack_t<D, double> bit_or(const simd_pack_t<D, double>& a, const simd_pack_t<D, double>& b) {
		simd_pack_t<D, double> r(0.0);
		for (int i = 0;i < D;++i) {
			uint64_t ua = _bit_cast<uint64_t>((&a.x)[i]), ub = _bit_cast<uint64_t>((&b.x)[i]);
			(&r.x)[i] = _bit_cast<double>(ua | ub);
		}
		return r;
	}
	template<int D>
	SIMD_FORCEINLINE simd_pack_t<D, double> bit_xor(const simd_pack_t<D, double>& a, const simd_pack_t<D, double>& b) {
		simd_pack_t<D, double> r(0.0);
		for (int i = 0;i < D;++i) {
			uint64_t ua = _bit_cast<uint64_t>((&a.x)[i]), ub = _bit_cast<uint64_t>((&b.x)[i]);
			(&r.x)[i] = _bit_cast<double>(ua ^ ub);
		}
		return r;
	}
	template<int D>
	SIMD_FORCEINLINE simd_pack_t<D, double> bit_not(const simd_pack_t<D, double>& a) {
		simd_pack_t<D, double> r(0.0);
		for (int i = 0;i < D;++i) {
			uint64_t ua = _bit_cast<uint64_t>((&a.x)[i]);
			(&r.x)[i] = _bit_cast<double>(~ua);
		}
		return r;
	}

} // namespace simd








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Especializaciones
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(__AVX__) || defined(_M_AVX)
// AVX: float8 / double4
template<> SIMD_FORCEINLINE simd_pack_t<8, float>  simd::bit_and(const simd_pack_t<8, float>& a, const simd_pack_t<8, float>& b) { return simd_pack_t<8, float>(_mm256_and_ps(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<8, float>  simd::bit_or(const simd_pack_t<8, float>& a, const simd_pack_t<8, float>& b) { return simd_pack_t<8, float>(_mm256_or_ps(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<8, float>  simd::bit_xor(const simd_pack_t<8, float>& a, const simd_pack_t<8, float>& b) { return simd_pack_t<8, float>(_mm256_xor_ps(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<8, float>  simd::bit_not(const simd_pack_t<8, float>& a) { return simd_pack_t<8, float>(_mm256_xor_ps(a.m, _mm256_castsi256_ps(_mm256_set1_epi32(-1)))); }

template<> SIMD_FORCEINLINE simd_pack_t<4, double> simd::bit_and(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_and_pd(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<4, double> simd::bit_or(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_or_pd(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<4, double> simd::bit_xor(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_xor_pd(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<4, double> simd::bit_not(const simd_pack_t<4, double>& a) { return simd_pack_t<4, double>(_mm256_xor_pd(a.m, _mm256_castsi256_pd(_mm256_set1_epi32(-1)))); }
#endif

#if defined(__SSE2__) || defined(_M_X64) || (defined(__SSE__) || (defined(_M_IX86_FP) && _M_IX86_FP>=1))
// SSE: float4 / double2
template<> SIMD_FORCEINLINE simd_pack_t<4, float>  simd::bit_and(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_and_ps(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<4, float>  simd::bit_or(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_or_ps(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<4, float>  simd::bit_xor(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_xor_ps(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<4, float>  simd::bit_not(const simd_pack_t<4, float>& a) { return simd_pack_t<4, float>(_mm_xor_ps(a.m, _mm_castsi128_ps(_mm_set1_epi32(-1)))); }

template<> SIMD_FORCEINLINE simd_pack_t<2, double> simd::bit_and(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_and_pd(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<2, double> simd::bit_or(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_or_pd(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<2, double> simd::bit_xor(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_xor_pd(a.m, b.m)); }
template<> SIMD_FORCEINLINE simd_pack_t<2, double> simd::bit_not(const simd_pack_t<2, double>& a) { return simd_pack_t<2, double>(_mm_xor_pd(a.m, _mm_castsi128_pd(_mm_set1_epi32(-1)))); }
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
// NEON: float4 / (AArch64: double2)
template<> SIMD_FORCEINLINE simd_pack_t<4, float> simd::bit_and(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(a.m), vreinterpretq_u32_f32(b.m)))); }
template<> SIMD_FORCEINLINE simd_pack_t<4, float> simd::bit_or(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(a.m), vreinterpretq_u32_f32(b.m)))); }
template<> SIMD_FORCEINLINE simd_pack_t<4, float> simd::bit_xor(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(a.m), vreinterpretq_u32_f32(b.m)))); }
template<> SIMD_FORCEINLINE simd_pack_t<4, float> simd::bit_not(const simd_pack_t<4, float>& a) { return simd_pack_t<4, float>(vreinterpretq_f32_u32(vmvnq_u32(vreinterpretq_u32_f32(a.m)))); }

#if defined(__aarch64__)
template<> SIMD_FORCEINLINE simd_pack_t<2, double> simd::bit_and(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vreinterpretq_f64_u64(vandq_u64(vreinterpretq_u64_f64(a.m), vreinterpretq_u64_f64(b.m)))); }
template<> SIMD_FORCEINLINE simd_pack_t<2, double> simd::bit_or(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vreinterpretq_f64_u64(vorrq_u64(vreinterpretq_u64_f64(a.m), vreinterpretq_u64_f64(b.m)))); }
template<> SIMD_FORCEINLINE simd_pack_t<2, double> simd::bit_xor(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vreinterpretq_f64_u64(veorq_u64(vreinterpretq_u64_f64(a.m), vreinterpretq_u64_f64(b.m)))); }
template<> SIMD_FORCEINLINE simd_pack_t<2, double> simd::bit_not(const simd_pack_t<2, double>& a) { return simd_pack_t<2, double>(vreinterpretq_f64_u64(vmvnq_u64(vreinterpretq_u64_f64(a.m)))); }
#endif
#endif
