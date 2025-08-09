#ifndef PRE_OS_H
#define PRE_OS_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OS detection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define OS_UNKNOWN		(0x00000000)
#define OS_WINDOWS		(1 << 8)
#define OS_LINUX		(1 << 9)
#define OS_ANDROID		(1 << 10)
#define OS_CHROME_NACL	(1 << 11)
#define OS_UNIX			(1 << 12)
#define OS_QNXNTO		(1 << 13)
#define OS_WINCE		(1 << 14)
#define OS_CYGWIN		(1 << 15)
#define OS_APPLE		(1 << 16)
#define OS_IOS			(1 << 17)
#define OS_OSX			(1 << 18)
#define OS_SIMULATOR	(1 << 19)
#define OS_BSD			(1 << 20)
#define OS_FREE_BSD		(1 << 21)
#define OS_NET_BSD		(1 << 22)
#define OS_SOLARIS		(1 << 23)
#define OS_SYMBIAN		(1 << 24)
#define OS_AIX			(1 << 25)



#if defined(__CYGWIN__)
#	define OS (OS_CYGWIN)
#elif defined(__QNXNTO__)
#	define OS (OS_QNXNTO)
#elif defined(__APPLE__)
#	include <TargetConditionals.h>
#	if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE!=0
#		include <Availability.h>
#		if defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR!=0
#			define OS (OS_APPLE | OS_IOS | OS_SIMULATOR | OS_UNIX)
#		else
#			define OS (OS_APPLE | OS_IOS | OS_UNIX)
#		endif
#	elif defined(TARGET_OS_MAC) && TARGET_OS_MAC != 0
#		define OS (OS_APPLE | OS_OSX | OS_UNIX)
#	endif
#elif defined(WINCE)
#	define OS (OS_WINCE)
#elif defined(_WIN32)
#	define OS (OS_WINDOWS)
#elif defined(sun) || defined(__sun)
#	define OS (OS_SOLARIS)
#elif defined(__FreeBSD__)
#	define OS (OS_FREE_BSD | OS_UNIX | OS_BSD)
#elif defined(__NetBSD__)
#	define OS (OS_NET_BSD | OS_UNIX | OS_BSD)
#elif defined(_AIX)
#	define OS (OS_AIX | OS_UNIX)
#elif defined(__S60__)
#	define OS (OS_SYMBIAN | OS_UNIX)
#elif defined(__native_client__)
#	define OS (OS_CHROME_NACL)
#elif defined(__ANDROID__)
#	define OS (OS_ANDROID | OS_UNIX)
#elif defined(__linux) || defined(__linux__)
#	define OS (OS_LINUX | OS_UNIX)
#elif defined(__unix)
#	define OS (OS_UNIX)
#else
#   error "Unsupported operating system"
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Log messages
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Report platform detection
#if PRE_MESSAGES == PRE_MESSAGES_ENABLED && !defined(PRE_MESSAGE_PLATFORM_DISPLAYED)
#	define PRE_MESSAGE_PLATFORM_DISPLAYED
#	if(OS & OS_QNXNTO)
#		pragma message("PRE: QNX platform detected")
#	endif
#	if(OS & OS_IOS)
#		pragma message("PRE: iOS platform detected")
#	endif
#	if(OS & OS_APPLE)
#		pragma message("PRE: Apple platform detected")
#	endif
#	if(OS & OS_WINCE)
#		pragma message("PRE: WinCE platform detected")
#	endif
#	if(OS & OS_WINDOWS)
#		pragma message("PRE: Windows platform detected")
#	endif
#	if(OS & OS_CHROME_NACL)
#		pragma message("PRE: Native Client detected")
#	endif
#	if(OS & OS_ANDROID)
#		pragma message("PRE: Android platform detected")
#	endif
#	if(OS & OS_LINUX)
#		pragma message("PRE: Linux platform detected")
#	endif
#	if(OS & OS_UNIX)
#		pragma message("PRE: UNIX platform detected")
#	endif
#	if(OS & OS_UNKNOWN)
#		pragma message("PRE: platform unknown")
#	endif
#endif//PRE_MESSAGES

#endif // PRE_OS_H

