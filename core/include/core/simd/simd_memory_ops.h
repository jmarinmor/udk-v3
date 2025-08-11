// simd_memory_ops.h
#pragma once
#include <simd/simd_types.h>

namespace simd {

  // ---------------- Fallback por-lane ----------------
  template<int D>
  SIMD_FORCEINLINE simd_pack_t<D,float> gather(const float* base, const int* idx){
    simd_pack_t<D,float> r(0.f);
    for (int i=0;i<D;++i) (&r.x)[i] = base[idx[i]];
    return r;
  }
  template<int D>
  SIMD_FORCEINLINE simd_pack_t<D,double> gather(const double* base, const int* idx){
    simd_pack_t<D,double> r(0.0);
    for (int i=0;i<D;++i) (&r.x)[i] = base[idx[i]];
    return r;
  }

  template<int D>
  SIMD_FORCEINLINE void scatter(float* base, const int* idx, const simd_pack_t<D,float>& v){
    for (int i=0;i<D;++i) base[idx[i]] = (&v.x)[i];
  }
  template<int D>
  SIMD_FORCEINLINE void scatter(double* base, const int* idx, const simd_pack_t<D,double>& v){
    for (int i=0;i<D;++i) base[idx[i]] = (&v.x)[i];
  }

  template<int D>
  SIMD_FORCEINLINE simd_pack_t<D,float> masked_load(const float* p, const simd_pack_t<D,float>& m01, float fallback = 0.f){
    simd_pack_t<D,float> r(fallback);
    for (int i=0;i<D;++i) if ((&m01.x)[i] != 0.0f) (&r.x)[i] = p[i];
    return r;
  }
  template<int D>
  SIMD_FORCEINLINE simd_pack_t<D,double> masked_load(const double* p, const simd_pack_t<D,double>& m01, double fallback = 0.0){
    simd_pack_t<D,double> r(fallback);
    for (int i=0;i<D;++i) if ((&m01.x)[i] != 0.0) (&r.x)[i] = p[i];
    return r;
  }

  template<int D>
  SIMD_FORCEINLINE void masked_store(float* p, const simd_pack_t<D,float>& m01, const simd_pack_t<D,float>& v){
    for (int i=0;i<D;++i) if ((&m01.x)[i] != 0.0f) p[i] = (&v.x)[i];
  }
  template<int D>
  SIMD_FORCEINLINE void masked_store(double* p, const simd_pack_t<D,double>& m01, const simd_pack_t<D,double>& v){
    for (int i=0;i<D;++i) if ((&m01.x)[i] != 0.0) p[i] = (&v.x)[i];
  }

  template<int D>
  SIMD_FORCEINLINE void stream_store(float* p, const simd_pack_t<D,float>& v){
    // Fallback: almacena normal
    for (int i=0;i<D;++i) p[i] = (&v.x)[i];
  }
  template<int D>
  SIMD_FORCEINLINE void stream_store(double* p, const simd_pack_t<D,double>& v){
    for (int i=0;i<D;++i) p[i] = (&v.x)[i];
  }

} // namespace simd

// ==================== ESPECIALIZACIONES SIMD ====================
// AVX2 tiene gather; scatter no (hasta AVX-512)
#if defined(__AVX2__) || defined(_M_AVX2)
template<> SIMD_FORCEINLINE simd_pack_t<8,float>  simd::gather<8>(const float* base,  const int* idx){
  return simd_pack_t<8,float>(_mm256_i32gather_ps(base, _mm256_loadu_si256((const __m256i*)idx), 4));
}
template<> SIMD_FORCEINLINE simd_pack_t<4,double> simd::gather<4>(const double* base, const int* idx){
  return simd_pack_t<4,double>(_mm256_i32gather_pd(base, _mm_loadu_si128((const __m128i*)idx), 8));
}
#endif

#if defined(__AVX__) || defined(_M_AVX)
// stream_store AVX
template<> SIMD_FORCEINLINE void simd::stream_store<8>(float* p,  const simd_pack_t<8,float>& v){ _mm256_stream_ps(p, v.m); }
template<> SIMD_FORCEINLINE void simd::stream_store<4>(double* p, const simd_pack_t<4,double>& v){ _mm256_stream_pd(p, v.m); }
#endif

#if defined(__SSE2__) || defined(_M_X64)
template<> SIMD_FORCEINLINE void simd::stream_store<4>(float* p,  const simd_pack_t<4,float>& v){ _mm_stream_ps(p, v.m); }
template<> SIMD_FORCEINLINE void simd::stream_store<2>(double* p, const simd_pack_t<2,double>& v){ _mm_stream_pd(p, v.m); }
#endif

// NEON: no hay stream store estándar; usamos fallback (ya definido)
