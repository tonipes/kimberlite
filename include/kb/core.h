// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>


//#####################################################################################################################
// API
//#####################################################################################################################


#define KB_API 
#define KB_API_INLINE inline static
#define KB_INTERNAL   static


//#####################################################################################################################
// Assert
//#####################################################################################################################


#ifndef KB_NOT_IMPLEMENTED
  #define KB_NOT_IMPLEMENTED(m) assert(false && "Not implemented. " m)
#endif

#ifndef KB_ASSERT
  #define KB_ASSERT(e, m) assert(e && m)
#endif

#ifndef KB_ASSERT_NOT_NULL
  #define KB_ASSERT_NOT_NULL(e) assert(e && "Invalid NULL: '" #e "'")
#endif


//#####################################################################################################################
// Utils
//#####################################################################################################################


#define KB_ENUM_GEN(ENUM) ENUM,
#define KB_ENUM_GEN_STR(STRING) #STRING,

#define KB_ENUM_OF(name, def) enum name { def(KB_ENUM_GEN) }; static const char* name##keys[] = { def(KB_ENUM_GEN_STR) };

#define KB_ENUM_FLAG_OPERATORS(_T) \
  template<class _T> constexpr inline _T operator~ (_T a) { return (_T)~(int)a; } \
  template<class _T> constexpr inline _T operator| (_T a, _T b) { return (_T)((int)a | (int)b); } \
  template<class _T> constexpr inline _T operator& (_T a, _T b) { return (_T)((int)a & (int)b); } \
  template<class _T> constexpr inline _T operator^ (_T a, _T b) { return (_T)((int)a ^ (int)b); } \
  template<class _T> constexpr inline _T& operator|= (_T& a, _T b) { return (_T&)((int&)a |= (int)b); } \
  template<class _T> constexpr inline _T& operator&= (_T& a, _T b) { return (_T&)((int&)a &= (int)b); } \
  template<class _T> constexpr inline _T& operator^= (_T& a, _T b) { return (_T&)((int&)a ^= (int)b); }

#define KB_COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))

#define KB_FOURCC(a, b, c, d) (((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24)))

#define STRING(_x) STRING_(_x)
#define STRING_(_x) #_x

#define KB_KILO(value) ((value)/1024LL)
#define KB_MEGA(value) (KB_KILO(value)/1024LL)
#define KB_GIGA(value) (KB_MEGA(value)/1024LL)
#define KB_TERA(value) (KB_GIGA(value)/1024LL)


//#####################################################################################################################
// Build
//#####################################################################################################################


#define KB_BUILD_MODE_DEBUG    0
#define KB_BUILD_MODE_RELEASE  0

#if defined(DEBUG) \
  || defined(_DEBUG)
# undef  KB_BUILD_MODE_DEBUG
# define KB_BUILD_MODE_DEBUG 1
#else
# undef  KB_BUILD_MODE_RELEASE
# define KB_BUILD_MODE_RELEASE 1
#endif

#if defined(NDEBUG)
# undef  KB_BUILD_MODE_RELEASE
# define KB_BUILD_MODE_RELEASE 1
#endif

#if KB_BUILD_MODE_DEBUG
#	define KB_BUILD_MODE_NAME "Debug"
#elif KB_BUILD_MODE_RELEASE
#	define KB_BUILD_MODE_NAME "Release"
#endif


//#####################################################################################################################
// CPU
//#####################################################################################################################


#define KB_CPU_ARM   0
#define KB_CPU_X86   0

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
#endif


//#####################################################################################################################
// Platform
//#####################################################################################################################


#define KB_PLATFORM_LINUX      0
#define KB_PLATFORM_MACOS      0
#define KB_PLATFORM_WINDOWS    0
#define KB_PLATFORM_IOS        0
#define KB_PLATFORM_NX         0
#define KB_PLATFORM_PS         0
#define KB_PLATFORM_XBO        0
#define KB_PLATFORM_CONSOLE    0
#define KB_PLATFORM_NAME       "Unknown"

#if defined(_WIN32) || defined(_WIN64)
#	undef  KB_PLATFORM_WINDOWS
#	define KB_PLATFORM_WINDOWS 1
#elif  defined(__linux__)
#	undef  KB_PLATFORM_LINUX
#	define KB_PLATFORM_LINUX 1
#elif __APPLE__
  #include <TargetConditionals.h>
  #if TARGET_OS_IPHONE
    #	undef  KB_PLATFORM_IOS
    #	define KB_PLATFORM_IOS 1
  #else
    #	undef  KB_PLATFORM_MACOS
    #	define KB_PLATFORM_MACOS 1
  #endif
#elif defined(__NX__)
#	undef  KB_PLATFORM_NX
#	define KB_PLATFORM_NX 1
#elif defined(__ORBIS__)
#	undef  KB_PLATFORM_PS
#	define KB_PLATFORM_PS 1
#elif defined(_DURANGO)
#	undef  KB_PLATFORM_XBO
#	define KB_PLATFORM_XBO 1
#endif //

#if KB_PLATFORM_NX || KB_PLATFORM_PS || KB_PLATFORM_XBO
#	undef  KB_PLATFORM_CONSOLE
#	define KB_PLATFORM_CONSOLE 1
#endif

#if KB_PLATFORM_IOS
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "iOS"
#elif KB_PLATFORM_LINUX
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "Linux"
#elif KB_PLATFORM_NX
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "NX"
#elif KB_PLATFORM_PS
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "PS"
#elif KB_PLATFORM_XBO
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "XBO"
#elif KB_PLATFORM_MACOS
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "macOS"
#elif KB_PLATFORM_WINDOWS
#	undef  KB_PLATFORM_NAME
#	define KB_PLATFORM_NAME "Windows"
#else
#	error "Unknown KB_PLATFORM"
#endif


//#####################################################################################################################
// Compiler
//#####################################################################################################################


#define KB_COMPILER_CLANG   0
#define KB_COMPILER_GCC     0
#define KB_COMPILER_MSVC    0

#if defined(__clang__)
#	undef  KB_COMPILER_CLANG
#	define KB_COMPILER_CLANG 1
#elif defined(_MSC_VER)
#	undef  KB_COMPILER_MSVC
#	define KB_COMPILER_MSVC 1
#elif defined(__GNUC__)
#	undef  KB_COMPILER_GCC
#	define KB_COMPILER_GCC 1
#else
#	error "KB_COMPILER_* is not defined!"
#endif

#if KB_COMPILER_GCC
#	define KB_COMPILER_NAME "GCC " STRING(__GNUC__) "." STRING(__GNUC_MINOR__) "." STRING(__GNUC_PATCHLEVEL__)
#elif KB_COMPILER_CLANG
#	define KB_COMPILER_NAME "Clang " STRING(__clang_major__) "." STRING(__clang_minor__) "." STRING(__clang_patchlevel__)
#elif KB_COMPILER_MSVC
# define KB_COMPILER_NAME "MSVC"
#endif

#if defined(__cplusplus)
#	if __cplusplus < 201103L
#		define KB_CPP_NAME "C++Pre11"
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
#	define KB_CPP_NAME "Unknown"
#endif


//#####################################################################################################################
// Graphics Library
//#####################################################################################################################


#define KB_GRAPHICS_LIB_VULKAN                  0
#define KB_GRAPHICS_LIB_METAL                   0
#define KB_GRAPHICS_LIB_NOOP                    0
#define KB_GRAPHICS_LIB_NAME                    "Unknown"

#if defined(KB_SETUP_GRAPHICS_LIB_VULKAN)
#	undef  KB_GRAPHICS_LIB_VULKAN
#	define KB_GRAPHICS_LIB_VULKAN 1
#elif defined(KB_SETUP_GRAPHICS_LIB_METAL)
#	undef  KB_GRAPHICS_LIB_METAL
#	define KB_GRAPHICS_LIB_METAL 1
#elif defined(KB_SETUP_GRAPHICS_LIB_NOOP)
#	undef  KB_GRAPHICS_LIB_NOOP
#	define KB_GRAPHICS_LIB_NOOP 1
#endif

#if KB_GRAPHICS_LIB_VULKAN
#	undef  KB_GRAPHICS_LIB_NAME
#	define KB_GRAPHICS_LIB_NAME "Vulkan"
#elif KB_GRAPHICS_LIB_METAL
#	undef  KB_GRAPHICS_LIB_NAME
#	define KB_GRAPHICS_LIB_NAME "Metal"
#elif KB_GRAPHICS_LIB_NOOP
#	undef  KB_GRAPHICS_LIB_NAME
#	define KB_GRAPHICS_LIB_NAME "Noop"
// #else
// #	error "Unknown KB_GRAPHICS_LIB!"
#endif


//#####################################################################################################################
// Config
//#####################################################################################################################


#define KB_CONFIG_MAX_GAMEPADS                  8
#define KB_CONFIG_MAX_RENDERPASSES              8
#define KB_CONFIG_MAX_RENDERPASS_ATTACHMENTS    8
#define KB_CONFIG_MAX_GIZMOS                    8
#define KB_CONFIG_MAX_BUFFERS                   512
#define KB_CONFIG_MAX_MATERIALS                 512
#define KB_CONFIG_MAX_MESHES                    512
#define KB_CONFIG_MAX_PROGRAMS                  512
#define KB_CONFIG_MAX_COMMAND_BUFFERS           512
#define KB_CONFIG_MAX_TEXTURES                  512
#define KB_CONFIG_MAX_SOUNDS                    512
#define KB_CONFIG_MAX_NAME_SIZE                 512
#define KB_CONFIG_MAX_FONTS                     512
#define KB_CONFIG_MAX_GEOMS                     512
#define KB_CONFIG_MAX_AUDIO_TRACKS              512

#define KB_CONFIG_MAX_UNIFORM_BLOCK_SIZE        256
#define KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS   8
#define KB_CONFIG_MAX_VERTEX_ATTRIB_BUFFERS     8
#define KB_CONFIG_MAX_VERTEX_ATTRIBS            8 * KB_CONFIG_MAX_VERTEX_ATTRIB_BUFFERS
#define KB_CONFIG_MAX_UNIFORM_BINDINGS          8

#define KB_CONFIG_MAX_DRAW_CALLS                128
#define KB_CONFIG_MAX_ENCODERS                  8
#define KB_CONFIG_MAX_PASS_COLOR_ATTACHMENTS    8
#define KB_CONFIG_MAX_PIPE_ATTACHMENTS          16
#define KB_CONFIG_MAX_PASSES                    8
#define KB_CONFIG_TRANSIENT_BUFFER_SIZE         16 * 1024 * KB_CONFIG_MAX_DRAW_CALLS
#define KB_CONFIG_ENGINE_NAME                   "Kimberlite"
#define KB_CONFIG_STATS_SAMPLE_COUNT            120
#define KB_CONFIG_GIZMO_CACHE_SIZE              4096
#define KB_CONFIG_GIZMO_STACK_SIZE              32
#define KB_CONFIG_MAX_FRAMES_IN_FLIGHT          3

#define KB_CONFIG_FILE_MAGIC_GEOM               KB_FOURCC('K', 'B', 'G', 'E')
#define KB_CONFIG_FILE_MAGIC_TEX                KB_FOURCC('K', 'B', 'T', 'X')
#define KB_CONFIG_FILE_MAGIC_FONT               KB_FOURCC('K', 'B', 'F', 'N')

#define KB_CONFIG_AXIS_DEADZONE                 0.25
#define KB_CONFIG_GRAPHICS_VALIDATE             1

#define KB_CONFIG_ALLOC_DEBUG 0
#if defined(KB_BUILD_MODE_DEBUG)
# undef  KB_CONFIG_ALLOC_DEBUG
# define KB_CONFIG_ALLOC_DEBUG 1
#endif
