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
#    ifdef ERA_EXPORT
#        define ERA_API __declspec(dllexport)
#    else
#        define ERA_API __declspec(dllimport)
#    endif
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
#    ifdef ERA_EXPORTS
#        define ERA_API __attribute__((visibility("default")))
#    else
#        define ERA_API
#    endif
#else
#    define ERA_API
#endif

ERA_API void EngineInit(void);

#endif // ENGINE_H
