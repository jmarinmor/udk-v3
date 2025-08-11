#include <simd/simd_conversions.h>

namespace simd {

// -----------------------------------------------------
// float <-> half (escalares) — únicas definiciones aquí
// -----------------------------------------------------
uint16_t float_to_half(float f) {
	uint32_t x = _bit_cast<uint32_t>(f);
	uint32_t s = (x >> 31) & 1u;
	uint32_t e = (x >> 23) & 0xFFu;
	uint32_t m = x & 0x7FFFFFu;

	// Inf/NaN
	if (e == 0xFFu) {
		return (uint16_t)((s << 15) | (0x1Fu << 10) | (m ? 0x200u : 0u));
	}

	// Cero/Subnormal en float
	if (e == 0) {
		if (!m) return (uint16_t)(s << 15);
		// Normaliza mantisa para construir subnormal/normal en half
		int sh = 0;
		while ((m & 0x800000u) == 0u) { m <<= 1; ++sh; if (sh > 24) break; }
		int E  = 1 - 127 - sh;
		int he = E + 15;
		if (he <= 0) {
			if (he < -10) return (uint16_t)(s << 15);
			uint32_t mm = (0x800000u | (m & 0x7FFFFFu)) >> (1 - he + 13);
			return (uint16_t)((s << 15) | (mm & 0x3FFu));
		} else {
			uint16_t mm = (uint16_t)((m & 0x7FFFFFu) >> 13);
			return (uint16_t)((s << 15) | ((uint16_t)he << 10) | mm);
		}
	}

	// Normal
	int E  = (int)e - 127;
	int he = E + 15;
	if (he >= 0x1F) return (uint16_t)((s << 15) | (0x1Fu << 10)); // overflow -> +inf
	if (he <= 0) {
		if (he < -10) return (uint16_t)(s << 15);
		uint32_t mm = (0x800000u | m) >> (1 - he + 13);
		return (uint16_t)((s << 15) | (mm & 0x3FFu));
	}
	// Redondeo a 10 bits con round-to-nearest-even
	uint32_t mant = m >> 13;
	if ((m & 0x1000u) && (((m & 0x1FFFu) != 0u) || (mant & 1u))) {
		++mant;
		if (mant == 0x400u) { mant = 0; ++he; if (he >= 0x1F) { he = 0x1F; mant = 0; } }
	}
	return (uint16_t)((s << 15) | ((uint16_t)he << 10) | (uint16_t)mant);
}

float half_to_float(uint16_t h) {
	uint32_t s = (h >> 15) & 1u;
	uint32_t e = (h >> 10) & 0x1Fu;
	uint32_t m = h & 0x3FFu;

	uint32_t out;
	if (e == 0) {
		if (!m) {
			out = s << 31; // ±0
		} else {
			// subnormal: normaliza y ajusta exponente
			int sh = 0;
			while ((m & 0x400u) == 0u) { m <<= 1; ++sh; }
			m &= 0x3FFu;
			int E = 1 - 15 - sh;
			out = (s << 31) | (uint32_t(E + 127) << 23) | (m << 13);
		}
	} else if (e == 0x1Fu) {
		// Inf/NaN (propaga quiet NaN)
		out = (s << 31) | (0xFFu << 23) | (m ? ((m << 13) | 0x400000u) : 0u);
	} else {
		// Normal
		out = (s << 31) | (uint32_t(e - 15 + 127) << 23) | (m << 13);
	}
	return _bit_cast<float>(out);
}

} // namespace simd
