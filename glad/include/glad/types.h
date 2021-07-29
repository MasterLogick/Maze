#ifndef GLAD_TYPES_H_
#define GLAD_TYPES_H_

#include "../KHR/khrplatform.h"
#include "platform.h"

typedef unsigned int GLenum;

typedef unsigned char GLboolean;

typedef unsigned int GLbitfield;

typedef void GLvoid;

typedef khronos_int8_t GLbyte;

typedef khronos_uint8_t GLubyte;

typedef khronos_int16_t GLshort;

typedef khronos_uint16_t GLushort;

typedef int GLint;

typedef unsigned int GLuint;

typedef khronos_int32_t GLclampx;

typedef int GLsizei;

typedef khronos_float_t GLfloat;

typedef khronos_float_t GLclampf;

typedef double GLdouble;

typedef double GLclampd;

typedef void *GLeglClientBufferEXT;

typedef void *GLeglImageOES;

typedef char GLchar;

typedef char GLcharARB;

#ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif

typedef khronos_uint16_t GLhalf;

typedef khronos_uint16_t GLhalfARB;

typedef khronos_int32_t GLfixed;

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef khronos_intptr_t GLintptr;
#else
typedef khronos_intptr_t GLintptr;
#endif

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef khronos_intptr_t GLintptrARB;
#else
typedef khronos_intptr_t GLintptrARB;
#endif

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef khronos_ssize_t GLsizeiptr;
#else
typedef khronos_ssize_t GLsizeiptr;
#endif

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef khronos_ssize_t GLsizeiptrARB;
#else
typedef khronos_ssize_t GLsizeiptrARB;
#endif

typedef khronos_int64_t GLint64;

typedef khronos_int64_t GLint64EXT;

typedef khronos_uint64_t GLuint64;

typedef khronos_uint64_t GLuint64EXT;

typedef struct __GLsync *GLsync;

struct _cl_context;

struct _cl_event;

typedef void (GLAD_API_PTR *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

typedef void (GLAD_API_PTR *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

typedef void (GLAD_API_PTR *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

typedef void (GLAD_API_PTR *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);

typedef unsigned short GLhalfNV;

typedef GLintptr GLvdpauSurfaceNV;

typedef void (GLAD_API_PTR *GLVULKANPROCNV)(void);

#endif //GLAD_TYPES_H_
