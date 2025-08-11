#pragma once

#include <simd/simd_types.h>

namespace simd 
{
	template <typename T>	SIMD_FORCEINLINE T	add(const T& a, const T& b) { return a + b; }
	template <typename T>	SIMD_FORCEINLINE T	sub(const T& a, const T& b) { return a - b; }
	template <typename T>	SIMD_FORCEINLINE T	mul(const T& a, const T& b) { return a * b; }
	template <typename T>	SIMD_FORCEINLINE T	div(const T& a, const T& b) { return a / b; }
	template <typename T>	SIMD_FORCEINLINE T	min(const T& a, const T& b) { return a <= b ? a : b; }
	template <typename T>	SIMD_FORCEINLINE T	max(const T& a, const T& b) { return a >= b ? a : b; }
	template <typename T>	SIMD_FORCEINLINE T	rcp(const T& v) { return static_cast<T>(1.0 / static_cast<double>(v));}
}

// rcp
namespace simd 
{
	template <>	SIMD_FORCEINLINE float			rcp<float>(const float& v) { return 1.0f / v; }
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
	template <> SIMD_FORCEINLINE simd_pack_t<4, float> rcp(const simd_pack_t<4, float>& v) 
	{
		float32x4_t x = vrecpeq_f32(v.m);
		x = vmulq_f32(x, vrecpsq_f32(v.m, x));
		x = vmulq_f32(x, vrecpsq_f32(v.m, x));
		return simd_pack_t<4, float>(x);
	}
	//template <> SIMD_FORCEINLINE simd_pack_t<2, float>
	//rcp(const simd_pack_t<2, float>& v) {
	//	float32x2_t x = vrecpe_f32(v.m);
	//	x = vmul_f32(x, vrecps_f32(v.m, x));
	//	x = vmul_f32(x, vrecps_f32(v.m, x));
	//	return simd_pack_t<2, float>(x);
	//}
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
	template <> SIMD_FORCEINLINE simd_pack_t<4, float> rcp(const simd_pack_t<4, float>& v) 
	{
		float32x4_t x = vrecpeq_f32(v.m);
		x = vmulq_f32(x, vrecpsq_f32(v.m, x));
		x = vmulq_f32(x, vrecpsq_f32(v.m, x));
		return simd_pack_t<4, float>(x);
	}
	template <> SIMD_FORCEINLINE simd_pack_t<2, float> rcp(const simd_pack_t<2, float>& v) 
	{
		float32x2_t x = vrecpe_f32(v.m);
		x = vmul_f32(x, vrecps_f32(v.m, x));
		x = vmul_f32(x, vrecps_f32(v.m, x));
		return simd_pack_t<2, float>(x);
	}
#elif defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
	template <> SIMD_FORCEINLINE simd_pack_t<4, float>		rcp(const simd_pack_t<4, float>& v) { return simd_pack_t<4, float>(sse_rcp_nr_ps(v.m)); }
#endif

}


namespace simd 
{

	// Fallbacks escalares (genéricos)
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> add(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) {
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = (&a.x)[i] + (&b.x)[i];
		return r;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> sub(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) {
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = (&a.x)[i] - (&b.x)[i];
		return r;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> mul(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) {
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = (&a.x)[i] * (&b.x)[i];
		return r;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> div(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) {
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = (&a.x)[i] / (&b.x)[i];
		return r;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> min(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) {
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = (&a.x)[i] <= (&b.x)[i] ? (&a.x)[i] : (&b.x)[i];
		return r;
	}
	template<int D, typename T>
	SIMD_FORCEINLINE simd_pack_t<D, T> max(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) {
		simd_pack_t<D, T> r(T{});
		for (int i = 0;i < D;++i) (&r.x)[i] = (&a.x)[i] >= (&b.x)[i] ? (&a.x)[i] : (&b.x)[i];
		return r;
	}
}

namespace simd
{
	#define SIMD_GEN_BASIC_OP_4(_func, _op, _type) \
	template<> SIMD_FORCEINLINE simd_pack_t<4, _type>	_func<4, _type>(const simd_pack_t<4, _type>& a, const simd_pack_t<4, _type>& b)  \
	{ \
		return simd_pack_t<4, _type>(a.x _op b.x, a.y _op b.y, a.z _op b.z, a.w _op b.w); \
	} \
	template<> SIMD_FORCEINLINE simd_pack_t<4, _type>	_func<4, _type>(const simd_pack_t<4, _type>& a, const _type& b)  \
	{ \
		return simd_pack_t<4, _type>(a.x _op b, a.y _op b, a.z _op b, a.w _op b); \
	} \
	template<> SIMD_FORCEINLINE simd_pack_t<4, _type>	_func<4, _type>(const _type& a, const simd_pack_t<4, _type>& b)  \
	{ \
		return simd_pack_t<4, _type>(a _op b.x, a _op b.y, a _op b.z, a _op b.w); \
	} \

	#define SIMD_GEN_BASIC_OP_3(_func, _op, _type) \
	template<> SIMD_FORCEINLINE simd_pack_t<3, _type>	_func<3, _type>(const simd_pack_t<3, _type>& a, const simd_pack_t<3, _type>& b)  \
	{ \
		return simd_pack_t<3, _type>(a.x _op b.x, a.y _op b.y, a.z _op b.z); \
	} \
	template<> SIMD_FORCEINLINE simd_pack_t<3, _type>	_func<3, _type>(const simd_pack_t<3, _type>& a, const _type& b)  \
	{ \
		return simd_pack_t<3, _type>(a.x _op b, a.y _op b, a.z _op b); \
	} \
	template<> SIMD_FORCEINLINE simd_pack_t<3, _type>	_func<3, _type>(const _type& a, const simd_pack_t<3, _type>& b)  \
	{ \
		return simd_pack_t<3, _type>(a _op b.x, a _op b.y, a _op b.z); \
	} \
	
	#define SIMD_GEN_BASIC_OP_2(_func, _op, _type) \
	template<> SIMD_FORCEINLINE simd_pack_t<2, _type>	_func<2, _type>(const simd_pack_t<2, _type>& a, const simd_pack_t<2, _type>& b)  \
	{ \
		return simd_pack_t<2, _type>(a.x _op b.x, a.y _op b.y); \
	} \
	template<> SIMD_FORCEINLINE simd_pack_t<2, _type>	_func<2, _type>(const simd_pack_t<2, _type>& a, const _type& b)  \
	{ \
		return simd_pack_t<2, _type>(a.x _op b, a.y _op b); \
	} \
	template<> SIMD_FORCEINLINE simd_pack_t<2, _type>	_func<2, _type>(const _type& a, const simd_pack_t<2, _type>& b)  \
	{ \
		return simd_pack_t<2, _type>(a _op b.x, a _op b.y); \
	} \

	#define SIMD_GEN_BASIC_OPS_ALL(_func, _op, _type) \
		SIMD_GEN_BASIC_OP_4(_func, _op, _type)\
		SIMD_GEN_BASIC_OP_3(_func, _op, _type)\
		SIMD_GEN_BASIC_OP_2(_func, _op, _type)

	#define SIMD_GEN_BASIC_OPS(_type) \
		SIMD_GEN_BASIC_OPS_ALL(mul, *, _type)\
		SIMD_GEN_BASIC_OPS_ALL(add, +, _type)\
		SIMD_GEN_BASIC_OPS_ALL(sub, -, _type)\
		SIMD_GEN_BASIC_OPS_ALL(div, /, _type)

	SIMD_GEN_BASIC_OPS(uint32_t)
	SIMD_GEN_BASIC_OPS(uint64_t)
	SIMD_GEN_BASIC_OPS(int32_t)
	SIMD_GEN_BASIC_OPS(int64_t)
	SIMD_GEN_BASIC_OPS(float)
	SIMD_GEN_BASIC_OPS(double)
}

namespace simd 
{
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

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> add<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vaddq_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> sub<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vsubq_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> mul<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vmulq_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, float> add<2, float>(const simd_pack_t<2, float>& a, const simd_pack_t<2, float>& b) { return simd_pack_t<2, float>(vadd_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, float> sub<2, float>(const simd_pack_t<2, float>& a, const simd_pack_t<2, float>& b) { return simd_pack_t<2, float>(vsub_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, float> mul<2, float>(const simd_pack_t<2, float>& a, const simd_pack_t<2, float>& b) { return simd_pack_t<2, float>(vmul_f32(a.m, b.m)); }
#	if !defined(__aarch64__)
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> div<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(neon_div_f32x4(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, float> div<2, float>(const simd_pack_t<2, float>& a, const simd_pack_t<2, float>& b) { return simd_pack_t<2, float>(neon_div_f32x2(a.m, b.m)); }
#	endif
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> min<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vminq_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> max<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vmaxq_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, float> min<2, float>(const simd_pack_t<2, float>& a, const simd_pack_t<2, float>& b) { return simd_pack_t<2, float>(vmin_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, float> max<2, float>(const simd_pack_t<2, float>& a, const simd_pack_t<2, float>& b) { return simd_pack_t<2, float>(vmax_f32(a.m, b.m)); }

	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			add<4, int>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(vaddq_s32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	add<4, unsigned>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(vaddq_u32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			sub<4, int>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(vsubq_s32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	sub<4, unsigned>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(vsubq_u32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			min<4, int>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(vminq_s32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	min<4, unsigned>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(vminq_u32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			max<4, int>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(vmaxq_s32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	max<4, unsigned>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(vmaxq_u32(a.m, b.m)); }

#elif defined(__SSE__) || defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> add<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_add_ps(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> sub<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_sub_ps(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> mul<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_mul_ps(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> div<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_div_ps(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> min<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_min_ps(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> max<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(_mm_max_ps(a.m, b.m)); }
#endif


#if defined(__aarch64__)
	template<> SIMD_FORCEINLINE simd_pack_t<4, float> 		div<4, float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, float>& b) { return simd_pack_t<4, float>(vdivq_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, float> 		div<2, float>(const simd_pack_t<2, float>& a, const simd_pack_t<2, float>& b) { return simd_pack_t<2, float>(vdiv_f32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> 		add<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vaddq_f64(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> 		sub<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vsubq_f64(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> 		mul<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vmulq_f64(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> 		div<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vdivq_f64(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> 		min<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vminq_f64(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> 		max<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(vmaxq_f64(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, int32_t>		mul<4, int>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) {return simd_pack_t<4, int>(vmulq_s32(a.m, b.m));}
	template<> SIMD_FORCEINLINE simd_pack_t<4, uint32_t>	mul<4, unsigned>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) {return simd_pack_t<4, unsigned>(vmulq_u32(a.m, b.m));}
#endif

#if defined(__SSE2__) || defined(_M_X64)
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> add<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_add_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> sub<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_sub_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> mul<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_mul_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> div<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_div_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> min<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_min_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<2, double> max<2, double>(const simd_pack_t<2, double>& a, const simd_pack_t<2, double>& b) { return simd_pack_t<2, double>(_mm_max_pd(a.m, b.m)); }
#endif



#if defined(__SSE2__) || defined(_M_X64)
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			add<4, int>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(_mm_add_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	add<4, unsigned>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(_mm_add_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			sub<4, int>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(_mm_sub_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	sub<4, unsigned>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(_mm_sub_epi32(a.m, b.m)); }
#if defined(__SSE4_1__)
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			mul<4, int>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(_mm_mullo_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	mul<4, unsigned>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(_mm_mullo_epi32(a.m, b.m)); }
#else
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			mul<4, int32_t>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { simd_pack_t<4, int> r; r.x=a.x*b.x; r.y=a.y*b.y; r.z=a.z*b.z; r.w=a.w*b.w; return r; }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	mul<4, uint32_t>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { simd_pack_t<4, unsigned> r; r.x=a.x*b.x; r.y=a.y*b.y; r.z=a.z*b.z; r.w=a.w*b.w; return r; }
#endif

#if defined(__SSE4_1__)
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			min<4, int32_t>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(_mm_min_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	min<4, uint32_t>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(_mm_min_epu32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			max<4, int32_t>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) { return simd_pack_t<4, int>(_mm_max_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	max<4, uint32_t>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) { return simd_pack_t<4, unsigned>(_mm_max_epu32(a.m, b.m)); }
#else
	// Fallback con compare+blend si no hay SSE4.1
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			min<4, int32_t>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) {
		__m128i mask = _mm_cmplt_epi32(a.m, b.m);
		return simd_pack_t<4, int>(_mm_or_si128(_mm_and_si128(mask, a.m), _mm_andnot_si128(mask, b.m)));
	}
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	min<4, uint32_t>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) {
		__m128i as = _mm_sub_epi32(a.m, _mm_set1_epi32(0x80000000u));
		__m128i bs = _mm_sub_epi32(b.m, _mm_set1_epi32(0x80000000u));
		__m128i mask = _mm_cmplt_epi32(as, bs);
		return simd_pack_t<4, unsigned>(_mm_or_si128(_mm_and_si128(mask, a.m), _mm_andnot_si128(mask, b.m)));
	}
	template<> SIMD_FORCEINLINE simd_pack_t<4, int>			max<4, int32_t>(const simd_pack_t<4, int>& a, const simd_pack_t<4, int>& b) {
		__m128i mask = _mm_cmpgt_epi32(a.m, b.m);
		return simd_pack_t<4, int>(_mm_or_si128(_mm_and_si128(mask, a.m), _mm_andnot_si128(mask, b.m)));
	}
	template<> SIMD_FORCEINLINE simd_pack_t<4, unsigned>	max<4, uint32_t>(const simd_pack_t<4, unsigned>& a, const simd_pack_t<4, unsigned>& b) {
		__m128i as = _mm_sub_epi32(a.m, _mm_set1_epi32(0x80000000u));
		__m128i bs = _mm_sub_epi32(b.m, _mm_set1_epi32(0x80000000u));
		__m128i mask = _mm_cmpgt_epi32(as, bs);
		return simd_pack_t<4, unsigned>(_mm_or_si128(_mm_and_si128(mask, a.m), _mm_andnot_si128(mask, b.m)));
	}
#endif
#endif // SSE2


#if defined(__AVX__) || defined(_M_AVX)
	template<> SIMD_FORCEINLINE simd_pack_t<4, double> add<4, double>(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_add_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, double> sub<4, double>(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_sub_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, double> mul<4, double>(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_mul_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, double> div<4, double>(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_div_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, double> min<4, double>(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_min_pd(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<4, double> max<4, double>(const simd_pack_t<4, double>& a, const simd_pack_t<4, double>& b) { return simd_pack_t<4, double>(_mm256_max_pd(a.m, b.m)); }
#endif
#if defined(__AVX2__) || defined(_M_AVX2)
	template<> SIMD_FORCEINLINE simd_pack_t<8, int32_t>		add<8, int>(const simd_pack_t<8, int>& a, const simd_pack_t<8, int>& b) { return simd_pack_t<8, int>(_mm256_add_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<8, uint32_t>	add<8, unsigned>(const simd_pack_t<8, unsigned>& a, const simd_pack_t<8, unsigned>& b) { return simd_pack_t<8, unsigned>(_mm256_add_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<8, int32_t>		sub<8, int>(const simd_pack_t<8, int>& a, const simd_pack_t<8, int>& b) { return simd_pack_t<8, int>(_mm256_sub_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<8, uint32_t>	sub<8, unsigned>(const simd_pack_t<8, unsigned>& a, const simd_pack_t<8, unsigned>& b) { return simd_pack_t<8, unsigned>(_mm256_sub_epi32(a.m, b.m)); }

	template<> SIMD_FORCEINLINE simd_pack_t<8, int32_t>		mul<8, int>(const simd_pack_t<8, int>& a, const simd_pack_t<8, int>& b) { return simd_pack_t<8, int>(_mm256_mullo_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<8, uint32_t>	mul<8, unsigned>(const simd_pack_t<8, unsigned>& a, const simd_pack_t<8, unsigned>& b) { return simd_pack_t<8, unsigned>(_mm256_mullo_epi32(a.m, b.m)); }

	template<> SIMD_FORCEINLINE simd_pack_t<8, int32_t>		min<8, int>(const simd_pack_t<8, int>& a, const simd_pack_t<8, int>& b) { return simd_pack_t<8, int>(_mm256_min_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<8, uint32_t>	min<8, unsigned>(const simd_pack_t<8, unsigned>& a, const simd_pack_t<8, unsigned>& b) { return simd_pack_t<8, unsigned>(_mm256_min_epu32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<8, int32_t>		max<8, int>(const simd_pack_t<8, int>& a, const simd_pack_t<8, int>& b) { return simd_pack_t<8, int>(_mm256_max_epi32(a.m, b.m)); }
	template<> SIMD_FORCEINLINE simd_pack_t<8, uint32_t>	max<8, unsigned>(const simd_pack_t<8, unsigned>& a, const simd_pack_t<8, unsigned>& b) { return simd_pack_t<8, unsigned>(_mm256_max_epu32(a.m, b.m)); }
#endif

} // namespace simd



// Operators
namespace simd
{
	// Binarios vector-vector
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator+(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) { return add<D, T>(a, b); }
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator+(const simd_pack_t<D, T>& a, T b) { return add<D, T>(a, b); }
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator+(T a, const simd_pack_t<D, T>& b) { return add<D, T>(a, b); }

	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator-(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) { return sub<D, T>(a, b); }
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator-(const simd_pack_t<D, T>& a, T b) { return sub<D, T>(a, b); }
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator-(T a, const simd_pack_t<D, T>& b) { return sub<D, T>(a, b); }

	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator*(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) { return mul<D, T>(a, b); }
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator*(const simd_pack_t<D, T>& a, T b) { return mul<D, T>(a, b); }
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator*(T a, const simd_pack_t<D, T>& b) { return mul<D, T>(a, b); }

	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator/(const simd_pack_t<D, T>& a, const simd_pack_t<D, T>& b) { return div<D, T>(a, b); }
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator/(const simd_pack_t<D, T>& a, T b) { return div<D, T>(a, b); }
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator/(T a, const simd_pack_t<D, T>& b) { return div<D, T>(a, b); }

	// Unario negación
	template<int D, typename T> SIMD_FORCEINLINE simd_pack_t<D, T> operator-(const simd_pack_t<D, T>& v) { return v * T(-1); }

}