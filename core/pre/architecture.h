#ifndef PRE_ARCHITECTURE_H
#define PRE_ARCHITECTURE_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Full cpu architecture detection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define CPU_8_BIT		(1 << 0)
#define CPU_16_BIT		(1 << 1)
#define CPU_32_BIT		(1 << 2)
#define CPU_64_BIT		(1 << 3)
#define CPU_128_BIT		(1 << 4)
#define CPU_256_BIT		(1 << 5)
#define CPU_X86_BIT		(1 << 6)
#define CPU_X64_BIT		(1 << 7)
#define CPU_ARM_BIT		(1 << 8)
#define CPU_MIPS_BIT	(1 << 9)
#define CPU_PPC_BIT		(1 << 10)

#define CPU_MMX_BIT	    (1 << 16)
#define CPU_SSE1_BIT	(1 << 17)
#define CPU_SSE2_BIT	(1 << 18)
#define CPU_SSE3_BIT	(1 << 19)
#define CPU_SSSE3_BIT	(1 << 20)
#define CPU_SSE41_BIT	(1 << 21)
#define CPU_SSE42_BIT	(1 << 22)
#define CPU_AVX_BIT		(1 << 23)
#define CPU_AVX2_BIT	(1 << 24)
#define CPU_AVX512_BIT	(1 << 25)
#define CPU_NEON_BIT	(1 << 26)






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect architecture
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__)
#   define CPU_ARCH    (CPU_X86_BIT)
#elif  defined(_M_X64)
#   dfine CPU_ARCH      (CPU_X64_BIT)
#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM)
#   define CPU_ARCH     (CPU_ARM_BIT)
#elif defined(__mips__ )
#	define CPU_ARCH     (CPU_MIPS_BIT)
#elif defined(__powerpc__ )
#	define CPU_ARCH     (CPU_PPC_BIT)
#else
    #error "Unsupported architecture"
#endif





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect architecture bit count
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(__aarch64__) || defined(__arch64__) || defined(__LP64__) || defined(_M_X64) || defined(__ppc64__) || defined(__PPC64__) || defined(__x86_64__) || defined(__ia64__) || defined(__64BIT__) || (__WORDSIZE == 64)
#	define CPU_BITS_BIT	(CPU_64_BIT)
#   define CPU_BIT_COUNT 64
#else
#	define CPU_BITS_BIT	(CPU_32_BIT)
#   define CPU_BIT_COUNT 32
#endif




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Detect extensions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CPU_SSE1		(CPU_SSE1_BIT)
#define CPU_SSE2		(CPU_SSE2_BIT | CPU_SSE1_BIT)
#define CPU_SSE3		(CPU_SSE3_BIT | CPU_SSE2)
#define CPU_SSSE3		(CPU_SSSE3_BIT | CPU_SSE3)
#define CPU_SSE41		(CPU_SSE41_BIT | CPU_SSSE3)
#define CPU_SSE42		(CPU_SSE42_BIT | CPU_SSE41)
#define CPU_AVX			(CPU_AVX_BIT | CPU_SSE42)
#define CPU_AVX2		(CPU_AVX2_BIT | CPU_AVX)
#define CPU_AVX512		(CPU_AVX512_BIT | CPU_AVX2)
#define CPU_NEON		(CPU_NEON_BIT)
#define CPU_PURE		()
#if defined(__AVX512BW__) && defined(__AVX512F__) && defined(__AVX512CD__) && defined(__AVX512VL__) && defined(__AVX512DQ__)
#	define CPU_EXTENSIONS_BIT (CPU_AVX512)
#elif defined(__AVX2__)
#	define CPU_EXTENSIONS_BIT (CPU_AVX2)
#elif defined(__AVX__)
#	define CPU_EXTENSIONS_BIT (CPU_AVX)
#elif defined(__SSE4_2__)
#	define CPU_EXTENSIONS_BIT (CPU_SSE42)
#elif defined(__SSE4_1__)
#	define CPU_EXTENSIONS_BIT (CPU_SSE41)
#elif defined(__SSSE3__)
#	define CPU_EXTENSIONS_BIT (CPU_SSSE3)
#elif defined(__SSE3__)
#	define CPU_EXTENSIONS_BIT (CPU_SSE3)
#elif defined(__SSE2__)
#	define CPU_EXTENSIONS_BIT (CPU_SSE2)
#elif defined(_M_X64)
#	define CPU_EXTENSIONS_BIT (CPU_SSE2)
#elif defined(_M_IX86_FP)
#	if _M_IX86_FP >= 2
#		define CPU (CPU_SSE2)
#	else
#		define CPU (CPU_PURE)
#	endif
#elif defined(__SSE__)
#	define CPU_EXTENSIONS_BIT (CPU_SSE1)
#elif defined(_M_PPC)
#	define CPU_EXTENSIONS_BIT (CPU_PPC)
#elif defined(__i386__) || defined(__x86_64__)
#	define CPU_EXTENSIONS_BIT (CPU_X86)
#elif defined(__ARM_NEON)
#	define CPU_EXTENSIONS_BIT (CPU_NEON)
#elif defined(__MMX__)
#   define CPU_EXTENSIONS_BIT (CPU_MMX)
#else
#	define CPU_EXTENSIONS_BIT (CPU_PURE)
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Final cpu
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CPU (CPU_ARCH | CPU_BITS_BIT | CPU_EXTENSIONS_BIT)





#if PRE_MESSAGES == PRE_MESSAGES_ENABLED && !defined(PRE_MESSAGE_ARCH_DISPLAYED)
#	define PRE_MESSAGE_ARCH_DISPLAYED
#	if(CPU_EXTENSIONS_BIT == CPU_EXTENSIONS_BIT_PURE)
#		pragma message("GLM: Platform independent code")
#	elif(CPU_EXTENSIONS_BIT == CPU_AVX2)
#		pragma message("GLM: AVX2 instruction set")
#	elif(CPU_EXTENSIONS_BIT == CPU_AVX)
#		pragma message("GLM: AVX instruction set")
#	elif(CPU_EXTENSIONS_BIT == CPU_SSE42)
#		pragma message("GLM: SSE4.2 instruction set")
#	elif(CPU_EXTENSIONS_BIT == CPU_SSE41)
#		pragma message("GLM: SSE4.1 instruction set")
#	elif(CPU_EXTENSIONS_BIT == CPU_SSSE3)
#		pragma message("GLM: SSSE3 instruction set")
#	elif(CPU_EXTENSIONS_BIT == CPU_SSE3)
#		pragma message("GLM: SSE3 instruction set")
#	elif(CPU_EXTENSIONS_BIT == CPU_SSE2)
#		pragma message("GLM: SSE2 instruction set")
#	elif(CPU_EXTENSIONS_BIT == CPU_NEON)
#		pragma message("GLM: NEON instruction set")
#	endif//CPU_EXTENSIONS_BIT
#endif//PRE_MESSAGES


#endif // ARCHITECTURE_H


