#if !defined glob_stdint_h_included
#define glob_stdint_h_included

#include <stddef.h> // size_t, ptrdiff_t
#include "glob/glob.compiler.h"

// Fixed-size (C99) integer types.
#if glob_compiler_msvc
//typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif
typedef unsigned char uchar_t;
typedef float float32_t;
typedef double float64_t;

#endif

