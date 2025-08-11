#pragma once

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
  #include <immintrin.h>   // AVX (también expone SSE/AVX en compilers modernos)
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


// ===============================
//  Infra interna
// ===============================
namespace simd_internal {

  // Registro por (tipo, dimensión). Por defecto: array escalar (fallback portable).
  template<typename T, int D>
  struct reg { using type_t = T[D]; };

  // =========================
  //  Especializaciones ISA
  // =========================

  // ---- NEON (ARMv7/v8)
  #if defined(__ARM_NEON) || defined(__ARM_NEON__)
    template<> struct reg<float, 2> { using type_t = float32x2_t; };
    template<> struct reg<float, 4> { using type_t = float32x4_t; };
    template<> struct reg<int,   4> { using type_t = int32x4_t;   };
    template<> struct reg<unsigned, 4> { using type_t = uint32x4_t; };
  #endif

  // NEON con double: seguro en AArch64; en ARMv7 sólo si FP64
  #if defined(__aarch64__) || (defined(__ARM_NEON) && defined(__ARM_FEATURE_FP64))
    template<> struct reg<double, 2> { using type_t = float64x2_t; };
  #endif

  // ---- SSE2 (x86/x64)
  #if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    template<> struct reg<float, 4>  { using type_t = __m128;  };   // 4xfloat
    template<> struct reg<int,   4>  { using type_t = __m128i; };   // 4xint32
    template<> struct reg<double, 2> { using type_t = __m128d; };   // 2xdouble (SSE2)
    // Nota: si quieres "vec2<float>" en SSE, usa __m128 y ocupa sólo 2 lanes bajas.
  #endif

  // ---- AVX / AVX2 (extensiones útiles de ejemplo)
  #if defined(__AVX__) || defined(_M_AVX)
    // 4xdouble existe con AVX: __m256d. No redefinimos reg<> aquí porque D=4<double>
    // suele mapear a __m256d, pero tu interfaz actual define reg<double,2>.
    // Puedes crear otra instanciación si la necesitas:
    struct avx_dvec4 { using type_t = __m256d; };
  #endif

  #if defined(__AVX2__) || defined(_M_AVX2)
    struct avx2_i64x4 { using type_t = __m256i; };
  #endif

  // ---- Fallbacks (cuando NO hay intrínsecos)
  // GCC/Clang vector extensions: mejor performance que arrays si no hay ISA arriba
  #if !defined(__ARM_NEON) && !defined(__ARM_NEON__) \
   && !defined(__SSE2__) && !defined(_M_X64) && !(defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    #if defined(__GNUC__) || defined(__clang__)
      template<> struct reg<float, 2>  { using type_t = float  __attribute__((__vector_size__(8),  __may_alias__));  };
      template<> struct reg<float, 4>  { using type_t = float  __attribute__((__vector_size__(16), __may_alias__));  };
      template<> struct reg<int,   4>  { using type_t = int    __attribute__((__vector_size__(16), __may_alias__));  };
      template<> struct reg<double, 2> { using type_t = double __attribute__((__vector_size__(16), __may_alias__));  };
    #endif
  #endif

  // Si tampoco hay GCC vector extensions (p. ej. MSVC sin intrínsecos habilitados), nos quedamos con el array por defecto.

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

  simd_pack_t() : x(T{}), y(T{}) {}
  explicit simd_pack_t(T v) : x(v), y(v) {}
  simd_pack_t(T ix, T iy) : x(ix), y(iy) {}
  explicit simd_pack_t(typename simd_internal::reg<T, 2>::type_t r) : m(r) {}

  template <typename T2>
  explicit simd_pack_t(const simd_pack_t<2, T2>& v) : x(T(v.x)), y(T(v.y)) {}

  T&       channel(int i)       { assert(i>=0 && i<2); return i==0? x : y; }
  const T& channel(int i) const { assert(i>=0 && i<2); return i==0? x : y; }

  T& operator[](int i)       { return channel(i); }
  const T& operator[](int i) const { return channel(i); }
};

// -------- D = 3
template <typename T>
struct simd_pack_t<3, T> {
  union {
    struct { T x, y, z; };
    typename simd_internal::reg<T, 3>::type_t m; // cae al array por defecto (o vector_size si fallback)
  };
  static constexpr int component_count() { return 3; }

  simd_pack_t() : x(T{}), y(T{}), z(T{}) {}
  explicit simd_pack_t(T v) : x(v), y(v), z(v) {}
  simd_pack_t(T ix, T iy, T iz) : x(ix), y(iy), z(iz) {}
  explicit simd_pack_t(typename simd_internal::reg<T, 3>::type_t r) : m(r) {}
  simd_pack_t(const simd_pack_t<2, T>& v, T iz) : x(v.x), y(v.y), z(iz) {}

  template <typename T2>
  explicit simd_pack_t(const simd_pack_t<3, T2>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}

  T&       channel(int i)       { assert(i>=0 && i<3); return (&x)[i]; }
  const T& channel(int i) const { assert(i>=0 && i<3); return (&x)[i]; }

  T& operator[](int i)       { return channel(i); }
  const T& operator[](int i) const { return channel(i); }
};

// -------- D = 4
template <typename T>
struct simd_pack_t<4, T> {
  union {
    struct { T x, y, z, w; };
    typename simd_internal::reg<T, 4>::type_t m; // según ISA puede ser SIMD o array / vector_size
  };
  static constexpr int component_count() { return 4; }

  simd_pack_t() : x(T{}), y(T{}), z(T{}), w(T{}) {}
  explicit simd_pack_t(T v) : x(v), y(v), z(v), w(v) {}
  simd_pack_t(T ix, T iy, T iz, T iw) : x(ix), y(iy), z(iz), w(iw) {}
  simd_pack_t(const simd_pack_t<3, T>& v, T iw) : x(v.x), y(v.y), z(v.z), w(iw) {}
  explicit simd_pack_t(typename simd_internal::reg<T, 4>::type_t r) : m(r) {}

  template <typename T2>
  explicit simd_pack_t(const simd_pack_t<4, T2>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}

  T&       channel(int i)       { assert(i>=0 && i<4); return (&x)[i]; }
  const T& channel(int i) const { assert(i>=0 && i<4); return (&x)[i]; }

  T& operator[](int i)       { return channel(i); }
  const T& operator[](int i) const { return channel(i); }
};