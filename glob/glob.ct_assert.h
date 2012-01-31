#if !defined glob_ct_assert_h_included
#define glob_ct_assert_h_included

// Compile-time assert macro.
#if defined(ct_assert)
#undef ct_assert
#endif
#define ct_assert(x) typedef char _glob_ct_assert[(x) ? 1 : -1]; /* expect "error C2118: negative subscript" */
//#define ct_assert(condition) enum { _glob_rt_assert = 0 / (int)(condition) }; /* expect "error C2057: expected constant expression" */

#endif

