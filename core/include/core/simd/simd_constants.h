#pragma once

namespace simd
{
	// ==========================================================
// Constantes y helpers (sin <cmath>)
// ==========================================================
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
		static constexpr float ln2_hi = 0.6931457519f;   // split para reducción
		static constexpr float ln2_lo = 1.4286067653e-6f;
	};
	template<> struct _cst_<double> {
		static constexpr double ln2 = 0.693147180559945309417232121458176568;
		static constexpr double inv_ln2 = 1.44269504088896340735992468100189214; // log2(e)
		static constexpr double ln2_hi = 0.6931471805596629; // split (Cody–Waite)
		static constexpr double ln2_lo = 2.384185791015625e-07;
	};

}