#ifndef DEFINES_H
#define DEFINES_H

#ifndef NULL
#    define NULL ((void *)0)
#endif

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
#    define PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#    include <TargetConditionals.h>
#    if TARGET_OS_IPHONE
#        define PLATFORM_IOS
#    elif TARGET_OS_MAC
#        define PLATFORM_MACOS
#    endif
#elif defined(__ANDROID__)
#    define PLATFORM_ANDROID
#elif defined(__linux__)
#    define PLATFORM_LINUX
#elif defined(__NINTENDO__)
#    define PLATFORM_NINTENDO
#elif defined(__XBOX__)
#    define PLATFORM_XBOX
#elif defined(__PLAYSTATION__)
#    define PLATFORM_PLAYSTATION
#else
#    error "Unknown platform"
#endif

// Architecture detection
#if defined(__x86_64__) || defined(_M_X64)
#    define ENGINE_ARCH_X64
#elif defined(__i386) || defined(_M_IX86)
#    define ENGINE_ARCH_X86
#elif defined(__arm__) || defined(_M_ARM)
#    define ENGINE_ARCH_ARM
#elif defined(__aarch64__) || defined(_M_ARM64)
#    define ENGINE_ARCH_ARM64
#else
#    error "Unknown architecture"
#endif

// API export/import
#if defined(PLATFORM_WINDOWS)
#    ifdef ENGINE_EXPORT
#        define ENGINE_API __declspec(dllexport)
#    else
#        define ENGINE_API __declspec(dllimport)
#    endif
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
#    ifdef ENGINE_EXPORT
#        define ENGINE_API __attribute__((visibility("default")))
#    else
#        define ENGINE_API
#    endif
#else
#    define ENGINE_API
#endif

// Standard macros
#define ENGINE_GLOBAL static   // Global variable.
#define ENGINE_INTERNAL static // Internal function.

// Mutex macros
#if defined(PLATFORM_WINDOWS)
#    include <Windows.h>
#    define ENGINE_MUTEX CRITICAL_SECTION
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
#    include <pthread.h>
#    define ENGINE_MUTEX pthread_mutex_t
#endif

#define ENGINE_UNUSED(x) (void)(x)                               // To avoid unused parameter warnings.
#define ENGINE_ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0])) // Get array element count.
#define ENGINE_ALIGN(x) __attribute__((aligned(x)))              // Align data to x bytes.
#define ENGINE_INLINE inline                                     // Inline function.

// Common typedefs
#include <stdbool.h>
#include <stdint.h>

/** @brief Unsigned 8-bit integer. */
typedef unsigned char u8;
/** @brief Unsigned 16-bit integer. */
typedef unsigned short u16;
/** @brief Unsigned 32-bit integer. */
typedef unsigned int u32;
/** @brief Unsigned 64-bit integer. */
typedef unsigned long long u64;

/** @brief Signed 8-bit integer. */
typedef signed char i8;
/** @brief Signed 16-bit integer. */
typedef signed short i16;
/** @brief Signed 32-bit integer. */
typedef signed int i32;
/** @brief Signed 64-bit integer. */
typedef signed long long i64;

/** @brief 32-bit floating point number. */
typedef float f32;
/** @brief 64-bit floating point number. */
typedef double f64;

/** @brief Boolean type. */
typedef _Bool b8;

/** @brief True value. */
#define true 1
/** @brief False value. */
#define false 0

// Return codes
typedef enum EngineResult {
    ENGINE_SUCCESS = 0,
    ENGINE_ERROR,
    ENGINE_ERROR_ALLOCATION_FAILED,
    ENGINE_ERROR_INVALID_ARGUMENT,
} EngineResult;

#define ENGINE_SUCCESS 0
#define ENGINE_FAILURE 1

// Min and max limits
#define MIN_U8 0U
#define MIN_U16 0U
#define MIN_U32 0U
#define MIN_U64 0UL
#define MAX_U8 255U
#define MAX_U16 65535U
#define MAX_U32 4294967295U
#define MAX_U64 18446744073709551615UL

#define MIN_I8 -128
#define MIN_I16 -32768
#define MIN_I32 -2147483648
#define MIN_I64 -9223372036854775808L
#define MAX_I8 127
#define MAX_I16 32767
#define MAX_I32 2147483647
#define MAX_I64 9223372036854775807L

// Invalid values
#define INVALID_ID_U8 MAX_U8
#define INVALID_ID_U16 MAX_U16
#define INVALID_ID_U32 MAX_U32
#define INVALID_ID_U64 MAX_U64

// Memory macros

#include <memory.h>

#define ENGINE_ALLOC(size) malloc(size)
#define ENGINE_FREE(ptr) free(ptr)
#define ENGINE_COPY(dest, src, size) memcpy(dest, src, size)
#define ENGINE_SET(ptr, value, size) memset(ptr, value, size)
#define ENGINE_ZERO(ptr, size) ENGINE_SET(ptr, 0, size)

// Error handling macros
#ifdef _DEBUG
#    define ENGINE_ASSERT(condition, message)                               \
        do {                                                                \
            if (!(condition)) {                                             \
                log_error("[ASSERT] %s\nFile: %s\nLine: %d\nMessage: %s\n", \
                          #condition, __FILE__, __LINE__, message);         \
                abort();                                                    \
            }                                                               \
        } while (0)
#else
#    define ENGINE_ASSERT(condition, message) ((void)0)
#endif

// Unused parameter macro
#define ENGINE_UNUSED(x) (void)(x)

#endif // DEFINES_H
