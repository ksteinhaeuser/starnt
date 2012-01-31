#if !defined glob_hint_h_included
#define glob_hint_h_included

#include "glob/glob.debug.h"

// A notification that applies only in debug builds.
#if glob_debug
#include <iostream>
#define glob_hint(text) { \
	std::cout << "hint: " << text << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
}
#else
#define glob_hint(text) { \
}
#endif

#endif

