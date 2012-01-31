#if !defined glob_compiler_h_included
#define glob_compiler_h_included

#define glob_compiler_msvc (defined _MSC_VER)
#define glob_compiler_gcc (defined __GNUC__)

#if glob_compiler_msvc
#define NOMINMAX // prevent min and max macros from being defined deep in the unsightly bowels of <windows.h>
#endif

#endif

