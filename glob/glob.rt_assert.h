#if !defined glob_rt_assert_n_included
#define glob_rt_assert_n_included

#include "glob/glob.compiler.h"
#include "glob/glob.debug.h"
#include "glob/glob.warnings.h"

/* Runtime assert macro. Has two advantages over the standard assert:
(1) emits zero bytes of code in release builds (which is not the case when using the standard msvc assert),
(2) when triggered, stops program execution at the line where it is located rather than several calls deep in a library routine.
*/
#if glob_compiler_msvc
#define _glob_halt() __debugbreak()
#else
#define _glob_halt() raise(SIGINT)
#endif
#if defined rt_assert
#undef rt_assert
#endif
#if glob_debug
#include <stdio.h>
#define rt_assert(condition) \
	do { \
		if (!(condition)) { \
			std::cout << __FILE__ << "(" << __LINE__ << "): assert '" << #condition << "' failed" << std::endl; \
			_glob_halt(); \
		} \
	} while(0)
#else
#define rt_assert(condition) glob_unused(condition)
#endif

///printf("%s(%d): assert '%s' failed\n", __FILE__, __LINE__, #condition);

#endif

