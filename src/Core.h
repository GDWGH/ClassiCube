#ifndef CC_CORE_H
#define CC_CORE_H
/* Core fixed-size integer types, automatic platform detection, and common small structs.
   Copyright 2014-2019 ClassiCube | Licensed under BSD-3
*/

#if _MSC_VER
typedef signed __int8  cc_int8;
typedef signed __int16 cc_int16;
typedef signed __int32 cc_int32;
typedef signed __int64 cc_int64;

typedef unsigned __int8  cc_uint8;
typedef unsigned __int16 cc_uint16;
typedef unsigned __int32 cc_uint32;
typedef unsigned __int64 cc_uint64;
#ifdef _WIN64
typedef unsigned __int64 cc_uintptr;
#else
typedef unsigned int     cc_uintptr;
#endif

#define CC_INLINE inline
#define CC_NOINLINE __declspec(noinline)
#ifndef CC_API
#define CC_API __declspec(dllexport, noinline)
#define CC_VAR __declspec(dllexport)
#endif

#define CC_HAS_TYPES
#define CC_HAS_MISC
#elif __GNUC__
/* really old GCC/clang might not have these defined */
#ifdef __INT8_TYPE__
/* avoid including <stdint.h> because it breaks defining UNICODE in Platform.c with MinGW */
typedef __INT8_TYPE__  cc_int8;
typedef __INT16_TYPE__ cc_int16;
typedef __INT32_TYPE__ cc_int32;
typedef __INT64_TYPE__ cc_int64;

#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__   cc_uint8;
typedef __UINT16_TYPE__  cc_uint16;
typedef __UINT32_TYPE__  cc_uint32;
typedef __UINT64_TYPE__  cc_uint64;
typedef __UINTPTR_TYPE__ cc_uintptr;
#else
/* clang doesn't define the __UINT8_TYPE__ */
typedef unsigned __INT8_TYPE__   cc_uint8;
typedef unsigned __INT16_TYPE__  cc_uint16;
typedef unsigned __INT32_TYPE__  cc_uint32;
typedef unsigned __INT64_TYPE__  cc_uint64;
typedef unsigned __INTPTR_TYPE__ cc_uintptr;
#endif
#define CC_HAS_TYPES
#endif

#define CC_INLINE inline
#define CC_NOINLINE __attribute__((noinline))
#ifndef CC_API
#ifdef _WIN32
#define CC_API __attribute__((dllexport, noinline))
#define CC_VAR __attribute__((dllexport))
#else
#define CC_API __attribute__((visibility("default"), noinline))
#define CC_VAR __attribute__((visibility("default")))
#endif
#endif
#define CC_HAS_MISC
#ifdef __BIG_ENDIAN__
#define CC_BIG_ENDIAN
#endif
#elif __MWERKS__
/* TODO: Is there actual attribute support for CC_API etc somewhere? */
#define CC_BIG_ENDIAN
#endif

/* Unrecognised compiler, so just go with sensible defaults */
#ifndef CC_HAS_TYPES
#include <stdint.h>
typedef int8_t  cc_int8;
typedef int16_t cc_int16;
typedef int32_t cc_int32;
typedef int64_t cc_int64;

typedef uint8_t   cc_uint8;
typedef uint16_t  cc_uint16;
typedef uint32_t  cc_uint32;
typedef uint64_t  cc_uint64;
typedef uintptr_t cc_uintptr;
#endif
#ifndef CC_HAS_MISC
#define CC_INLINE
#define CC_NOINLINE
#define CC_API
#define CC_VAR
#endif

typedef cc_uint16 Codepoint;
typedef cc_uint8  cc_bool;
#ifdef __APPLE__
/* TODO: REMOVE THIS AWFUL AWFUL HACK */
#include <stdbool.h>
#elif __cplusplus
#else
#define true 1
#define false 0
#endif

#ifndef NULL
#if __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

#define EXTENDED_BLOCKS
#ifdef EXTENDED_BLOCKS
typedef cc_uint16 BlockID;
#else
typedef cc_uint8 BlockID;
#endif

#define EXTENDED_TEXTURES
#ifdef EXTENDED_TEXTURES
typedef cc_uint16 TextureLoc;
#else
typedef cc_uint8 TextureLoc;
#endif

typedef cc_uint8 BlockRaw;
typedef cc_uint8 EntityID;
typedef cc_uint8 Face;
typedef cc_uint32 cc_result;
typedef cc_uint64 TimeMS;

typedef struct Rect2D_  { int X, Y, Width, Height; } Rect2D;
typedef struct TextureRec_ { float U1, V1, U2, V2; } TextureRec;

/*#define CC_BUILD_GL11*/
#ifndef CC_BUILD_MANUAL
#if defined _WIN32
#define CC_BUILD_WIN
#define CC_BUILD_D3D9
#define CC_BUILD_WINGUI
#define CC_BUILD_WGL
#define CC_BUILD_WININET
#define CC_BUILD_WINMM
#elif defined __ANDROID__
#define CC_BUILD_ANDROID
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#define CC_BUILD_GLMODERN
#define CC_BUILD_GLES
#define CC_BUILD_EGL
#define CC_BUILD_TOUCH
#define CC_BUILD_NOAUDIO
#elif defined __linux__
#define CC_BUILD_LINUX
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#define CC_BUILD_X11
#define CC_BUILD_CURL
#define CC_BUILD_OPENAL
#if defined CC_BUILD_RPI
#define CC_BUILD_GLMODERN
#define CC_BUILD_GLES
#define CC_BUILD_EGL
#endif
#elif defined __APPLE__
#define CC_BUILD_OSX
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#ifdef __x86_64__
#define CC_BUILD_COCOA
#else
#define CC_BUILD_CARBON
#endif
#define CC_BUILD_CURL
#define CC_BUILD_OPENAL
#elif defined __sun__
#define CC_BUILD_SOLARIS
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#define CC_BUILD_X11
#define CC_BUILD_CURL
#define CC_BUILD_OPENAL
#elif defined __FreeBSD__ || defined __DragonFly__
#define CC_BUILD_FREEBSD
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#define CC_BUILD_X11
#define CC_BUILD_CURL
#define CC_BUILD_OPENAL
#elif defined __OpenBSD__
#define CC_BUILD_OPENBSD
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#define CC_BUILD_X11
#define CC_BUILD_CURL
#define CC_BUILD_OPENAL
#elif defined __NetBSD__
#define CC_BUILD_NETBSD
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#define CC_BUILD_X11
#define CC_BUILD_CURL
#define CC_BUILD_OPENAL
#elif defined __HAIKU__
#define CC_BUILD_HAIKU
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#define CC_BUILD_CURL
#define CC_BUILD_OPENAL
#define CC_BUILD_SDL
#elif defined __EMSCRIPTEN__
#define CC_BUILD_WEB
#define CC_BUILD_POSIX
#define CC_BUILD_GL
#define CC_BUILD_GLMODERN
#define CC_BUILD_GLES
#define CC_BUILD_TOUCH
#define CC_BUILD_NOAUDIO
#define CC_BUILD_MINFILES
#endif
#endif

#ifdef CC_BUILD_D3D9
typedef void* GfxResourceID;
#else
/* Ensure size is same as D3D9, even though only 32 bits are used */
/* NOTE: OpenGL 1.1 does actually use the full 64 bits for 'dynamic' vertex buffers */
typedef cc_uintptr GfxResourceID;
#endif

/* Contains the information to describe a 2D textured quad. */
struct Texture {
	GfxResourceID ID;
	cc_int16 X, Y; cc_uint16 Width, Height;
	TextureRec uv;
};
#endif
