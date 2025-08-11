#pragma once

#include <simd/simd_types.h>

namespace simd
{
		inline int floatBitsToInt(const float& v) noexcept {
		return std::bit_cast<int>(v);
	}

	inline unsigned int floatBitsToUint(const float& v) noexcept {
		return std::bit_cast<unsigned int>(v);
	}

	inline float intBitsToFloat(const int& v) noexcept {
		return std::bit_cast<float>(v);
	}

	inline float uintBitsToFloat(const unsigned int& v) noexcept {
		return std::bit_cast<float>(v);
	}

	template<typename To, typename From>
	SIMD_FORCEINLINE To _bit_cast(const From& src) noexcept {
		static_assert(sizeof(To) == sizeof(From), "bit_cast: tamaños incompatibles");
		To dst; std::memcpy(&dst, &src, sizeof(To)); return dst;
	}


// ---- pack_saturate<int16 <- int32> ----
template<int D>
SIMD_FORCEINLINE simd_pack_t<D,int16_t>
pack_saturate(const simd_pack_t<D,int32_t>& a){
  simd_pack_t<D,int16_t> r(int16_t{0});
  for (int i=0;i<D;++i){
    int32_t v = (&a.x)[i];
    if (v >  32767) v =  32767;
    if (v < -32768) v = -32768;
    (&r.x)[i] = (int16_t)v;
  }
  return r;
}

// ---- pack_saturate<uint8 <- float> (round-to-nearest, clamp [0,255]) ----
template<int D>
SIMD_FORCEINLINE simd_pack_t<D,uint8_t>
pack_saturate(const simd_pack_t<D,float>& a){
  simd_pack_t<D,uint8_t> r(uint8_t{0});
  for (int i=0;i<D;++i){
    float v = (&a.x)[i];
    int iv = (int)(v + (v >= 0.f ? 0.5f : -0.5f));
    if (iv <   0) iv = 0;
    if (iv > 255) iv = 255;
    (&r.x)[i] = (uint8_t)iv;
  }
  return r;
}

// ---- float <-> half (escalar) ----
SIMD_FORCEINLINE uint16_t float_to_half(float f){
  uint32_t x = _bit_cast<uint32_t>(f);
  uint32_t sign = (x >> 31) & 1u, exp = (x >> 23) & 0xFFu, man = x & 0x7FFFFFu;
  if (exp == 0xFFu){ // Inf/NaN
    uint16_t h = (uint16_t)((sign<<15) | (0x1Fu<<10) | (man?0x200u:0u));
    return h;
  }
  if (exp == 0){ // cero/subnormal float
    if (!man) return (uint16_t)(sign<<15);
    int shift=0; while ((man & 0x800000u)==0u){ man<<=1; ++shift; if (shift>24) break; }
    int e = 1 - 127 - shift; int he = e + 15;
    if (he <= 0){
      if (he < -10) return (uint16_t)(sign<<15);
      uint32_t m = (0x800000u | (man & 0x7FFFFFu)) >> (1 - he + 13);
      return (uint16_t)((sign<<15) | (m & 0x3FFu));
    } else {
      uint16_t mant = (uint16_t)((man & 0x7FFFFFu) >> 13);
      return (uint16_t)((sign<<15) | (uint16_t(he)<<10) | mant);
    }
  }
  int e = int(exp) - 127; int he = e + 15;
  if (he >= 0x1F) return (uint16_t)((sign<<15) | (0x1Fu<<10)); // overflow->inf
  if (he <= 0){
    if (he < -10) return (uint16_t)(sign<<15);
    uint32_t m = (0x800000u | man) >> (1 - he + 13);
    return (uint16_t)((sign<<15) | (m & 0x3FFu));
  }
  uint32_t mant = man >> 13;
  if ((man & 0x1000u) && (((man & 0x1FFFu)!=0u) || (mant & 1u))) {
    ++mant; if (mant==0x400u){ mant=0; ++he; if (he>=0x1F) { he=0x1F; mant=0; } }
  }
  return (uint16_t)((sign<<15) | (uint16_t(he)<<10) | uint16_t(mant));
}
SIMD_FORCEINLINE float half_to_float(uint16_t h){
  uint32_t sign=(h>>15)&1u, exp=(h>>10)&0x1Fu, man=h&0x3FFu;
  uint32_t out;
  if (exp==0){
    if (!man) out = sign<<31;
    else{
      int shift=0; while ((man & 0x400u)==0u){ man<<=1; ++shift; }
      man &= 0x3FFu; int e = 1 - 15 - shift;
      out = (sign<<31) | (uint32_t(e+127)<<23) | (man<<13);
    }
  } else if (exp==0x1F){
    out = (sign<<31) | (0xFFu<<23) | (man ? ((man<<13)|0x400000u) : 0u);
  } else {
    out = (sign<<31) | (uint32_t(exp-15+127)<<23) | (man<<13);
  }
  return _bit_cast<float>(out);
}

// ---- packs: float<->half por-lane ----
template<int D>
SIMD_FORCEINLINE simd_pack_t<D,uint16_t>
float_to_half(const simd_pack_t<D,float>& a){
  simd_pack_t<D,uint16_t> r(uint16_t{0});
  for (int i=0;i<D;++i) (&r.x)[i] = float_to_half((&a.x)[i]);
  return r;
}
template<int D>
SIMD_FORCEINLINE simd_pack_t<D,float>
half_to_float(const simd_pack_t<D,uint16_t>& a){
  simd_pack_t<D,float> r(0.f);
  for (int i=0;i<D;++i) (&r.x)[i] = half_to_float((&a.x)[i]);
  return r;
}
}

namespace simd {


// ======================================
// 9) SWIZZLES Y PERMUTES
//  - shuffle<X,Y,Z,W>(float4/double4) fallback + patrones comunes
//  - permute runtime: con int idx[4] y con simd_pack_t<4,int>
// ======================================

// Fallback genérico (X..W ∈ [0..3])
template<typename T, int X, int Y, int Z, int W>
SIMD_FORCEINLINE simd_pack_t<4,T>
shuffle(const simd_pack_t<4,T>& a){
  static_assert(0<=X && X<4 && 0<=Y && Y<4 && 0<=Z && Z<4 && 0<=W && W<4, "shuffle: index out of range");
  const T lanes[4] = { a.x, a.y, a.z, a.w };
  return simd_pack_t<4,T>( lanes[X], lanes[Y], lanes[Z], lanes[W] );
}

// Patrones comunes
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> dup_x(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.x,a.x,a.x,a.x); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> dup_y(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.y,a.y,a.y,a.y); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> dup_z(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.z,a.z,a.z,a.z); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> dup_w(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.w,a.w,a.w,a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> swap_xy(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.y,a.x,a.z,a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> swap_zw(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.x,a.y,a.w,a.z); }

// Permute runtime (array de 4 índices)
template<typename T>
SIMD_FORCEINLINE simd_pack_t<4,T>
permute(const simd_pack_t<4,T>& a, const int idx[4]){
  const T lanes[4]={a.x,a.y,a.z,a.w};
  return simd_pack_t<4,T>( lanes[clampT(idx[0],0,3)],
                           lanes[clampT(idx[1],0,3)],
                           lanes[clampT(idx[2],0,3)],
                           lanes[clampT(idx[3],0,3)] );
}
// Permute runtime (pack de índices)
template<typename T>
SIMD_FORCEINLINE simd_pack_t<4,T>
permute(const simd_pack_t<4,T>& a, const simd_pack_t<4,int>& idx){
  const T lanes[4]={a.x,a.y,a.z,a.w};
  return simd_pack_t<4,T>( lanes[clampT(idx.x,0,3)],
                           lanes[clampT(idx.y,0,3)],
                           lanes[clampT(idx.z,0,3)],
                           lanes[clampT(idx.w,0,3)] );
}

} // namespace simd




namespace simd
{
// =====================================================================
// ESPECIALIZACIONES SIMD (bloques fuera de las funciones)
// =====================================================================

// ---------- pack_saturate y float<->half ----------
#if defined(__SSE2__) || defined(_M_X64)
template<> SIMD_FORCEINLINE simd_pack_t<4,uint8_t>
simd::pack_saturate<4>(const simd_pack_t<4,float>& a){
  __m128  x   = a.m;
  __m128i i32 = _mm_cvtps_epi32(x);
  __m128i i16 = _mm_packs_epi32(i32, _mm_setzero_si128());
  __m128i u8  = _mm_packus_epi16(i16, _mm_setzero_si128());
  alignas(16) uint8_t out[16];
  _mm_store_si128((__m128i*)out, u8);
  return simd_pack_t<4,uint8_t>( out[0], out[1], out[2], out[3] );
}
template<> SIMD_FORCEINLINE simd_pack_t<4,int16_t>
simd::pack_saturate<4>(const simd_pack_t<4,int32_t>& a){
  __m128i i16 = _mm_packs_epi32(a.m, _mm_setzero_si128());
  alignas(16) int16_t out[8];
  _mm_store_si128((__m128i*)out, i16);
  return simd_pack_t<4,int16_t>( out[0], out[1], out[2], out[3] );
}
#endif

#if defined(__F16C__)
template<> SIMD_FORCEINLINE simd_pack_t<4,uint16_t>
simd::float_to_half<4>(const simd_pack_t<4,float>& a){
  __m128  x  = a.m;
  __m128i h4 = _mm_cvtps_ph(x, 0); // RNE
  alignas(16) uint16_t out[8];
  _mm_store_si128((__m128i*)out, h4);
  return simd_pack_t<4,uint16_t>( out[0], out[1], out[2], out[3] );
}
template<> SIMD_FORCEINLINE simd_pack_t<4,float>
simd::half_to_float<4>(const simd_pack_t<4,uint16_t>& a){
  alignas(16) uint16_t in[8] = { a.x,a.y,a.z,a.w,0,0,0,0 };
  __m128  f4 = _mm_cvtph_ps( _mm_load_si128((const __m128i*)in) );
  return simd_pack_t<4,float>(f4);
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
template<> SIMD_FORCEINLINE simd_pack_t<4,uint8_t>
simd::pack_saturate<4>(const simd_pack_t<4,float>& a){
  int32x4_t i32 = vcvtq_s32_f32(a.m);
  uint16x4_t u16= vqmovun_s32(i32);
  uint8x8_t  u8 = vqmovn_u16(vcombine_u16(u16, vdup_n_u16(0)));
  return simd_pack_t<4,uint8_t>( vget_lane_u8(u8,0), vget_lane_u8(u8,1),
                                 vget_lane_u8(u8,2), vget_lane_u8(u8,3) );
}
template<> SIMD_FORCEINLINE simd_pack_t<4,int16_t>
simd::pack_saturate<4>(const simd_pack_t<4,int32_t>& a){
  int16x4_t i16 = vqmovn_s32(a.m);
  return simd_pack_t<4,int16_t>( vget_lane_s16(i16,0), vget_lane_s16(i16,1),
                                 vget_lane_s16(i16,2), vget_lane_s16(i16,3) );
}
#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC__)
template<> SIMD_FORCEINLINE simd_pack_t<4,uint16_t>
simd::float_to_half<4>(const simd_pack_t<4,float>& a){
  float16x4_t h = vcvt_f16_f32(a.m);
  return simd_pack_t<4,uint16_t>( vget_lane_u16(vreinterpret_u16_f16(h),0),
                                  vget_lane_u16(vreinterpret_u16_f16(h),1),
                                  vget_lane_u16(vreinterpret_u16_f16(h),2),
                                  vget_lane_u16(vreinterpret_u16_f16(h),3) );
}
template<> SIMD_FORCEINLINE simd_pack_t<4,float>
simd::half_to_float<4>(const simd_pack_t<4,uint16_t>& a){
  uint64_t pack = (uint64_t)a.x | ((uint64_t)a.y<<16) | ((uint64_t)a.z<<32) | ((uint64_t)a.w<<48);
  float16x4_t h = vreinterpret_f16_u16( vcreate_u16(pack) );
  return simd_pack_t<4,float>( vcvt_f32_f16(h) );
}
#endif
#endif // NEON

// ---------- Swizzles / Permutes ----------
#if defined(__SSE__) || defined(__SSE2__) || defined(_M_X64)
// Ejemplos de patrones: reverse para float4
template<> inline simd::simd_pack_t<4,float>
simd::shuffle<float,3,2,1,0>(const simd_pack_t<4,float>& a){
  return simd_pack_t<4,float>(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(0,1,2,3)));
}
template<> inline simd::simd_pack_t<4,float>
simd::dup_x<float>(const simd_pack_t<4,float>& a){
  __m128 r = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(0,0,0,0));
  return simd_pack_t<4,float>(r);
}
template<> inline simd::simd_pack_t<4,float>
simd::dup_y<float>(const simd_pack_t<4,float>& a){
  __m128 r = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(1,1,1,1));
  return simd_pack_t<4,float>(r);
}
template<> inline simd::simd_pack_t<4,float>
simd::swap_xy<float>(const simd_pack_t<4,float>& a){
  __m128 r = _mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(3,2,0,1));
  return simd_pack_t<4,float>(r);
}
#endif

#if defined(__AVX__) || defined(_M_AVX)
// Permute runtime (float4) con _mm_permutevar_ps (AVX)
template<> inline simd::simd_pack_t<4,float>
simd::permute<float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,int>& idx){
  __m128i m = _mm_set_epi32(idx.w&3, idx.z&3, idx.y&3, idx.x&3);
  return simd_pack_t<4,float>(_mm_permutevar_ps(a.m, m));
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
// Duplicados con NEON
template<> inline simd::simd_pack_t<4,float>
simd::dup_x<float>(const simd_pack_t<4,float>& a){ return simd_pack_t<4,float>( vdupq_laneq_f32(a.m,0) ); }
template<> inline simd::simd_pack_t<4,float>
simd::dup_y<float>(const simd_pack_t<4,float>& a){ return simd_pack_t<4,float>( vdupq_laneq_f32(a.m,1) ); }
template<> inline simd::simd_pack_t<4,float>
simd::swap_xy<float>(const simd_pack_t<4,float>& a){
  float32x4_t r = vshuffle_f32(a.m, 1,0,2,3); // helper si lo tienes; sino:
  float32x4_t v = a.m;
  r = vcombine_f32(vget_high_f32(v), vget_low_f32(v)); // simple ejemplo
  return simd_pack_t<4,float>(r);
}
#if defined(__aarch64__)
// Permute runtime con vtbl (AArch64: vqtbl1q_u8)
template<> inline simd::simd_pack_t<4,float>
simd::permute<float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,int>& idx){
  uint8x16_t bytes = vreinterpretq_u8_f32(a.m);
  uint8x16_t lut   = bytes;
  // construir máscara de 16 bytes a partir de idx (cada lane => 4 bytes)
  uint8_t m[16]; for(int i=0;i<4;++i){ uint8_t base = uint8_t(((&idx.x)[i]&3)*4); m[i*4+0]=base+0; m[i*4+1]=base+1; m[i*4+2]=base+2; m[i*4+3]=base+3; }
  uint8x16_t sel = vld1q_u8(m);
  float32x4_t r  = vreinterpretq_f32_u8( vqtbl1q_u8(lut, sel) );
  return simd_pack_t<4,float>(r);
}
#endif
#endif // NEON	
}


namespace simd
{
	// ================== 6) Conversiones y empaquetado ==================
// ---- pack_saturate<int16 <- int32> ----
template<int D>
SIMD_FORCEINLINE simd_pack_t<D,int16_t> pack_saturate(const simd_pack_t<D,int32_t>& a){
  simd_pack_t<D,int16_t> r(int16_t{0});
  for (int i=0;i<D;++i){
    int32_t v = (&a.x)[i];
    if (v >  32767) v =  32767;
    if (v < -32768) v = -32768;
    (&r.x)[i] = (int16_t)v;
  }
  return r;
}

// ---- pack_saturate<uint8 <- float> (round-to-nearest, clamp [0..255]) ----
template<int D>
SIMD_FORCEINLINE simd_pack_t<D,uint8_t> pack_saturate(const simd_pack_t<D,float>& a){
  simd_pack_t<D,uint8_t> r(uint8_t{0});
  for (int i=0;i<D;++i){
    float v = (&a.x)[i];
    int iv = (int)(v + (v>=0.f ? 0.5f : -0.5f));
    if (iv < 0) iv = 0; else if (iv > 255) iv = 255;
    (&r.x)[i] = (uint8_t)iv;
  }
  return r;
}

// ---- float <-> half (escalar) ----
SIMD_FORCEINLINE uint16_t float_to_half(float f){
  uint32_t x = _bit_cast<uint32_t>(f);
  uint32_t s = (x>>31)&1u, e=(x>>23)&0xFFu, m=x&0x7FFFFFu;
  if (e==0xFF){ // Inf/NaN
    return (uint16_t)((s<<15) | (0x1Fu<<10) | (m?0x200u:0u));
  }
  if (e==0){
    if (!m) return (uint16_t)(s<<15);
    int sh=0; while((m&0x800000u)==0u){m<<=1;++sh; if(sh>24)break;}
    int E = 1-127-sh, he = E+15;
    if (he<=0){ if(he<-10) return (uint16_t)(s<<15);
      uint32_t mm=(0x800000u|(m&0x7FFFFFu))>>(1-he+13);
      return (uint16_t)((s<<15)|(mm&0x3FFu));
    } else {
      uint16_t mm=(uint16_t)((m&0x7FFFFFu)>>13);
      return (uint16_t)((s<<15)|((uint16_t)he<<10)|mm);
    }
  }
  int E=(int)e-127, he=E+15;
  if (he>=0x1F) return (uint16_t)((s<<15)|(0x1Fu<<10));
  if (he<=0){ if(he<-10) return (uint16_t)(s<<15);
    uint32_t mm=(0x800000u|m)>>(1-he+13);
    return (uint16_t)((s<<15)|(mm&0x3FFu));
  }
  uint32_t mant = m>>13;
  if((m&0x1000u) && (((m&0x1FFFu)!=0u) || (mant&1u))){
    ++mant; if(mant==0x400u){ mant=0; ++he; if(he>=0x1F){he=0x1F; mant=0;} }
  }
  return (uint16_t)((s<<15)|((uint16_t)he<<10)|(uint16_t)mant);
}
SIMD_FORCEINLINE float half_to_float(uint16_t h){
  uint32_t s=(h>>15)&1u, e=(h>>10)&0x1Fu, m=h&0x3FFu, out;
  if (e==0){
    if (!m) out = s<<31;
    else {
      int sh=0; while((m&0x400u)==0u){m<<=1;++sh;}
      m&=0x3FFu; int E=1-15-sh;
      out = (s<<31) | ((uint32_t)(E+127)<<23) | (m<<13);
    }
  } else if (e==0x1F){
    out = (s<<31)|(0xFFu<<23)|(m?((m<<13)|0x400000u):0u);
  } else {
    out = (s<<31) | ((uint32_t)(e-15+127)<<23) | (m<<13);
  }
  return _bit_cast<float>(out);
}

// ---- packs: float<->half por-lane ----
template<int D>
SIMD_FORCEINLINE simd_pack_t<D,uint16_t> float_to_half(const simd_pack_t<D,float>& a){
  simd_pack_t<D,uint16_t> r(uint16_t{0});
  for(int i=0;i<D;++i) (&r.x)[i] = float_to_half((&a.x)[i]);
  return r;
}
template<int D>
SIMD_FORCEINLINE simd_pack_t<D,float> half_to_float(const simd_pack_t<D,uint16_t>& a){
  simd_pack_t<D,float> r(0.f);
  for(int i=0;i<D;++i) (&r.x)[i] = half_to_float((&a.x)[i]);
  return r;
}







// ================== 9) Swizzles / Permutes ==================
// Fallback compile-time: float4/double4
template<typename T, int X,int Y,int Z,int W>
SIMD_FORCEINLINE simd_pack_t<4,T> shuffle(const simd_pack_t<4,T>& a){
  static_assert(0<=X && X<4 && 0<=Y && Y<4 && 0<=Z && Z<4 && 0<=W && W<4, "shuffle index out of range");
  const T v[4]={a.x,a.y,a.z,a.w};
  return simd_pack_t<4,T>(v[X],v[Y],v[Z],v[W]);
}
// Para double2: shuffle<X,Y>
template<int X,int Y>
SIMD_FORCEINLINE simd_pack_t<2,double> shuffle(const simd_pack_t<2,double>& a){
  static_assert(0<=X && X<2 && 0<=Y && Y<2,"shuffle2 index");
  const double v[2]={a.x,a.y};
  return simd_pack_t<2,double>(v[X],v[Y]);
}

// Patrones comunes (float4/double4)
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> dup_x (const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.x,a.x,a.x,a.x); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> dup_y (const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.y,a.y,a.y,a.y); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> dup_z (const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.z,a.z,a.z,a.z); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> dup_w (const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.w,a.w,a.w,a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> swap_xy(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.y,a.x,a.z,a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> swap_zw(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.x,a.y,a.w,a.z); }
// Algunos patrones frecuentes en gráficos
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> wzxy(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.w,a.z,a.x,a.y); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> zxyw(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.z,a.x,a.y,a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> yzxw(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.y,a.z,a.x,a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> xywz(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.x,a.y,a.w,a.z); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> yxzw(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.y,a.x,a.z,a.w); }
template<typename T> SIMD_FORCEINLINE simd_pack_t<4,T> zwxy(const simd_pack_t<4,T>& a){ return simd_pack_t<4,T>(a.z,a.w,a.x,a.y); }

// Permute runtime: array de 4 índices o pack<int,4>
template<typename T>
SIMD_FORCEINLINE simd_pack_t<4,T> permute(const simd_pack_t<4,T>& a, const int idx[4]){
  const T v[4]={a.x,a.y,a.z,a.w};
  return simd_pack_t<4,T>( v[clampT(idx[0],0,3)], v[clampT(idx[1],0,3)],
                           v[clampT(idx[2],0,3)], v[clampT(idx[3],0,3)] );
}
template<typename T>
SIMD_FORCEINLINE simd_pack_t<4,T> permute(const simd_pack_t<4,T>& a, const simd_pack_t<4,int>& idx){
  const T v[4]={a.x,a.y,a.z,a.w};
  return simd_pack_t<4,T>( v[clampT(idx.x,0,3)], v[clampT(idx.y,0,3)],
                           v[clampT(idx.z,0,3)], v[clampT(idx.w,0,3)] );
}

}


namespace simd
{
// --------- pack_saturate y float<->half ----------
#if defined(__SSE2__) || defined(_M_X64)
template<> SIMD_FORCEINLINE simd_pack_t<4,uint8_t>
simd::pack_saturate<4>(const simd_pack_t<4,float>& a){
  __m128  x   = a.m;
  __m128i i32 = _mm_cvtps_epi32(x);
  __m128i i16 = _mm_packs_epi32(i32, _mm_setzero_si128());
  __m128i u8  = _mm_packus_epi16(i16, _mm_setzero_si128());
  alignas(16) uint8_t out[16];
  _mm_store_si128((__m128i*)out, u8);
  return simd_pack_t<4,uint8_t>(out[0],out[1],out[2],out[3]);
}
template<> SIMD_FORCEINLINE simd_pack_t<4,int16_t>
simd::pack_saturate<4>(const simd_pack_t<4,int32_t>& a){
  __m128i i16 = _mm_packs_epi32(a.m, _mm_setzero_si128());
  alignas(16) int16_t out[8];
  _mm_store_si128((__m128i*)out, i16);
  return simd_pack_t<4,int16_t>(out[0],out[1],out[2],out[3]);
}
#endif

#if defined(__F16C__)
template<> SIMD_FORCEINLINE simd_pack_t<4,uint16_t>
simd::float_to_half<4>(const simd_pack_t<4,float>& a){
  __m128  x  = a.m;
  __m128i h4 = _mm_cvtps_ph(x, 0); // RNE
  alignas(16) uint16_t out[8];
  _mm_store_si128((__m128i*)out, h4);
  return simd_pack_t<4,uint16_t>(out[0],out[1],out[2],out[3]);
}
template<> SIMD_FORCEINLINE simd_pack_t<4,float>
simd::half_to_float<4>(const simd_pack_t<4,uint16_t>& a){
  alignas(16) uint16_t in[8] = {a.x,a.y,a.z,a.w,0,0,0,0};
  __m128 f4 = _mm_cvtph_ps(_mm_load_si128((const __m128i*)in));
  return simd_pack_t<4,float>(f4);
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
template<> SIMD_FORCEINLINE simd_pack_t<4,uint8_t>
simd::pack_saturate<4>(const simd_pack_t<4,float>& a){
  int32x4_t i32 = vcvtq_s32_f32(a.m);
  uint16x4_t u16= vqmovun_s32(i32);
  uint8x8_t  u8 = vqmovn_u16(vcombine_u16(u16, vdup_n_u16(0)));
  return simd_pack_t<4,uint8_t>( vget_lane_u8(u8,0),vget_lane_u8(u8,1),
                                 vget_lane_u8(u8,2),vget_lane_u8(u8,3) );
}
template<> SIMD_FORCEINLINE simd_pack_t<4,int16_t>
simd::pack_saturate<4>(const simd_pack_t<4,int32_t>& a){
  int16x4_t i16 = vqmovn_s32(a.m);
  return simd_pack_t<4,int16_t>( vget_lane_s16(i16,0),vget_lane_s16(i16,1),
                                 vget_lane_s16(i16,2),vget_lane_s16(i16,3) );
}
#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC__)
template<> SIMD_FORCEINLINE simd_pack_t<4,uint16_t>
simd::float_to_half<4>(const simd_pack_t<4,float>& a){
  float16x4_t h = vcvt_f16_f32(a.m);
  return simd_pack_t<4,uint16_t>( vget_lane_u16(vreinterpret_u16_f16(h),0),
                                  vget_lane_u16(vreinterpret_u16_f16(h),1),
                                  vget_lane_u16(vreinterpret_u16_f16(h),2),
                                  vget_lane_u16(vreinterpret_u16_f16(h),3) );
}
template<> SIMD_FORCEINLINE simd_pack_t<4,float>
simd::half_to_float<4>(const simd_pack_t<4,uint16_t>& a){
  uint64_t pack = (uint64_t)a.x | ((uint64_t)a.y<<16) | ((uint64_t)a.z<<32) | ((uint64_t)a.w<<48);
  float16x4_t h = vreinterpret_f16_u16( vcreate_u16(pack) );
  return simd_pack_t<4,float>( vcvt_f32_f16(h) );
}
#endif
#endif // NEON






// ---------------- Swizzles / Permute runtime ----------------
#if defined(__SSE__) || defined(__SSE2__) || defined(_M_X64)
// Algunos patrones optimizados en float4
template<> inline simd::simd_pack_t<4,float>
simd::shuffle<float,3,2,1,0>(const simd_pack_t<4,float>& a){
  return simd_pack_t<4,float>(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(0,1,2,3)));
}
template<> inline simd::simd_pack_t<4,float>
simd::dup_x<float>(const simd_pack_t<4,float>& a){
  return simd_pack_t<4,float>(_mm_shuffle_ps(a.m,a.m,_MM_SHUFFLE(0,0,0,0)));
}
template<> inline simd::simd_pack_t<4,float>
simd::dup_y<float>(const simd_pack_t<4,float>& a){
  return simd_pack_t<4,float>(_mm_shuffle_ps(a.m,a.m,_MM_SHUFFLE(1,1,1,1)));
}
// double2 shuffle
template<> inline simd::simd_pack_t<2,double>
simd::shuffle<1,0>(const simd_pack_t<2,double>& a){
  return simd_pack_t<2,double>(_mm_shuffle_pd(a.m, a.m, 0b01));
}
#endif

#if defined(__AVX__) || defined(_M_AVX)
// Permute runtime (float4) con _mm_permutevar_ps (AVX1+)
template<> inline simd::simd_pack_t<4,float>
simd::permute<float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,int>& idx){
  __m128i m = _mm_set_epi32(idx.w&3, idx.z&3, idx.y&3, idx.x&3);
  return simd_pack_t<4,float>(_mm_permutevar_ps(a.m, m));
}
#endif

#if defined(__AVX2__) || defined(_M_AVX2)
// double4: permute runtime por 64-bit lanes
template<> inline simd::simd_pack_t<4,double>
simd::permute<double>(const simd_pack_t<4,double>& a, const simd_pack_t<4,int>& idx){
  // construimos máscara para _mm256_permute4x64_pd (cada 2-bit controla un lane)
  int m = ((idx.w&3)<<6)|((idx.z&3)<<4)|((idx.y&3)<<2)|(idx.x&3);
  return simd_pack_t<4,double>(_mm256_permute4x64_pd(a.m, m));
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
// Duplicados frecuentes en NEON
template<> inline simd::simd_pack_t<4,float>
simd::dup_x<float>(const simd_pack_t<4,float>& a){ return simd_pack_t<4,float>( vdupq_laneq_f32(a.m,0) ); }
template<> inline simd::simd_pack_t<4,float>
simd::dup_y<float>(const simd_pack_t<4,float>& a){ return simd_pack_t<4,float>( vdupq_laneq_f32(a.m,1) ); }
#if defined(__aarch64__)
// Permute runtime float4 con vqtbl1q_u8
template<> inline simd::simd_pack_t<4,float>
simd::permute<float>(const simd_pack_t<4,float>& a, const simd_pack_t<4,int>& idx){
  uint8x16_t bytes = vreinterpretq_u8_f32(a.m);
  uint8_t m[16];
  for(int i=0;i<4;++i){ uint8_t b = uint8_t(((&idx.x)[i]&3)*4); m[i*4+0]=b+0; m[i*4+1]=b+1; m[i*4+2]=b+2; m[i*4+3]=b+3; }
  uint8x16_t sel = vld1q_u8(m);
  float32x4_t r  = vreinterpretq_f32_u8( vqtbl1q_u8(bytes, sel) );
  return simd_pack_t<4,float>(r);
}
#endif
#endif // NEON
}