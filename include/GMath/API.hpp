#pragma once

#undef GMATH_API

#if defined(_MSC_VER)
#ifdef GMATH_SHARED
#ifdef GMATH_DLL
#define GMATH_API __declspec(dllexport)
#else
#define GMATH_API __declspec(dllimport)
#endif

#else
#define GMATH_API
#endif

#elif defined(__GNUC__)

#ifdef GMATH_SHARED

#ifdef GMATH_DLL
#define GMATH_API __attribute__((visibility("default")))
#else
#define GMATH_API
#endif

#else
#define GMATH_API
#endif

#else
#ifdef GMATH_SHARED
#error Compiler not supported.
#else 
#define GMATH_API
#endif
#endif
