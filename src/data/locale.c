#include "locale.h"
#include "../render/render.h"

// External Constants
extern int EXIT;
extern char ASSET_DIR[PATH_MAX];
extern int ASSET_DIR_LEN;
extern FILE* dbgOut;

struct dlocale_id {
    DataLocale id;
    char* location;
};

// Internal Constants
#define LOCALE_VERSION 1
#define LOCALES 2
#define MAX_LOCALE_KEY_LEN 255
#define MAX_LOCALE_TRANSLATION_LEN 255

struct dlocale_id locale_meta[LOCALES] = {
    { EN_US, "en_US" },
    { PT_PT, "pt_PT" }
};

DataLocaleNode locales[LOCALES];

// Default factories
DataLocaleNode defaultLocale() {
    DataLocaleNode locale = (DataLocaleNode)malloc(sizeof(DATA_LOCALE_NODE));
    locale->id = -1;
    locale->translations = hm_new(1);

    return locale;
}

DataLocaleLine defaultLocaleLine() {
    DataLocaleLine line = (DataLocaleLine)malloc(sizeof(DATA_LOCALE_LINE));
    line->key = "nil";
    line->translation = "N/A";

    return line;
}

// Loaders
void load_locales() {
    char locales_path[PATH_MAX];

    strncpy(locales_path, ASSET_DIR, ASSET_DIR_LEN);
    strcat(locales_path, "/lang/");

    for (int i = 0; i < LOCALES; i++) {
        FILE* file;

        char locale_path[PATH_MAX];
        memset(locale_path, 0, PATH_MAX * sizeof(char));
        strncpy(locale_path, locales_path, strlen(locales_path));
        strcat(locale_path, locale_meta[i].location);
        strcat(locale_path, ".lang");

        file = fopen(locale_path, "r");
        if (file == NULL) {
            debug_file(dbgOut, 0, "Cannot load locales: Missing language file: %s", locale_path);
            // exit(1);
            EXIT = TRUE;
            return;
        }

        DataLocaleNode locale = defaultLocale();
        locale->id = locale_meta[i].id;

        size_t read;
        char * line = NULL;
        size_t len = 0;
        int lineCount = 0;

        while ((read = getline(&line, &len, file)) != (size_t)-1) {
            lineCount++;
            if (line[read - 1] == '\n') line[read - 1] = '\0';

            if (lineCount == 1) {
                int version = -1;
                if (!sscanf(line, "#version=%d", &version)) {
                    debug_file(dbgOut, 0, "Cannot load locales: Invalid version on locale %s.\n", locale->id);
                    // exit(1);
                    EXIT = TRUE;
                    return;
                }

                if (version != LOCALE_VERSION) {
                    debug_file(dbgOut, 0, "Cannot load locales: Version not supported on locale %s.\n", locale->id);
                    // exit(1);
                    EXIT = TRUE;
                    return;
                }
            } else {
                DataLocaleLine localeLine = parse_locale_line(line, read - 1);
                if (localeLine == NULL) {
                    debug_file(
                        dbgOut, 
                        "Cannot load locales: Invalid translation at line %d on locale %s.\n", 
                        lineCount, 
                        locale->id
                    );
                    // exit(1);
                    EXIT = TRUE;
                    return;
                }

                if (hm_has(locale->translations, localeLine->key)) {
                    debug_file(
                        dbgOut, 
                        "Cannot load locales: Unable to add translation '%s' (at line %d): "
                        "Translation with same key already registered on locale %s.\n", 
                        localeLine->key, 
                        lineCount,
                        locale->id
                    );
                    // exit(1);
                    EXIT = TRUE;
                    return;
                }

                hm_set(locale->translations, localeLine->key, localeLine->translation);
            }
        }

        fclose(file);
        if (line) free(line);

        locales[locale_meta[i].id] = locale;
    }

    return;
}

DataLocaleLine parse_locale_line(char* raw, int len) {
    IGNORE_ARG(len);

    DataLocaleLine line = defaultLocaleLine();

    char key[MAX_LOCALE_KEY_LEN];
    char translation[MAX_LOCALE_TRANSLATION_LEN];

    int part = 0;
    int count = 0;
    for (size_t i = 0; i < strlen(raw); i++) {
        if (raw[i] == '=') {
            key[count] = '\0';

            count = 0;
            part++;
            continue;
        }

        if (part == 0) key[count++] = raw[i];
        else translation[count++] = raw[i];
    }

    translation[count] = '\0';

    line->key = (char*)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(line->key, key);

    line->translation = (char*)malloc((strlen(translation) + 1) * sizeof(char));
    strcpy(line->translation, translation);

    return line;
}

// Data accessors
char* get_localized_string(DataLocale loc, char* key) {
    if (key == NULL) return NULL;

    char* transl = hm_get(locales[loc]->translations, key);

    if (transl == NULL) return key;
    return transl;
}

char* get_args_localized_string(DataLocale loc, char* key, ...) {
    va_list valist;
    va_start(valist, key);

    char* format = get_localized_string(loc, key);

    char* transl = (char*)malloc(MAX_LOCALE_TRANSLATION_LEN * sizeof(char));

    vsnprintf(transl, MAX_LOCALE_TRANSLATION_LEN, format, valist);

    return transl;
}

char* format_localized_string(char* format, ...) {
    va_list valist;
    va_start(valist, format);

    char* transl = (char*)malloc(MAX_LOCALE_TRANSLATION_LEN * sizeof(char));

    vsnprintf(transl, MAX_LOCALE_TRANSLATION_LEN, format, valist);

    return transl;
}

void change_locale() {

    g_renderstate->language = (g_renderstate->language == en_US) ? pt_PT : en_US;
    
    return;
}