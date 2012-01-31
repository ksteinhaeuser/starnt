#if !defined glob_endianness_h_included
#define glob_endianness_h_included

#include "glob/glob.stdint.h"

// Endianness.
static const uint32_t deadbeef = 0xdeadbeef;
#define glob_endianness_big (*(uchar_t*)&deadbeef == 0xde)
#define glob_endianness_little (*(uchar_t*)&deadbeef == 0xef)

#endif

