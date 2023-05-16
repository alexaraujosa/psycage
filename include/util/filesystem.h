#ifndef __RL_UTIL_FS_H
#define __RL_UTIL_FS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

static inline int createParentFolder(const char* filePath) {
    char* pathCopy = strdup(filePath);
    if (pathCopy == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return -1;
    }

    char* lastSlash = strrchr(pathCopy, '/');
    if (lastSlash == NULL) {
        fprintf(stderr, "Invalid file path: %s\n", filePath);
        free(pathCopy);
        return -1;
    }

    *lastSlash = '\0';

    int result = mkdir(pathCopy, S_IRWXU | S_IRWXG | S_IRWXO);
    if (result == -1) {
        if (errno != EEXIST) {
            fprintf(stderr, "Failed to create parent directory: %s\n", pathCopy);
            free(pathCopy);
            return -1;
        }
    }

    free(pathCopy);
    return 0;
}

#endif