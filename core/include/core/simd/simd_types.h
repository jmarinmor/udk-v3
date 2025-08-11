#pragma once

// ===============================
//  Detección de ISA + includes
// ===============================
#include <simd/simd_include.h>

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <cmath>  // opcional: sólo por si usas luego std:: funcs
#include <bit>

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
//  Infra interna
// ===============================
namespace simd_internal {

	// Plantilla base: por defecto usa arrays escalares
	template<typename T, int D>
	struct reg { using type_t = T[D]; };

	// -------- ARM NEON
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
	template<> struct reg<float, 2> { using type_t = float32x2_t; };
	template<> struct reg<float, 4> { using type_t = float32x4_t; };
	template<> struct reg<int, 4> { using type_t = int32x4_t; };
	template<> struct reg<unsigned, 4> { using type_t = uint32x4_t; };
#endif

	// NEON + double (AArch64 o ARMv8 con FP64)
#if defined(__aarch64__) || (defined(__ARM_NEON) && defined(__ARM_FEATURE_FP64))
	template<> struct reg<double, 2> { using type_t = float64x2_t; };
#endif

	// -------- x86/x64 SSE2
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
	template<> struct reg<float, 4> { using type_t = __m128; };
	template<> struct reg<int, 4> { using type_t = __m128i; };
	template<> struct reg<double, 2> { using type_t = __m128d; };
#endif

	// -------- AVX (si alg. día amplías a D=8/4 dobles)
#if defined(__AVX__) || defined(_M_AVX)
	template<> struct reg<double, 4> { using type_t = __m256d; };
	// Nota: no definimos float,D=8 porque tus packs están en D=2/3/4.
#endif

// -------- Fallback GCC/Clang (vector_size) si no hay NEON/SSE
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
    template<> struct reg<float,    8> { using type_t = __m256;  };
    template<> struct reg<int,      8> { using type_t = __m256i; };
    template<> struct reg<unsigned, 8> { using type_t = __m256i; };
#endif

// Fallback GCC/Clang (vector_size) si no hay AVX
#if !defined(__AVX__) && !defined(_M_AVX)
# if defined(__GNUC__) || defined(__clang__)
    template<> struct reg<float,    8> { using type_t = float    __attribute__((__vector_size__(32), __may_alias__)); };
    template<> struct reg<int,      8> { using type_t = int      __attribute__((__vector_size__(32), __may_alias__)); };
    template<> struct reg<unsigned, 8> { using type_t = unsigned __attribute__((__vector_size__(32), __may_alias__)); };
# endif
#endif

	// -------- D=3: asegura que exista type_t también para 3 componentes
	// Usamos un contenedor trivial para D=3 en todas las T.
	template<typename T>
	struct reg<T, 3> { struct _wrap { T v[3]; }; using type_t = _wrap; };

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

	SIMD_FORCEINLINE explicit constexpr	simd_pack_t(typename simd_internal::reg<T, 2>::type_t value) : m(value) {}
	SIMD_FORCEINLINE explicit constexpr simd_pack_t(T value) : x(value), y(value) {}
	SIMD_FORCEINLINE constexpr simd_pack_t(T x_, T y_) : x(x_), y(y_) {}
	template <typename T2>
	SIMD_FORCEINLINE explicit constexpr simd_pack_t(const simd_pack_t<2, T2>& v) : x(T(v.x)), y(T(v.y)) {}

	SIMD_FORCEINLINE T& channel(int index) { assert(index >= 0 && index < 2); return index == 0 ? x : y; }
	SIMD_FORCEINLINE const T& channel(int index) const { assert(index >= 0 && index < 2); return index == 0 ? x : y; }

	SIMD_FORCEINLINE T& operator[](int index) { return channel(index); }
	SIMD_FORCEINLINE const T& operator[](int index) const { return channel(index); }
};

// -------- D = 3
template <typename T>
struct simd_pack_t<3, T> {
	union {
		struct { T x, y, z; };
		typename simd_internal::reg<T, 3>::type_t m; // siempre definido (wrapper trivial)
	};

	SIMD_FORCEINLINE explicit constexpr simd_pack_t(typename simd_internal::reg<T, 3>::type_t value) : m(value) {}
	SIMD_FORCEINLINE explicit constexpr simd_pack_t(T value) : x(value), y(value), z(value) {}
	SIMD_FORCEINLINE constexpr simd_pack_t(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
	SIMD_FORCEINLINE constexpr simd_pack_t(const simd_pack_t<2, T>& v, T z_) : x(v.x), y(v.y), z(z_) {}
	template <typename T2>
	SIMD_FORCEINLINE explicit constexpr simd_pack_t(const simd_pack_t<3, T2>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}

	SIMD_FORCEINLINE static constexpr int component_count() { return 3; }
	SIMD_FORCEINLINE T& channel(int index) { assert(index >= 0 && index < 3); return (&x)[index]; }
	SIMD_FORCEINLINE const T& channel(int index) const { assert(index >= 0 && index < 3); return (&x)[index]; }

	SIMD_FORCEINLINE T& operator[](int index) { return channel(index); }
	SIMD_FORCEINLINE const T& operator[](int index) const { return channel(index); }
};

// -------- D = 4
template <typename T>
struct simd_pack_t<4, T> {
	union {
		struct { T x, y, z, w; };
		typename simd_internal::reg<T, 4>::type_t m;
	};

	SIMD_FORCEINLINE explicit constexpr
		simd_pack_t(typename simd_internal::reg<T, 4>::type_t value) : m(value) {}
	SIMD_FORCEINLINE explicit constexpr simd_pack_t(T value) : x(value), y(value), z(value), w(value) {}
	SIMD_FORCEINLINE constexpr simd_pack_t(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
	SIMD_FORCEINLINE constexpr simd_pack_t(const simd_pack_t<3, T>& v, T w_) : x(v.x), y(v.y), z(v.z), w(w_) {}
	template <typename T2>
	SIMD_FORCEINLINE explicit constexpr simd_pack_t(const simd_pack_t<4, T2>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}

	SIMD_FORCEINLINE static constexpr int component_count() { return 4; }
	SIMD_FORCEINLINE T& channel(int index) { assert(index >= 0 && index < 4); return (&x)[index]; }
	SIMD_FORCEINLINE const T& channel(int index) const { assert(index >= 0 && index < 4); return (&x)[index]; }

	SIMD_FORCEINLINE T& operator[](int index) { return channel(index); }
	SIMD_FORCEINLINE const T& operator[](int index) const { return channel(index); }
};


// -------- D = 8
template <typename T>
struct simd_pack_t<8, T> {
    union {
        struct { T v[8]; }; // acceso como array simple
        typename simd_internal::reg<T, 8>::type_t m;
    };

    SIMD_FORCEINLINE explicit constexpr
        simd_pack_t(typename simd_internal::reg<T, 8>::type_t value) : m(value) {}
    SIMD_FORCEINLINE explicit constexpr simd_pack_t(T value) {
        for (int i = 0; i < 8; ++i) v[i] = value;
    }
    SIMD_FORCEINLINE constexpr simd_pack_t(
        T v0,T v1,T v2,T v3,T v4,T v5,T v6,T v7
    ) : v{v0,v1,v2,v3,v4,v5,v6,v7} {}
    template <typename T2>
    SIMD_FORCEINLINE explicit constexpr simd_pack_t(const simd_pack_t<8, T2>& other) {
        for (int i = 0; i < 8; ++i) v[i] = T(other.v[i]);
    }

    SIMD_FORCEINLINE static constexpr int component_count() { return 8; }
    SIMD_FORCEINLINE T&       channel(int index)       { assert(index >= 0 && index < 8); return v[index]; }
    SIMD_FORCEINLINE const T& channel(int index) const { assert(index >= 0 && index < 8); return v[index]; }

    SIMD_FORCEINLINE T&       operator[](int index)       { return channel(index); }
    SIMD_FORCEINLINE const T& operator[](int index) const { return channel(index); }
};




// ===============================
//  Constantes y helpers (sin <cmath>)
// ===============================
namespace simd {

	template<typename T> struct _pi_;
	template<> struct _pi_<float> { static constexpr float  v = 3.14159265358979323846f; };
	template<> struct _pi_<double> { static constexpr double v = 3.141592653589793238462643383279502884; };

	template<typename T> struct _half_pi_;
	template<> struct _half_pi_<float> { static constexpr float  v = 1.57079632679489661923f; };
	template<> struct _half_pi_<double> { static constexpr double v = 1.57079632679489661923132169163975144; };

	template<typename T> struct _two_pi_;
	template<> struct _two_pi_<float> { static constexpr float  v = 2.0f * _pi_<float>::v; };
	template<> struct _two_pi_<double> { static constexpr double v = 2.0 * _pi_<double>::v; };

	template<typename T> struct _cst_;
	template<> struct _cst_<float> {
		static constexpr float ln2 = 0.69314718055994530941723212145818f;
		static constexpr float inv_ln2 = 1.4426950408889634073599246810019f;  // log2(e)
		static constexpr float ln2_hi = 0.6931457519f;     // split para reducción
		static constexpr float ln2_lo = 1.4286067653e-6f;
	};
	template<> struct _cst_<double> {
		static constexpr double ln2 = 0.693147180559945309417232121458176568;
		static constexpr double inv_ln2 = 1.44269504088896340735992468100189214; // log2(e)
		static constexpr double ln2_hi = 0.6931471805596629; // split (Cody–Waite)
		static constexpr double ln2_lo = 2.384185791015625e-07;
	};

	// splat genérico
	template<int D, typename T>		SIMD_FORCEINLINE constexpr simd_pack_t<D, T> splat(T v) { return simd_pack_t<D, T>(v); }

	template<class To, class From>	SIMD_FORCEINLINE To _bit_cast(const From& src) {return std::bit_cast<To>(src);}

	SIMD_FORCEINLINE int clampi(int v, int lo, int hi) {
		return v < lo ? lo : (v > hi ? hi : v);
	}
} // namespace simd


// ===============================
//  Macros utilitarias
// ===============================

// Aplicación escalar por componente (sin bucle)
#define SIMD_APPLY2(F, value) F((value).x), F((value).y)
#define SIMD_APPLY3(F, value) F((value).x), F((value).y), F((value).z)
#define SIMD_APPLY4(F, value) F((value).x), F((value).y), F((value).z), F((value).w)

// Helper de concatenación
#define SIMD_CAT(a, b) a##b
#define SIMD_DISPATCH_APPLY(dim, F, value) SIMD_CAT(SIMD_APPLY, dim)(F, value)

// Genera especialización de función vectorial usando función escalar
#define SIMD_GEN_EXPAND_PARTIAL(dim, type, func, scalarfunc)                                   \
template<> SIMD_FORCEINLINE constexpr simd_pack_t<dim, type>                                   \
func<dim, type>(const simd_pack_t<dim, type>& value) {                                         \
  return simd_pack_t<dim, type>( SIMD_DISPATCH_APPLY(dim, scalarfunc, value) );                \
}

// Genera para D = 4,3,2
#define SIMD_GEN_EXPAND(type, func, scalarfunc) \
  SIMD_GEN_EXPAND_PARTIAL(4, type, func, scalarfunc) \
  SIMD_GEN_EXPAND_PARTIAL(3, type, func, scalarfunc) \
  SIMD_GEN_EXPAND_PARTIAL(2, type, func, scalarfunc)

// Binarios por componente
// Binarias pack-pack
#define SIMD_APPLY2_BIN(OP, A, B) OP((A).x, (B).x), OP((A).y, (B).y)
#define SIMD_APPLY3_BIN(OP, A, B) OP((A).x, (B).x), OP((A).y, (B).y), OP((A).z, (B).z)
#define SIMD_APPLY4_BIN(OP, A, B) OP((A).x, (B).x), OP((A).y, (B).y), OP((A).z, (B).z), OP((A).w, (B).w)

// Pack con escalar (left scalar / right scalar)
#define SIMD_APPLY2_LS(OP, A, s)  OP((A).x, (s)), OP((A).y, (s))
#define SIMD_APPLY3_LS(OP, A, s)  OP((A).x, (s)), OP((A).y, (s)), OP((A).z, (s))
#define SIMD_APPLY4_LS(OP, A, s)  OP((A).x, (s)), OP((A).y, (s)), OP((A).z, (s)), OP((A).w, (s))

#define SIMD_APPLY2_SL(OP, s, B)  OP((s), (B).x), OP((s), (B).y)
#define SIMD_APPLY3_SL(OP, s, B)  OP((s), (B).x), OP((s), (B).y), OP((s), (B).z)
#define SIMD_APPLY4_SL(OP, s, B)  OP((s), (B).x), OP((s), (B).y), OP((s), (B).z), OP((s), (B).w)

// Selección por dimensión (arreglado: sin espacios erróneos)
#define SIMD_PP_CAT(a,b) a##b
#define SIMD_DISPATCH_BIN(dim, OP, A, B) SIMD_PP_CAT(SIMD_APPLY, dim)_BIN(OP, A, B)
#define SIMD_DISPATCH_LS(dim, OP, A, s)  SIMD_PP_CAT(SIMD_APPLY, dim)_LS (OP, A, s)
#define SIMD_DISPATCH_SL(dim, OP, s, B)  SIMD_PP_CAT(SIMD_APPLY, dim)_SL (OP, s, B)

// Generador de ops básicas (a OP b, a OP escalar, escalar OP b)
#define SIMD_GEN_BASIC_OP_PARTIAL(dim, func, OP, type)                                          \
template<> SIMD_FORCEINLINE constexpr simd_pack_t<dim, type>                                    \
func<dim, type>(const simd_pack_t<dim, type>& a, const simd_pack_t<dim, type>& b) noexcept {    \
  return simd_pack_t<dim, type>( SIMD_DISPATCH_BIN(dim, OP, a, b) );                            \
}                                                                                                \
template<> SIMD_FORCEINLINE constexpr simd_pack_t<dim, type>                                    \
func<dim, type>(const simd_pack_t<dim, type>& a, const type& b) noexcept {                      \
  return simd_pack_t<dim, type>( SIMD_PP_CAT(SIMD_APPLY, dim)_LS(OP, a, b) );                   \
}                                                                                                \
template<> SIMD_FORCEINLINE constexpr simd_pack_t<dim, type>                                    \
func<dim, type>(const type& a, const simd_pack_t<dim, type>& b) noexcept {                      \
  return simd_pack_t<dim, type>( SIMD_PP_CAT(SIMD_APPLY, dim)_SL(OP, a, b) );                   \
}

// Genera para D = 4,3,2
#define SIMD_GEN_BASIC_OP(type, func, OP) \
  SIMD_GEN_BASIC_OP_PARTIAL(4, func, OP, type) \
  SIMD_GEN_BASIC_OP_PARTIAL(3, func, OP, type) \
  SIMD_GEN_BASIC_OP_PARTIAL(2, func, OP, type)

