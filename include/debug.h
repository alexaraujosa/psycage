/***************************************************************
 * DEBUG UTILITY                                               *
 *                                                             *
 * This utility handles debug processes for this project.      *
 ***************************************************************/


#ifndef __RL_DEBUG_H
#define __RL_DEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

#define IGNORE_ARG(x) (void)(x)

extern int DEBUG_LEVEL; // 0 - Minimal | 1 - Expanded | 2 - All
// #define DEBUG_LEVEL 0

/*
 * Similar behavior to `printf`.
 */
static inline void debug(const char* format, ...);

/*
 * Similar behavior to `debug`, but outputs to a specificed file.
 */
static inline void debug_file(FILE* file, int level, const char* format, ...);

FILE* make_debug_file(char* bin, int bin_len, char* filename);

// Implementation
#ifdef RL_DEBUG

#include <stdio.h>
#include <stdarg.h>

static inline void debug(const char* format, ...) {
    va_list valist;

    va_start(valist, format);
    vprintf(format, valist);
    va_end(valist);
}

static inline void debug_file(FILE* file, int level, const char* format, ...) {
    if (file == NULL) return;
    if (level > DEBUG_LEVEL) return;

    va_list valist;

    va_start(valist, format);
    vfprintf(file, format, valist);
    va_end(valist);

    fflush(file);
}

#else

static inline void debug(const char* format, ...) {
    IGNORE_ARG(format);
    __asm__("nop");
}

static inline void debug_file(FILE* file, int level, const char* format, ...) {
    IGNORE_ARG(file);
    IGNORE_ARG(level);
    IGNORE_ARG(format);
    __asm__("nop");
}


#endif

#endif