#if !defined glob_inline_h_included
#define glob_inline_h_included

#include "glob/glob.compiler.h"

#if glob_compiler_gcc 
#define glob_inline __attribute__((always_inline)) 
#define glob_noinline __attribute__((noinline)) 
#elif glob_compiler_msvc 
#define glob_inline __forceinline 
#define glob_noinline __declspec(noinline) 
#else 
#define glob_inline inline 
#define glob_noinline 
#endif 

#endif

