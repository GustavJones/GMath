#pragma once

#ifdef GMATH_WINDOWS
#ifdef GMATH_BUILD_DLL
#define GMATH_API __declspec(dllexport)
#else
#define GMATH_API __declspec(dllimport)
#endif
#else
#define GMATH_API
#endif
