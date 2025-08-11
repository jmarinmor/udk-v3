#pragma once
#include <simd/simd_types.h>

namespace simd {

// -----------------------------------------------------
// Utilidades básicas
// -----------------------------------------------------

// bit_cast portable (sin exigir C++20)
template<typename To, typename From>
SIMD_FORCEINLINE To _bit_cast(const From& src) noexcept {
	static_assert(sizeof(To) == sizeof(From), "bit_cast: tamaños incompatibles");
	To dst; std::memcpy(&dst, &src, sizeof(To)); return dst;
}


// -----------------------------------------------------
// 6) Conversiones y empaquetado
// -----------------------------------------------------

// Widen/narrow con saturación (fallback por-lane)
template<int D>
SIMD_FORCEINLINE simd_pack_t<D, int16_t> pack_saturate(const simd_pack_t<D, int32_t>& a) {
	simd_pack_t<D, int16_t> r(int16_t{0});
	for (int i=0;i<D;++i) {
		int32_t v = (&a.x)[i];
		if (v >  32767) v =  32767;
		if (v < -32768) v = -32768;
		(&r.x)[i] = (int16_t)v;
	}
	return r;
}

template<int D>
SIMD_FORCEINLINE simd_pack_t<D, uint8_t> pack_saturate(const simd_pack_t<D, float>& a) {
	simd_pack_t<D, uint8_t> r(uint8_t{0});
	for (int i=0;i<D;++i) {
		float v = (&a.x)[i];
		int iv = (int)(v + (v >= 0.f ? 0.5f : -0.5f)); // round-to-nearest
		if (iv < 0) iv = 0; else if (iv > 255) iv = 255;
		(&r.x)[i] = (uint8_t)iv;
	}
	return r;
}

// float <-> half (solo declaraciones escalares aquí; implementación en .cpp)
uint16_t float_to_half(float f);
float    half_to_float(uint16_t h);

// Packs float<->half por-lane (inline)
template<int D>
SIMD_FORCEINLINE simd_pack_t<D, uint16_t> float_to_half(const simd_pack_t<D, float>& a) {
	simd_pack_t<D, uint16_t> r(uint16_t{0});
	for (int i=0;i<D;++i) (&r.x)[i] = float_to_half((&a.x)[i]);
	return r;
}
template<int D>
SIMD_FORCEINLINE simd_pack_t<D, float> half_to_float(const simd_pack_t<D, uint16_t>& a) {
	simd_pack_t<D, float> r(0.f);
	for (int i=0;i<D;++i) (&r.x)[i] = half_to_float((&a.x)[i]);
	return r;
}

} // namespace simd



// -----------------------------------------------------
// 9) Swizzles / Permutes (fallbacks + especializaciones)
// -----------------------------------------------------
namespace simd {

// Fallback compile-time: float4/double4
template<typename T, int X, int Y, int Z, int W>
SIMD_FORCEINLINE simd_pack_t<4, T> shuffle(const simd_pack_t<4, T>& a) {
	static_assert(0 <= X && X < 4 && 0 <= Y && Y < 4 && 0 <= Z && Z < 4 && 0 <= W && W < 4, "shuffle index out of range");
	const T v[4] = { a.x,a.y,a.z,a.w };
	return simd_pack_t<4, T>(v[X], v[Y], v[Z], v[W]);
}
// Para double2: shuffle<X,Y>
template<int X, int Y>
SIMD_FORCEINLINE simd_pack_t<2, double> shuffle(const simd_pack_t<2, double>& a) {
	static_assert(0 <= X && X < 2 && 0 <= Y && Y < 2, "shuffle2 index");
	const double v[2] = { a.x,a.y };
	return simd_pack_t<2, double>(v[X], v[Y]);
}

// Patrones comunes (float4/double4) – fallbacks
template<typename T> SIMD_FORCEINLINE simd_pack_t<4, T> dup_x (const simd_pack_t<4, T>& a) { return simd_pack_t<4, T>(a.x, a.x, a.x, a.x); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4, T> dup_y (const simd_pack_t<4, T>& a) { return simd_pack_t<4, T>(a.y, a.y, a.y, a.y); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4, T> dup_z (const simd_pack_t<4, T>& a) { return simd_pack_t<4, T>(a.z, a.z, a.z, a.z); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4, T> dup_w (const simd_pack_t<4, T>& a) { return simd_pack_t<4, T>(a.w, a.w, a.w, a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4, T> swap_xy(const simd_pack_t<4, T>& a) { return simd_pack_t<4, T>(a.y, a.x, a.z, a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4, T> swap_zw(const simd_pack_t<4, T>& a) { return simd_pack_t<4, T>(a.x, a.y, a.w, a.z); }

// Permute runtime (array o pack<int,4>)
template<typename T>
SIMD_FORCEINLINE simd_pack_t<4, T> permute(const simd_pack_t<4, T>& a, const int idx[4]) {
	const T v[4] = { a.x,a.y,a.z,a.w };
	return simd_pack_t<4, T>(v[clampT(idx[0],0,3)], v[clampT(idx[1],0,3)],
	                         v[clampT(idx[2],0,3)], v[clampT(idx[3],0,3)]);
}
template<typename T>
SIMD_FORCEINLINE simd_pack_t<4, T> permute(const simd_pack_t<4, T>& a, const simd_pack_t<4, int>& idx) {
	const T v[4] = { a.x,a.y,a.z,a.w };
	return simd_pack_t<4, T>(v[clampT(idx.x,0,3)], v[clampT(idx.y,0,3)],
	                         v[clampT(idx.z,0,3)], v[clampT(idx.w,0,3)]);
}

} // namespace simd



// -----------------------------------------------------
// ESPECIALIZACIONES SIMD (bloques fuera de funciones)
// -----------------------------------------------------
namespace simd {

// ---------- pack_saturate y float<->half ----------
#if defined(__SSE2__) || defined(_M_X64)
template<> SIMD_FORCEINLINE simd_pack_t<4, uint8_t>
pack_saturate<4>(const simd_pack_t<4, float>& a) {
	__m128  x  = a.m;
	__m128i i32 = _mm_cvtps_epi32(x);                  // redondeo a int32
	__m128i i16 = _mm_packs_epi32(i32, _mm_setzero_si128());
	__m128i u8  = _mm_packus_epi16(i16, _mm_setzero_si128());
	alignas(16) uint8_t out[16];
	_mm_store_si128((__m128i*)out, u8);
	return simd_pack_t<4, uint8_t>(out[0], out[1], out[2], out[3]);
}
template<> SIMD_FORCEINLINE simd_pack_t<4, int16_t>
pack_saturate<4>(const simd_pack_t<4, int32_t>& a) {
	__m128i i16 = _mm_packs_epi32(a.m, _mm_setzero_si128());
	alignas(16) int16_t out[8];
	_mm_store_si128((__m128i*)out, i16);
	return simd_pack_t<4, int16_t>(out[0], out[1], out[2], out[3]);
}
#endif

#if defined(__F16C__)
template<> SIMD_FORCEINLINE simd_pack_t<4, uint16_t>
float_to_half<4>(const simd_pack_t<4, float>& a) {
	__m128  x  = a.m;
	__m128i h4 = _mm_cvtps_ph(x, 0); // RNE
	alignas(16) uint16_t out[8];
	_mm_store_si128((__m128i*)out, h4);
	return simd_pack_t<4, uint16_t>(out[0], out[1], out[2], out[3]);
}
template<> SIMD_FORCEINLINE simd_pack_t<4, float>
half_to_float<4>(const simd_pack_t<4, uint16_t>& a) {
	alignas(16) uint16_t in[8] = { a.x,a.y,a.z,a.w,0,0,0,0 };
	__m128 f4 = _mm_cvtph_ps(_mm_load_si128((const __m128i*)in));
	return simd_pack_t<4, float>(f4);
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
template<> SIMD_FORCEINLINE simd_pack_t<4, uint8_t>
pack_saturate<4>(const simd_pack_t<4, float>& a) {
	// Nota: vcvtq_s32_f32 trunca; si prefieres RN, aplica vrndnq_f32 antes (ARMv8.2)
	int32x4_t i32 = vcvtq_s32_f32(a.m);
	uint16x4_t u16 = vqmovun_s32(i32);
	uint8x8_t  u8  = vqmovn_u16(vcombine_u16(u16, vdup_n_u16(0)));
	return simd_pack_t<4, uint8_t>(vget_lane_u8(u8,0), vget_lane_u8(u8,1),
	                               vget_lane_u8(u8,2), vget_lane_u8(u8,3));
}
template<> SIMD_FORCEINLINE simd_pack_t<4, int16_t>
pack_saturate<4>(const simd_pack_t<4, int32_t>& a) {
	int16x4_t i16 = vqmovn_s32(a.m);
	return simd_pack_t<4, int16_t>(vget_lane_s16(i16,0), vget_lane_s16(i16,1),
	                               vget_lane_s16(i16,2), vget_lane_s16(i16,3));
}
#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC__)
template<> SIMD_FORCEINLINE simd_pack_t<4, uint16_t>
float_to_half<4>(const simd_pack_t<4, float>& a) {
	float16x4_t h = vcvt_f16_f32(a.m);
	return simd_pack_t<4, uint16_t>(
		vget_lane_u16(vreinterpret_u16_f16(h),0),
		vget_lane_u16(vreinterpret_u16_f16(h),1),
		vget_lane_u16(vreinterpret_u16_f16(h),2),
		vget_lane_u16(vreinterpret_u16_f16(h),3));
}
template<> SIMD_FORCEINLINE simd_pack_t<4, float>
half_to_float<4>(const simd_pack_t<4, uint16_t>& a) {
	uint64_t pack = (uint64_t)a.x | ((uint64_t)a.y<<16) | ((uint64_t)a.z<<32) | ((uint64_t)a.w<<48);
	float16x4_t h = vreinterpret_f16_u16(vcreate_u16(pack));
	return simd_pack_t<4, float>(vcvt_f32_f16(h));
}
#endif
#endif // NEON

// ---------- Swizzles / Permutes ----------
#if defined(__SSE__) || defined(__SSE2__) || defined(_M_X64)
// Ejemplos de patrones: reverse para float4
template<> inline simd_pack_t<4, float>
shuffle<float,3,2,1,0>(const simd_pack_t<4, float>& a) {
	return simd_pack_t<4, float>(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(0,1,2,3)));
}
template<> inline simd_pack_t<4, float>
dup_x<float>(const simd_pack_t<4, float>& a) {
	__m128 r = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(0,0,0,0));
	return simd_pack_t<4, float>(r);
}
template<> inline simd_pack_t<4, float>
dup_y<float>(const simd_pack_t<4, float>& a) {
	__m128 r = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(1,1,1,1));
	return simd_pack_t<4, float>(r);
}
template<> inline simd_pack_t<2, double>
shuffle<1,0>(const simd_pack_t<2, double>& a) {
	return simd_pack_t<2, double>(_mm_shuffle_pd(a.m, a.m, 0b01));
}
#endif

#if defined(__AVX__) || defined(_M_AVX)
// Permute runtime (float4) con _mm_permutevar_ps
template<> inline simd_pack_t<4, float>
permute<float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, int>& idx) {
	__m128i m = _mm_set_epi32(idx.w & 3, idx.z & 3, idx.y & 3, idx.x & 3);
	return simd_pack_t<4, float>(_mm_permutevar_ps(a.m, m));
}
#endif

#if defined(__AVX2__) || defined(_M_AVX2)
// double4: permute runtime por 64-bit lanes
template<> inline simd_pack_t<4, double>
permute<double>(const simd_pack_t<4, double>& a, const simd_pack_t<4, int>& idx) {
	int m = ((idx.w & 3) << 6) | ((idx.z & 3) << 4) | ((idx.y & 3) << 2) | (idx.x & 3);
	return simd_pack_t<4, double>(_mm256_permute4x64_pd(a.m, m));
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
// Duplicados frecuentes en NEON
template<> inline simd_pack_t<4, float> dup_x<float>(const simd_pack_t<4, float>& a) { return simd_pack_t<4, float>(vdupq_laneq_f32(a.m, 0)); }
template<> inline simd_pack_t<4, float> dup_y<float>(const simd_pack_t<4, float>& a) { return simd_pack_t<4, float>(vdupq_laneq_f32(a.m, 1)); }
// swap_xy correcto en NEON: [y,x,z,w]
template<> inline simd_pack_t<4, float> swap_xy<float>(const simd_pack_t<4, float>& a) {
	float32x2_t lo = vget_low_f32(a.m);           // [x y]
	float32x2_t hi = vget_high_f32(a.m);          // [z w]
	float32x2_t lo_sw = vrev64_f32(lo);           // [y x]
	return simd_pack_t<4, float>(vcombine_f32(lo_sw, hi));
}
#if defined(__aarch64__)
// Permute runtime float4 con vqtbl1q_u8
template<> inline simd_pack_t<4, float>
permute<float>(const simd_pack_t<4, float>& a, const simd_pack_t<4, int>& idx) {
	uint8x16_t bytes = vreinterpretq_u8_f32(a.m);
	uint8_t m[16];
	for (int i=0;i<4;++i) {
		uint8_t b = uint8_t(((&idx.x)[i] & 3) * 4);
		m[i*4+0]=b+0; m[i*4+1]=b+1; m[i*4+2]=b+2; m[i*4+3]=b+3;
	}
	uint8x16_t sel = vld1q_u8(m);
	float32x4_t r = vreinterpretq_f32_u8(vqtbl1q_u8(bytes, sel));
	return simd_pack_t<4, float>(r);
}
#endif
#endif // NEON

} // namespace simd
