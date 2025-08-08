#ifndef UDK_PRE_COMPILER_H
#define UDK_PRE_COMPILER_H

// Detect compiler
#if defined(__clang__)
    #define UDK_COMPILER_CLANG
#elif defined(__GNUC__)
    #define UDK_COMPILER_GCC
#elif defined(_MSC_VER)
    #define UDK_COMPILER_MSVC
#else
    #error "Unsupported compiler"
#endif

#endif // UDK_PRE_COMPILER_H