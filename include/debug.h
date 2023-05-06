/***************************************************************
 * DEBUG UTILITY                                               *
 *                                                             *
 * This utility handles debug processes for this project.      *
 ***************************************************************/


#ifndef __RL_DEBUG_H
#define __RL_DEBUG_H

/*
 * Similar behavior to `printf`.
 */
static inline void debug(const char* format, ...);

// Implementation
#ifdef RL_DEBUG

#include <stdio.h>
#include <stdarg.h>

static inline char* __dbg_convert(unsigned int num, int base) { 
    static char rep[] = "0123456789ABCDEF";
    static char buf[50]; 

    char *ptr; 
    ptr = &buf[49]; 
    *ptr = '\0'; 

    do  { 
        *(--ptr) = rep[num % base];
        num /= base; 
    } while(num != 0); 

    return(ptr); 
}

static inline void debug(const char* format, ...) {
    va_list valist;

    va_start(valist, format);

    while(*format != '\0') {
        if(*format != '%') {
            putchar(*format);
            format++;
            continue;
        }

        format++;

        switch(*format) {
            case 's': fputs(va_arg(valist, char*), stdout); break;
            case 'c': putchar(va_arg(valist, int)); break;
            case 'd' : {
                auto i = va_arg(valist, int);
                if(i < 0) { 
                    i = -i;
                    putchar('-'); 
                } 
                
                fputs(__dbg_convert(i, 10), stdout);
                break;} 
        }

        format++;
    }
}

#else

#define IGNORE_ARG(x) (void)(x)

static inline void debug(const char* format, ...) {
    IGNORE_ARG(format);
    __asm__("nop");
}

#endif

#endif