#pragma once

#define TEXT(s) #s

// Expands to macro
#if DEBUG_ENABLED && _MSC_VER

#ifndef _CRT_STRINGIZE
#define _CRT_STRINGIZE_(x) #x
#define _CRT_STRINGIZE(x) _CRT_STRINGIZE_(x)
#endif

#define EXPAND_MACRO(x) __pragma(message(__FILE__ _CRT_STRINGIZE((__LINE__) \
																 : \nmacro\t) #x " expands to:\n" _CRT_STRINGIZE(x)))

#else
#define EXPAND_MACRO(x)
#endif

// 
#if _MSC_VER

#define MSVC_WARNING_DISABLE(n) __pragma(warning(disable \
												 : n))
#define MSVC_WARNING_RESTORE(n) __pragma(warning(default \
												 : n))

#else
#define MSVC_WARNING_DISABLE(n)
#define MSVC_WARNING_RESTORE(n)
#endif
