#include "locale.h"
#include "../render/render.h"

// External Constants
extern char ASSET_DIR[PATH_MAX];
extern int ASSET_DIR_LEN;

typedef struct dlocale_id {
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
            debug("Missing language file: %s", locale_path);
            exit(1);
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
                    debug("Invalid version.\n");
                    exit(1);
                }

                if (version != LOCALE_VERSION) {
                    debug("Version not supported.\n");
                    exit(1);
                }
            } else {
                DataLocaleLine localeLine = parse_locale_line(line, read - 1);
                if (localeLine == NULL) {
                    debug("Invalid translation at line %d.\n", lineCount);
                    exit(1);
                }

                if (hm_has(locale->translations, localeLine->key)) {
                    debug("Unable to add translation '%s' (at line %d): Translation with same key already registered.\n", localeLine->key, lineCount);
                    exit(1);
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
    DataLocaleLine line = defaultLocaleLine();

    char key[MAX_LOCALE_KEY_LEN];
    char translation[MAX_LOCALE_TRANSLATION_LEN];

    int part = 0;
    int count = 0;
    for (int i = 0; i < strlen(raw); i++) {
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

void change_locale() {

    g_renderstate->language = (g_renderstate->language == en_US) ? pt_PT : en_US;
    
    return;
}