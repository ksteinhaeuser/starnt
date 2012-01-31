#if !defined glob_warnings_h_included
#define glob_warnings_h_included

#include "glob/glob.compiler.h"

#if glob_compiler_msvc
#pragma warning(disable: 4127) // (level 4) C4127: conditional expression is constant
//#pragma warning(disable: 4146) // (level 2) C4146: unary minus operator applied to unsigned type, result still unsigned
//#pragma warning(disable: 4251) // (level 1) C4251: 'identifier' : class 'class1' needs to have dll-interface to be used by clients of class 'class2'
//#pragma warning(disable: 4275) // (level 2) C4275: non dll-interface class 'class1' used as base for dll-interface class 'class2'
#pragma warning(disable: 4505) // (level 4) C4505: 'function' : unreferenced local function has been removed
//#pragma warning(disable: 4333) // (level 1) C4333: 'operator' : right shift by too large amount, data loss
//#pragma warning(disable: 4511) // (level 3) C4511: 'class' : copy constructor could not be generated
//#pragma warning(disable: 4512) // (level 4) C4512: 'class' : assignment operator could not be generated
//#pragma warning(disable: 4702) // (level 4) C4702: unreachable code
//#pragma warning(disable: 4996) // (level 3) C4996: 'function': was declared deprecated
#endif

#endif

