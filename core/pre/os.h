#ifndef UDK_PRE_OS_H
#define UDK_PRE_OS_H

// Detect operating system
#if defined(_WIN32)
    #define UDK_OS_WINDOWS
#elif defined(__APPLE__)
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #define UDK_OS_IOS
    #elif TARGET_OS_MAC
        #define UDK_OS_MACOS
    #else
        #error "Unsupported Apple platform"
    #endif
#elif defined(__ANDROID__)
    #define UDK_OS_ANDROID
#elif defined(__linux__)
    #define UDK_OS_LINUX
#else
    #error "Unsupported operating system"
#endif

#endif // UDK_PRE_OS_H