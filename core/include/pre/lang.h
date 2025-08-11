#ifndef PRE_LANG_H
#define PRE_LANG_H


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Language options
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compiling Mode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define COMPILING_MODE_DEBUG	(1 << 0)
#define COMPILING_MODE_RELEASE	(1 << 1)
#if defined(NDEBUG)
#	define COMPILING_MODE	COMPILING_MODE_RELEASE
#	define RELEASE
#elif defined(_DEBUG) || defined(__DEBUG) || defined(DEBUG) || defined(DEBUG_) || defined(DEBUG__) || defined(_debug) || defined(__debug) || defined(debug) || defined(debug_) || defined(debug__) || defined(XCODE_DEBUG_CONFIGURATION_DEBUG)
#	define COMPILING_MODE	COMPILING_MODE_DEBUG
#   if !defined(DEBUG)
#	    define DEBUG
#   endif
#   undef RELEASE
#else
#	define COMPILING_MODE	COMPILING_MODE_RELEASE
#   if !defined(RELEASE)
#	    define RELEASE
#   endif
#   undef DEBUG
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Force inline
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#	define FORCE_INLINE     __forceinline
#	define FORCE_OFFLINE    __declspec((noinline))
#elif defined(__CUDACC__)
#	define FORCE_INLINE     __forceinline__
#	define FORCE_OFFLINE    __noinline__
#elif defined(__clang__) || defined(__GNUC__) || defined(__MINGW32__)
#	define FORCE_INLINE     inline __attribute__((__always_inline__))
#	define FORCE_OFFLINE    __attribute__((__noinline__))
#else // GCC, Clang
#	define FORCE_INLINE inline
#	define FORCE_OFFLINE
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calling convention
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STDCALL
#	if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
#		define STDCALL __stdcall
#	else
#		define STDCALL
#	endif
#endif
#if defined(_MSC_VER)
#	define FASTCALL __fastcall
//#elif defined(__GCC__)
#else
#	define FASTCALL  __attribute__((fastcall))
#endif




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public API functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DLL_APICALL
#	if defined(_WIN32) && !defined(__SCITECH_SNAP__)
#		ifdef BUILD_DLL
#			define DLL_APICALL  __declspec(dllexport)
#		else
#			define DLL_APICALL  __declspec(dllimport)
#		endif
#	elif defined (__SYMBIAN32__)
#		define DLL_APICALL      IMPORT_C
#	else
#       define DLL_APICALL      __attribute__((visibility("default")))
#	endif
#endif






#define API_FUNCTION(_Return)			DLL_APICALL	_Return	DLL_CALLING_CONVENTION
#define API_FUNCTION_STATIC(_Return)	static DLL_APICALL	_Return	DLL_CALLING_CONVENTION
#define DLL_FNC(_Return)				API_FUNCTION(_Return)
#define SDLL_FNC(_Return)				static	API_FUNCTION(_Return)
#define INL_FNC(_Return)				FORCE_INLINE _Return
#define OFL_FNC(_Return)				FORCE_OFFLINE _Return
#define FAST_FNC(_Return)				_Return FASTCALL

#define PRE_CTORS(_T) \
		_T(); \
		_T(_T&&); \
		_T(const _T&); \
		~_T(); \
		auto operator = (const _T&) -> _T&;

#define PRE_ENUM_AND_OR(_T) \
	FORCE_INLINE _T operator | (_T a, _T b) {return _T(int(a)|int(b));} \
	FORCE_INLINE int operator & (_T a, _T b) {return int(int(a)&int(b));}


#endif // PRE_LANG_H
