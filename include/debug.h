/***************************************************************
 * DEBUG UTILITY                                               *
 *                                                             *
 * This utility handles debug processes for this project.      *
 ***************************************************************/


#ifndef __RL_DEBUG_H
#define __RL_DEBUG_H

#include <stdio.h>
#include <stdarg.h>

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

// Implementation
#ifdef RL_DEBUG

#include <stdio.h>
#include <stdarg.h>

// static inline char* __dbg_convert(unsigned int num, int base) { 
//     static char rep[] = "0123456789ABCDEF";
//     static char buf[50]; 

//     char *ptr; 
//     ptr = &buf[49]; 
//     *ptr = '\0'; 

//     do  { 
//         *(--ptr) = rep[num % base];
//         num /= base; 
//     } while(num != 0); 

//     return(ptr); 
// }

// static inline void _debug(FILE* stream, const char* format, ...) {
//     va_list valist;

//     va_start(valist, format);

//     while(*format != '\0') {
//         if(*format != '%') {
//             putchar(*format);
//             format++;
//             continue;
//         }

//         format++;

//         switch(*format) {
//             case 's': fputs(va_arg(valist, char*), stream); break;
//             case 'c': putchar(va_arg(valist, int)); break;
//             case 'd' : {
//                 int i = va_arg(valist, int);
//                 if(i < 0) { 
//                     i = -i;
//                     putchar('-'); 
//                 } 
                
//                 fputs(__dbg_convert(i, 10), stream);
//                 break;} 
//         }

//         format++;
//     }
// }

// static inline void _debug(FILE* stream, const char* format, va_list valist) {
//     while(*format != '\0') {
//         if(*format != '%') {
//             // putchar(*format);
//             fputc(*format, stream);
//             format++;
//             continue;
//         }

//         format++;

//         switch(*format) {
//             case 's': fputs(va_arg(valist, char*), stream); break;
//             case 'c': fputc(va_arg(valist, int), stream); break; // putchar(va_arg(valist, int)); break;
//             case 'd' : {
//                 int i = va_arg(valist, int);
//                 if(i < 0) { 
//                     i = -i;
//                     putchar('-'); 
//                 } 
                
//                 fputs(__dbg_convert(i, 10), stream);
//                 break;} 
//         }

//         format++;
//     }
// }

static inline void debug(const char* format, ...) {
    va_list valist;

    va_start(valist, format);
    // _debug(stdout, format, valist);
    vprintf(format, valist);
    va_end(valist);
}

// static inline void debug_file(FILE* file, const char* format, ...) {
//     if (file == NULL) return;

//     va_list valist;

//     va_start(valist, format);
//     // _debug(file, format, valist);
//     vfprintf(file, format, valist);
//     va_end(valist);

//     fflush(file);
// }

static inline void debug_file(FILE* file, int level, const char* format, ...) {
    if (file == NULL) return;
    if (level > DEBUG_LEVEL) return;

    va_list valist;

    va_start(valist, format);
    // _debug(file, format, valist);
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