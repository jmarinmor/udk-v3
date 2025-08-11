#pragma once

#include <cmath>
#include <type_traits>

// ===== Inline helpers =====
#ifndef SIMD_INLINE
#define SIMD_INLINE inline
#endif

#ifndef SIMD_FORCEINLINE
#if defined(_MSC_VER)
#define SIMD_FORCEINLINE __forceinline
#else
#define SIMD_FORCEINLINE inline __attribute__((always_inline))
#endif
#endif

// ===============================
//  Detección de ISA + includes
// ===============================

// x86/x64
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#include <emmintrin.h>   // SSE2
#endif

#if defined(__SSE4_1__) || defined(_M_X64)
#include <smmintrin.h>   // SSE4.1 (opcional si lo usas)
#endif

#if defined(__AVX__) || defined(_M_AVX)
#include <immintrin.h>   // AVX
#endif

#if defined(__AVX2__) || defined(_M_AVX2)
#include <immintrin.h>   // AVX2
#endif

// ARM/ARM64
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>    // NEON (float/int)
#endif

#include <cassert>
#include <cstdint>
#include <type_traits>

// ===============================
//  Infra interna
// ===============================
namespace simd_internal {

	template<typename T, int D>
	struct reg { using type_t = T[D]; };

	// NEON
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
	template<> struct reg<float, 2> { using type_t = float32x2_t; };
	template<> struct reg<float, 4> { using type_t = float32x4_t; };
	template<> struct reg<int, 4> { using type_t = int32x4_t; };
	template<> struct reg<unsigned, 4> { using type_t = uint32x4_t; };
#endif

	// NEON + double
#if defined(__aarch64__) || (defined(__ARM_NEON) && defined(__ARM_FEATURE_FP64))
	template<> struct reg<double, 2> { using type_t = float64x2_t; };
#endif

	// SSE2
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
	template<> struct reg<float, 4> { using type_t = __m128; };
	template<> struct reg<int, 4> { using type_t = __m128i; };
	template<> struct reg<double, 2> { using type_t = __m128d; };
#endif

	// Fallback GCC/Clang
#if !defined(__ARM_NEON) && !defined(__ARM_NEON__) \
   && !defined(__SSE2__) && !defined(_M_X64) && !(defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#if defined(__GNUC__) || defined(__clang__)
	template<> struct reg<float, 2> { using type_t = float  __attribute__((__vector_size__(8), __may_alias__)); };
	template<> struct reg<float, 4> { using type_t = float  __attribute__((__vector_size__(16), __may_alias__)); };
	template<> struct reg<int, 4> { using type_t = int    __attribute__((__vector_size__(16), __may_alias__)); };
	template<> struct reg<double, 2> { using type_t = double __attribute__((__vector_size__(16), __may_alias__)); };
#endif
#endif
#if defined(__AVX__) || defined(_M_AVX)
	template<> struct reg<double, 4> { using type_t = __m256d; };
#endif
} // namespace simd_internal


// ===============================
//  Paquetes / vectores genéricos
// ===============================
template <int D, typename T>
struct simd_pack_t {
	static constexpr int component_count() { return D; }
};

// -------- D = 2
template <typename T>
struct simd_pack_t<2, T> {
	union {
		struct { T x, y; };
		typename simd_internal::reg<T, 2>::type_t m;
	};
	static constexpr int component_count() { return 2; }

	SIMD_FORCEINLINE explicit	simd_pack_t(typename simd_internal::reg<T, 2>::type_t value) : m(value) {}
	explicit simd_pack_t(T value) : x(value), y(value) {}
	simd_pack_t(T x, T y) : x(x), y(y) {}

	template <typename T2>
	explicit 	simd_pack_t(const simd_pack_t<2, T2>& value) : x(T(value.x)), y(T(value.y)) {}

	T& channel(int index) { assert(index >= 0 && index < 2); return index == 0 ? x : y; }
	const T& channel(int index) const { assert(index >= 0 && index < 2); return index == 0 ? x : y; }

	T& operator[](int index) { return channel(index); }
	const T& operator[](int index) const { return channel(index); }
};

// -------- D = 3
template <typename T>
struct simd_pack_t<3, T> {
	union {
		struct { T x, y, z; };
		typename simd_internal::reg<T, 3>::type_t m;
	};
	static constexpr int component_count() { return 3; }

	SIMD_FORCEINLINE explicit simd_pack_t(typename simd_internal::reg<T, 3>::type_t value) : m(value) {}
	explicit simd_pack_t(T value) : x(value), y(value), z(value) {}
	simd_pack_t(T x, T y, T z) : x(x), y(y), z(z) {}
	simd_pack_t(const simd_pack_t<2, T>& value, T z) : x(value.x), y(value.y), z(z) {}

	template <typename T2>
	explicit	simd_pack_t(const simd_pack_t<3, T2>& value) : x(T(value.x)), y(T(value.y)), z(T(value.z)) {}

	T& channel(int index) { assert(index >= 0 && index < 3); return (&x)[index]; }
	const T& channel(int index) const { assert(index >= 0 && index < 3); return (&x)[index]; }

	T& operator[](int index) { return channel(index); }
	const T& operator[](int index) const { return channel(index); }
};

// -------- D = 4
template <typename T>
struct simd_pack_t<4, T> {
	union {
		struct { T x, y, z, w; };
		typename simd_internal::reg<T, 4>::type_t m;
	};
	static constexpr int component_count() { return 4; }

	SIMD_FORCEINLINE explicit simd_pack_t(typename simd_internal::reg<T, 4>::type_t value) : m(value) {}
	explicit simd_pack_t(T value) : x(value), y(value), z(value), w(value) {}
	simd_pack_t(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
	simd_pack_t(const simd_pack_t<3, T>& value, T w) : x(value.x), y(value.y), z(value.z), w(w) {}

	template <typename T2>
	explicit simd_pack_t(const simd_pack_t<4, T2>& value)
		: x(T(value.x)), y(T(value.y)), z(T(value.z)), w(T(value.w)) {
	}

	T& channel(int index) { assert(index >= 0 && index < 4); return (&x)[index]; }
	const T& channel(int index) const { assert(index >= 0 && index < 4); return (&x)[index]; }

	T& operator[](int index) { return channel(index); }
	const T& operator[](int index) const { return channel(index); }
};
