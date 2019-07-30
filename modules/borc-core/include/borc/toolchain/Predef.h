
#ifndef __BORC_PREDEF_H__
#define __BORC_PREDEF_H__

#if defined(BORC_DYNAMIC)
#define BORC_API_EXPORT __declspec(dllexport)
#define BORC_API_IMPORT __declspec(dllimport)
#else 
#define BORC_API_EXPORT
#define BORC_API_IMPORT
#endif

#if defined(BORC_BUILD)
#define BORC_API BORC_API_EXPORT
#else
#define BORC_API BORC_API_IMPORT
#endif

#endif
