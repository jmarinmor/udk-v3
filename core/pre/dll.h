#ifndef UDK_PRE_DLL_H
#define UDK_PRE_DLL_H

#include "compiler.h"

// DLL export and import definitions
#ifdef UDK_COMPILER_MSVC
    #ifdef UDK_DLL_EXPORT
        #define UDK_API __declspec(dllexport)
    #else
        #define UDK_API __declspec(dllimport)
    #endif
#else // GCC, Clang
    #ifdef UDK_DLL_EXPORT
        #define UDK_API __attribute__((visibility("default")))
    #else
        #define UDK_API
    #endif
#endif

// Force inline
#ifdef UDK_COMPILER_MSVC
    #define UDK_INLINE __forceinline
#else // GCC, Clang
    #define UDK_INLINE __attribute__((always_inline)) inline
#endif

#endif // UDK_PRE_DLL_H
