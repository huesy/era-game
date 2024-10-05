#ifndef ENGINE_H
#define ENGINE_H

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
#    ifdef ENGINE_EXPORTS
#        define ENGINE_API __attribute__((visibility("default")))
#    else
#        define ENGINE_API
#    endif
#else
#    define ENGINE_API
#endif

// Standard macros
#define ENGINE_UNUSED(x) (void)(x)                               // To avoid unused parameter warnings.
#define ENGINE_ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0])) // Get array element count.
#define ENGINE_ALIGN(x) __attribute__((aligned(x)))              // Align data to x bytes.
#define ENGINE_INLINE inline                                     // Inline function.

// Common typedefs
#include <stdbool.h>
#include <stdint.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

// Memory macros
#define ENGINE_ALLOC(size) malloc(size)
#define ENGINE_FREE(ptr) free(ptr)

// Error handling macros
#define ENGINE_ASSERT(cond, msg)                                                                  \
    do {                                                                                          \
        if (!(cond)) {                                                                            \
            fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", msg, __FILE__, __LINE__); \
            abort();                                                                              \
        }                                                                                         \
    } while (0)

// Function declarations
ENGINE_API void EngineInit(void);
ENGINE_API void EngineShutdown(void);

#endif // ENGINE_H
