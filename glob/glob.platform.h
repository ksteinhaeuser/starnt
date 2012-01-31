#if !defined glob_platform_h_included
#define glob_platform_h_included

#define glob_platform_win32 (defined _WIN32 || defined _WIN64)
#define glob_platform_win64 (defined _WIN64)
#define glob_platform_cygwin (defined __GNUC__ && defined __CYGWIN32__)

#define glob_platform_unix (defined __GNUC__ && !defined __APPLE_CC__)

#define glob_platform_mac (defined __GNUC__ && defined __APPLE_CC__)
#define glob_platform_mac64 (defined __GNUC__ && defined __APPLE_CC__ && defined __x86_64__)

#define glob_platform_x64 ((defined __GNUC__ && defined __x86_64__) || glob_platform_win64)

#endif

