#define SOL_PLATFORM_WIN32 0
#define SOL_PLATFORM_MAC32 1

#if defined(__WIN32__)
    #define SOL_PLATFORM SOL_PLATFORM_WIN32
#elif defined(__APPLE__)
    #define SOL_PLATFORM SOL_PLATFORM_MAC32
#endif

#include <stdio.h>
#include <stdlib.h>