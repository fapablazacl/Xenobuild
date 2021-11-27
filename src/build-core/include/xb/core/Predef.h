
#ifndef __BOK_PREDEF_H__
#define __BOK_PREDEF_H__

#if defined(BOK_DYNAMIC)
#define BOK_API_EXPORT __declspec(dllexport)
#define BOK_API_IMPORT __declspec(dllimport)
#else 
#define BOK_API_EXPORT
#define BOK_API_IMPORT
#endif

#if defined(BOK_BUILD)
#define BOK_API BOK_API_EXPORT
#else
#define BOK_API BOK_API_IMPORT
#endif

#endif
