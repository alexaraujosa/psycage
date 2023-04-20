#ifndef __RL_UTIL_STRING_H
#define __RL_UTIL_STRING_H

inline int strlen_cp(char* str) {
    int c = 0;
    while (*str != '\0') {
        c++;
        str = str + 1;
    }

    return c;
}

#endif