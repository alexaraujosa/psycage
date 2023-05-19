#include "items.h"

#define ITEMS 9

extern int EXIT;
extern char ASSET_DIR[PATH_MAX];
extern int ASSET_DIR_LEN;
extern FILE* dbgOut;

static HashMap items;
static int items_len;
static KeyValuePair* item_entries;

HashMap load_items() {
    FILE* file;

    char items_path[PATH_MAX];
    strncpy(items_path, ASSET_DIR, ASSET_DIR_LEN);
    strcat(items_path, "/items.dat");

    file = fopen(items_path, "r");
    if (file == NULL) exit(1);

    HashMap map = hm_new(1);

    size_t read;
    char * line = NULL;
    size_t len = 0;
    int lineCount = 0;
    while ((read = getline(&line, &len, file)) != (size_t)-1) {
        lineCount++;
        if (line[read - 1] == '\n') line[read - 1] = '\0';

        if (lineCount == 1) {
            int version = -1;
            if (!sscanf(line, "version = %d", &version)) {
                debug_file(dbgOut, 0, "Invalid version.\n");
                //exit(1);
                EXIT = TRUE;
                return NULL;
            }

            if (version != ITEM_VERSION) {
                debug_file(dbgOut, 0, "Version not supported.\n");
                // exit(1);
                EXIT = TRUE;
                return NULL;
            }
        } 
        else {
            DataItemNode item = parse_item(line, read - 1);
            if (item == NULL) {
                debug_file(dbgOut, 0, "Invalid item at line %d.\n", lineCount);
                // exit(1);
                EXIT = TRUE;
                return NULL;
            }

            if (hm_has(map, item->name)) {
                debug_file(dbgOut, 0, "Unable to add item '%s' (at line %d): Item with same name already registered.\n", item->name, lineCount);
                // exit(1);
                EXIT = TRUE;
                return NULL;
            }

            hm_set(map, item->id, item);
        }
    }

    fclose(file);
    if (line) free(line);

    items = map;
    items_len = hm_len(map);
    item_entries = hm_entries(map);

    return map;
}

DataItemNode defaultItemResource() {
    DataItemNode item = (DataItemNode)malloc(sizeof(DATA_ITEM_NODE));
    item->id = 0;
    item->name = "";
    item->damage = 0;
    item->armor = 0;

    return item;
}

DataItemNode parse_item(char* raw, int len) {
    IGNORE_ARG(len);
    // debug_file(dbgOut, 0, "LINE: %d | '%s'\n", len, raw);

    DataItemNode item = defaultItemResource();
    char name[MAX_INPUT];
    char id[MAX_ITEM_ID_SIZE];
    if (!sscanf(raw, "%s %s %d %d", id, name, &item->damage, &item->armor)) return NULL;

    item->id = (char*)malloc((strlen(id)+1) * sizeof(char));
    strcpy(item->id, id);

    // debug_file(dbgOut, 0, "NAME: %s\n", name);
    item->name = (char*)malloc((strlen(name)+1) * sizeof(char));
    strcpy(item->name, name);

    return item;
}

DataItemNode get_item_by_id(char* name) {
    return (DataItemNode)hm_get(items, name);
}

DataItemNode get_item_by_name(char* name) {
    for (int i = 0; i < items_len; i++) {
        if (!strcmp(((DataItemNode)item_entries[i].val)->name, name)) return (DataItemNode)item_entries[i].val;
    }

    return NULL; 
}


DataItemNode get_random_item() {

    char *itens[] = {"Gloves", "Helmet", "Belt", "Vest", "ExpressAmmo", "SoldierGloves", "SoldierHelmet", "SoldierBelt", "SoldierVest"};

    int index = rand()%ITEMS;

    return get_item_by_name(itens[index]);
}