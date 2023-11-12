#pragma once

#include <stdio.h>

#ifdef DEBUG_MODE
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(...)
#endif
