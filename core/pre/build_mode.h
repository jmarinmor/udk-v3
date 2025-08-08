#ifndef UDK_PRE_BUILD_MODE_H
#define UDK_PRE_BUILD_MODE_H

// Detect build mode
#if defined(DEBUG) || defined(_DEBUG)
    #define UDK_BUILD_MODE_DEBUG
#else
    #define UDK_BUILD_MODE_RELEASE
#endif

#endif // UDK_PRE_BUILD_MODE_H