#include "log.h"

#include <stdarg.h>
#include <stdio.h>


void debug_log(const char* format, ...) {
#ifdef DEBUG_LOG
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif
}