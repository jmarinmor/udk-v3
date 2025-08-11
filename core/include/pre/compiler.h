#ifndef PRE_COMPILER_H
#define PRE_COMPILER_H


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compiler detection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Intel
#define COMPILER_INTEL			0x00100000
// Visual C++ defines
#define COMPILER_VC				0x01000000
// GCC defines
#define COMPILER_GCC			0x02000000
// CUDA
#define COMPILER_CUDA			0x10000000
// Clang
#define COMPILER_CLANG			0x20000000

// Intel
#define COMPILER_INTEL14		(0x00000040 | COMPILER_INTEL)
#define COMPILER_INTEL15		(0x00000050 | COMPILER_INTEL)
#define COMPILER_INTEL16		(0x00000060 | COMPILER_INTEL)
#define COMPILER_INTEL17		(0x00000070 | COMPILER_INTEL)
// Visual C++ defines
#define COMPILER_VC12			(0x000000B0 | COMPILER_VC)
#define COMPILER_VC14			(0x000000C0 | COMPILER_VC)
#define COMPILER_VC15			(0x000000D0 | COMPILER_VC)
#define COMPILER_VC16			(0x000000E0 | COMPILER_VC)
// GCC defines
#define COMPILER_GCC46			(0x000000D0 | COMPILER_GCC)
#define COMPILER_GCC47			(0x000000E0 | COMPILER_GCC)
#define COMPILER_GCC48			(0x000000F0 | COMPILER_GCC)
#define COMPILER_GCC49			(0x00000100 | COMPILER_GCC)
#define COMPILER_GCC5			(0x00000200 | COMPILER_GCC)
#define COMPILER_GCC6			(0x00000300 | COMPILER_GCC)
#define COMPILER_GCC7			(0x00000400 | COMPILER_GCC)
#define COMPILER_GCC8			(0x00000500 | COMPILER_GCC)
// CUDA
#define COMPILER_CUDA70			(0x000000A0 | COMPILER_CUDA)
#define COMPILER_CUDA75			(0x000000B0 | COMPILER_CUDA)
#define COMPILER_CUDA80			(0x000000C0 | COMPILER_CUDA)
// Clang
#define COMPILER_CLANG34		(0x00000050 | COMPILER_CLANG)
#define COMPILER_CLANG35		(0x00000060 | COMPILER_CLANG)
#define COMPILER_CLANG36		(0x00000070 | COMPILER_CLANG)
#define COMPILER_CLANG37		(0x00000080 | COMPILER_CLANG)
#define COMPILER_CLANG38		(0x00000090 | COMPILER_CLANG)
#define COMPILER_CLANG39		(0x000000A0 | COMPILER_CLANG)
#define COMPILER_CLANG40		(0x000000B0 | COMPILER_CLANG)
#define COMPILER_CLANG41		(0x000000C0 | COMPILER_CLANG)
#define COMPILER_CLANG42		(0x000000D0 | COMPILER_CLANG)

// Force generic C++ compiler
#if defined(__INTEL_COMPILER)
#	if (__INTEL_COMPILER < 1400)
#		error "SML requires ICC 2013 SP1 or newer"
#	elif __INTEL_COMPILER == 1400
#		define COMPILER COMPILER_INTEL14
#	elif __INTEL_COMPILER == 1500
#		define COMPILER COMPILER_INTEL15
#	elif __INTEL_COMPILER == 1600
#		define COMPILER COMPILER_INTEL16
#	elif __INTEL_COMPILER >= 1700
#		define COMPILER COMPILER_INTEL17
#	endif
// CUDA
#elif defined(__CUDACC__)
#	if !defined(CUDA_VERSION) && !defined(FORCE_CUDA)
#		include <cuda.h>  // make sure version is defined since nvcc does not define it itself!
#	endif
#	if CUDA_VERSION < 7000
#		error "SML requires CUDA 7.0 or higher"
#	elif (CUDA_VERSION >= 7000 && CUDA_VERSION < 7500)
#		define COMPILER COMPILER_CUDA70
#	elif (CUDA_VERSION >= 7500 && CUDA_VERSION < 8000)
#		define COMPILER COMPILER_CUDA75
#	elif (CUDA_VERSION >= 8000)
#		define COMPILER COMPILER_CUDA80
#	endif
// Clang
#elif defined(__clang__)
#	if defined(__apple_build_version__)
#		if (__clang_major__ < 6)
#			error "SML requires Clang 3.4 / Apple Clang 6.0 or higher"
#		elif __clang_major__ == 6 && __clang_minor__ == 0
#			define COMPILER COMPILER_CLANG35
#		elif __clang_major__ == 6 && __clang_minor__ >= 1
#			define COMPILER COMPILER_CLANG36
#		elif __clang_major__ >= 7
#			define COMPILER COMPILER_CLANG37
#		endif
#	else
#		if ((__clang_major__ == 3) && (__clang_minor__ < 4)) || (__clang_major__ < 3)
#			error "SML requires Clang 3.4 or higher"
#		elif __clang_major__ == 3 && __clang_minor__ == 4
#			define COMPILER COMPILER_CLANG34
#		elif __clang_major__ == 3 && __clang_minor__ == 5
#			define COMPILER COMPILER_CLANG35
#		elif __clang_major__ == 3 && __clang_minor__ == 6
#			define COMPILER COMPILER_CLANG36
#		elif __clang_major__ == 3 && __clang_minor__ == 7
#			define COMPILER COMPILER_CLANG37
#		elif __clang_major__ == 3 && __clang_minor__ == 8
#			define COMPILER COMPILER_CLANG38
#		elif __clang_major__ == 3 && __clang_minor__ >= 9
#			define COMPILER COMPILER_CLANG39
#		elif __clang_major__ == 4 && __clang_minor__ == 0
#			define COMPILER COMPILER_CLANG40
#		elif __clang_major__ == 4 && __clang_minor__ == 1
#			define COMPILER COMPILER_CLANG41
#		elif __clang_major__ == 4 && __clang_minor__ >= 2
#			define COMPILER COMPILER_CLANG42
#		elif __clang_major__ >= 4
#			define COMPILER COMPILER_CLANG42
#		endif
#	endif
// Visual C++
#elif defined(_MSC_VER)
#	if _MSC_VER < 1800
#		error "SML requires Visual C++ 12 - 2013 or higher"
#	elif _MSC_VER == 1800
#		define COMPILER COMPILER_VC12
#	elif _MSC_VER == 1900
#		define COMPILER COMPILER_VC14
#	elif _MSC_VER >= 1910
#		define COMPILER COMPILER_VC15
#	endif//_MSC_VER
// G++
#elif defined(__GNUC__) || defined(__MINGW32__)
#	if ((__GNUC__ == 4) && (__GNUC_MINOR__ < 6)) || (__GNUC__ < 4)
#		error "SML requires GCC 4.7 or higher"
#	elif (__GNUC__ == 4) && (__GNUC_MINOR__ == 6)
#		define COMPILER (COMPILER_GCC46)
#	elif (__GNUC__ == 4) && (__GNUC_MINOR__ == 7)
#		define COMPILER (COMPILER_GCC47)
#	elif (__GNUC__ == 4) && (__GNUC_MINOR__ == 8)
#		define COMPILER (COMPILER_GCC48)
#	elif (__GNUC__ == 4) && (__GNUC_MINOR__ >= 9)
#		define COMPILER (COMPILER_GCC49)
#	elif (__GNUC__ == 5)
#		define COMPILER (COMPILER_GCC5)
#	elif (__GNUC__ == 6)
#		define COMPILER (COMPILER_GCC6)
#	elif (__GNUC__ == 7)
#		define COMPILER (COMPILER_GCC7)
#	elif (__GNUC__ >= 8)
#		define COMPILER (COMPILER_GCC8)
#	endif
#else
#	error "PRE: No compiler platform detected"
#endif




#if PRE_MESSAGES == PRE_MESSAGES_ENABLED && !defined(PRE_MESSAGE_COMPILER_DISPLAYED)
#	define PRE_MESSAGE_COMPILER_DISPLAYED
#	if COMPILER & COMPILER_CUDA
#		pragma message("PRE: CUDA compiler detected")
#	elif COMPILER & COMPILER_VC
#		pragma message("PRE: Visual C++ compiler detected")
#	elif COMPILER & COMPILER_CLANG
#		pragma message("PRE: Clang compiler detected")
#	elif COMPILER & COMPILER_INTEL
#		pragma message("PRE: Intel Compiler detected")
#	elif COMPILER & COMPILER_GCC
#		pragma message("PRE: GCC compiler detected")
#	else
#		pragma message("PRE: Compiler not detected")
#	endif
#endif//PRE_MESSAGES



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory alignment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#	define STRUCT_ALIGN(N) __declspec(align(N))
#elif defined(__GNUC__) || defined(__clang__)
#	define STRUCT_ALIGN(N) __attribute__((aligned(N)))
#else
#	define STRUCT_ALIGN(N)
#endif



#endif // PRE_COMPILER_H
