#if !defined glob_inline_h_included
#define glob_inline_h_included

#include "glob/glob.compiler.h"
#include "glob/glob.warnings.h"

/* A macro to suppress unused-variable compiler warnings. This is handy for situations where data or function arguments
are used in some build configurations but not others, for example, only in debug builds.
*/
#if glob_compiler_msvc
#define glob_unused(x) do { (void)(x); } while(0)
#else
#define glob_unused(x) do { (void)sizeof(x); } while(0)
#endif

#endif

