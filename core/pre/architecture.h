#ifndef UDK_PRE_ARCHITECTURE_H
#define UDK_PRE_ARCHITECTURE_H

// Detect architecture
#if defined(__i386__) || defined(_M_IX86)
    #define UDK_ARCH_X86 32
#elif defined(__x86_64__) || defined(_M_X64)
    #define UDK_ARCH_X86 64
#elif defined(__arm__) || defined(_M_ARM)
    #define UDK_ARCH_ARM 32
#elif defined(__aarch64__)
    #define UDK_ARCH_ARM 64
#else
    #error "Unsupported architecture"
#endif

// Detect CPU extensions
#if defined(__SSE__)
    #define UDK_CPU_SSE
#endif

#if defined(__SSE2__)
    #define UDK_CPU_SSE2
#endif

#if defined(__SSE3__)
    #define UDK_CPU_SSE3
#endif

#if defined(__SSSE3__)
    #define UDK_CPU_SSSE3
#endif

#if defined(__SSE4_1__)
    #define UDK_CPU_SSE4_1
#endif

#if defined(__SSE4_2__)
    #define UDK_CPU_SSE4_2
#endif

#if defined(__AVX__)
    #define UDK_CPU_AVX
#endif

#if defined(__AVX2__)
    #define UDK_CPU_AVX2
#endif

#if defined(__MMX__)
    #define UDK_CPU_MMX
#endif

#if defined(__ARM_NEON)
    #define UDK_CPU_NEON
#endif

#endif // UDK_PRE_ARCHITECTURE_H
