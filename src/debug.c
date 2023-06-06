#include "common.h"
#include "debug.h"
#include "util/filesystem.h"

#ifdef RL_DEBUG

FILE* make_debug_file(char* bin, int bin_len, char* filename) {
    char dbgfile_path[PATH_MAX];
    snprintf(dbgfile_path, sizeof(dbgfile_path), "%s/logs/%s-", bin, filename);

	time_t now = time(NULL);
    struct tm* tmInfo = localtime(&now);
    size_t dateFormatSize = 20;
    size_t new_dbgout_len = bin_len + sizeof("/logs/dbg-") - 1 + dateFormatSize;

    char* new_dbgfile_path = malloc(new_dbgout_len);
    if (new_dbgfile_path == NULL) {
        fprintf(stderr, "Unable to initialize game: Failed to generate log path.\n");
        return 1;
    }
    strcpy(new_dbgfile_path, dbgfile_path);
    strftime(new_dbgfile_path + new_dbgout_len - dateFormatSize, dateFormatSize, "%Y-%m-%dT%H:%M:%S", tmInfo);
    strcat(new_dbgfile_path, ".log");

	if (createParentFolder(new_dbgfile_path) != 0) {
        printf("Unable to initialize game: Unable to create log folder.\n");
        exit(1);
    }

    FILE* dbgFileOut = fopen(new_dbgfile_path, "a");
    if (dbgFileOut == NULL) return NULL;

    return dbgFileOut;
}

#else

FILE* make_debug_file(char* bin, int bin_len, char* filename) {
    IGNORE_ARG(bin);
    IGNORE_ARG(bin_len);
    IGNORE_ARG(filename);
    return NULL;
}

#endif