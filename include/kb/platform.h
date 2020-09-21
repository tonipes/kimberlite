#pragma once

// Stuff mostly from BX

#define STRING(_x) STRING_(_x)
#define STRING_(_x) #_x

//#####################################################################################################################
// CPU
//#####################################################################################################################

#define KB_CPU_ARM   0
#define KB_CPU_JIT   0
#define KB_CPU_X86   0

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
#if defined(__arm__)     \
 || defined(__aarch64__) \
 || defined(_M_ARM)
#	undef  KB_CPU_ARM
#	define KB_CPU_ARM 1
#	define KB_CPU_RISCV 1
#elif defined(_M_IX86)    \
 ||   defined(_M_X64)     \
 ||   defined(__i386__)   \
 ||   defined(__x86_64__)
#	undef  KB_CPU_X86
#	define KB_CPU_X86 1
#else // PNaCl doesn't have CPU defined.
#	undef  KB_CPU_JIT
#	define KB_CPU_JIT 1
#endif //

#if KB_CPU_ARM
#	define KB_CPU_NAME "ARM"
#elif KB_CPU_X86
#	define KB_CPU_NAME "x86"
#endif

//#####################################################################################################################
// Arch
//#####################################################################################################################

#define KB_ARCH_32BIT 0
#define KB_ARCH_64BIT 0

#if defined(__x86_64__)    \
 || defined(_M_X64)        \
 || defined(__aarch64__)   \
 || defined(__64BIT__)     \
 || defined(__mips64)      \
 || defined(__powerpc64__) \
 || defined(__ppc64__)     \
 || defined(__LP64__)
#	undef  KB_ARCH_64BIT
#	define KB_ARCH_64BIT 64
#else
#	undef  KB_ARCH_32BIT
#	define KB_ARCH_32BIT 32
#endif //

#if KB_ARCH_32BIT
#	define KB_ARCH_NAME "32-bit"
#elif KB_ARCH_64BIT
#	define KB_ARCH_NAME "64-bit"
#endif // KB_ARCH_

//#####################################################################################################################
// Platform
//#####################################################################################################################

#define KB_PLATFORM_LINUX      0
#define KB_PLATFORM_OSX        0
#define KB_PLATFORM_WINDOWS    0
#define KB_PLATFORM_WINRT      0
#define KB_PLATFORM_IOS        0
#define KB_PLATFORM_NX         0
#define KB_PLATFORM_NAME       "Unknown"

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_WIN32) || defined(_WIN64)
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif // NOMINMAX
//  If _USING_V110_SDK71_ is defined it means we are using the v110_xp or v120_xp toolset.
#	if defined(_MSC_VER) && (_MSC_VER >= 1700) && (!_USING_V110_SDK71_)
#		include <winapifamily.h>
#	endif // defined(_MSC_VER) && (_MSC_VER >= 1700) && (!_USING_V110_SDK71_)
#	if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#		undef  KB_PLATFORM_WINDOWS
#		if !defined(WINVER) && !defined(_WIN32_WINNT)
#			if KB_ARCH_64BIT
//				When building 64-bit target Win7 and above.
#				define WINVER 0x0601
#				define _WIN32_WINNT 0x0601
#			else
//				Windows Server 2003 with SP1, Windows XP with SP2 and above
#				define WINVER 0x0502
#				define _WIN32_WINNT 0x0502
#			endif // KB_ARCH_64BIT
#		endif // !defined(WINVER) && !defined(_WIN32_WINNT)
#		define KB_PLATFORM_WINDOWS _WIN32_WINNT
#	else
#		undef  KB_PLATFORM_WINRT
#		define KB_PLATFORM_WINRT 1
#	endif
#elif  defined(__linux__)
#	undef  KB_PLATFORM_LINUX
#	define KB_PLATFORM_LINUX 1
#elif  defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) \
  || defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#	undef  KB_PLATFORM_IOS
#	define KB_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#	undef  KB_PLATFORM_OSX
#	define KB_PLATFORM_OSX __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#elif defined(__NX__)
#	undef  KB_PLATFORM_NX
#	define KB_PLATFORM_NX 1
#endif //

#if KB_PLATFORM_IOS
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "iOS"
#elif KB_PLATFORM_LINUX
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "Linux"
#elif KB_PLATFORM_NONE
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "None"
#elif KB_PLATFORM_NX
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "NX"
#elif KB_PLATFORM_OSX
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "OSX"
#elif KB_PLATFORM_WINDOWS
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "Windows"
#elif KB_PLATFORM_WINRT
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "WinRT"
#else
#	error "Unknown KB_PLATFORM!"
#endif // KB_PLATFORM_

//#####################################################################################################################
// Compiler
//#####################################################################################################################

#define KB_COMPILER_CLANG          0
#define KB_COMPILER_CLANG_ANALYZER 0
#define KB_COMPILER_GCC            0
#define KB_COMPILER_MSVC           0

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(__clang__)
// clang defines __GNUC__ or _MSC_VER
#	undef  KB_COMPILER_CLANG
#	define KB_COMPILER_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#	if defined(__clang_analyzer__)
#		undef  KB_COMPILER_CLANG_ANALYZER
#		define KB_COMPILER_CLANG_ANALYZER 1
#	endif // defined(__clang_analyzer__)
#elif defined(_MSC_VER)
#	undef  KB_COMPILER_MSVC
#	define KB_COMPILER_MSVC _MSC_VER
#elif defined(__GNUC__)
#	undef  KB_COMPILER_GCC
#	define KB_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#	error "KB_COMPILER_* is not defined!"
#endif //

#if KB_COMPILER_GCC
#	define KB_COMPILER_NAME "GCC "       \
    STRING(__GNUC__) "."       \
    STRING(__GNUC_MINOR__) "." \
    STRING(__GNUC_PATCHLEVEL__)
#elif KB_COMPILER_CLANG
#	define KB_COMPILER_NAME "Clang "      \
    STRING(__clang_major__) "." \
    STRING(__clang_minor__) "." \
    STRING(__clang_patchlevel__)
#elif KB_COMPILER_MSVC
#	if KB_COMPILER_MSVC >= 1920 // Visual Studio 2019
#		define KB_COMPILER_NAME "MSVC 16.0"
#	elif KB_COMPILER_MSVC >= 1910 // Visual Studio 2017
#		define KB_COMPILER_NAME "MSVC 15.0"
#	elif KB_COMPILER_MSVC >= 1900 // Visual Studio 2015
#		define KB_COMPILER_NAME "MSVC 14.0"
#	elif KB_COMPILER_MSVC >= 1800 // Visual Studio 2013
#		define KB_COMPILER_NAME "MSVC 12.0"
#	elif KB_COMPILER_MSVC >= 1700 // Visual Studio 2012
#		define KB_COMPILER_NAME "MSVC 11.0"
#	elif KB_COMPILER_MSVC >= 1600 // Visual Studio 2010
#		define KB_COMPILER_NAME "MSVC 10.0"
#	elif KB_COMPILER_MSVC >= 1500 // Visual Studio 2008
#		define KB_COMPILER_NAME "MSVC 9.0"
#	else
#		define KB_COMPILER_NAME "MSVC"
#	endif //
#endif // KB_COMPILER_


#if KB_COMPILER_MSVC
#	define KB_CPP_NAME "C++MsvcUnknown"
#elif defined(__cplusplus)
#	if __cplusplus < 201103L
#		error "Pre-C++11 compiler is not supported!"
#	elif __cplusplus < 201402L
#		define KB_CPP_NAME "C++11"
#	elif __cplusplus < 201703L
#		define KB_CPP_NAME "C++14"
#	elif __cplusplus < 201704L
#		define KB_CPP_NAME "C++17"
#	else
#		define KB_CPP_NAME "C++XX"
#	endif // KB_CPP_NAME
#else
#	define KB_CPP_NAME "C++Unknown"
#endif // defined(__cplusplus)
