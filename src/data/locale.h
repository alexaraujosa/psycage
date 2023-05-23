/***************************************************************
 * LOCALE MODULE                                               *
 *                                                             *
 * This module handles loading, implementing and switching     *
 * localization files.                                         *
 *                                                             *
 * Example of usage:                                           *
 *                                                             *
 * load_locales();                                             *
 *                                                             *
 * char* key1 = "abc.efg";                                     *
 * char* transl1 = get_localized_string(EN_US, key1);          *
 * debug("Key: %s | Translation: %s\n", key1, transl1);        *
 * // Prints "abc.efg"                                         *
 *                                                             *
 * char* key2 = "menu.main.new_game";                          *
 * char* transl2 = get_localized_string(EN_US, key2);          *
 * debug("Key: %s | Translation: %s\n", key2, transl2);        *
 * // Prints "New Game"                                        *
 *                                                             *
 * char* transl2_1 = get_localized_string(PT_PT, key2);        *
 * debug("Key: %s | Translation: %s\n", key2, transl2_1);      *
 * // Prints "Novo Jogo"                                       *
 ***************************************************************/

#ifndef __RL_DATA_LOCALE_H
#define __RL_DATA_LOCALE_H

#include <stdio.h>
#include <stdarg.h>
#include "common.h"
#include "util/string.h"
#include "util/hashmap.h"
#include "debug.h"

#define MAX_ITEM_ID_SIZE 10
#define ITEM_VERSION 1

typedef enum {
    EN_US,
    PT_PT
} DataLocale;

typedef struct dlocale_node {
    DataLocale id;
    HashMap translations;
} DATA_LOCALE_NODE, *DataLocaleNode;

typedef struct dlocale_line {
    char* key;
    char* translation;
} DATA_LOCALE_LINE, *DataLocaleLine;

void load_locales();
DataLocaleLine parse_locale_line(char* raw, int len);

DataLocale get_locale_id(char* key);
char* get_localized_string(DataLocale loc, char* key);
char* get_args_localized_string(DataLocale loc, char* key, ...);
void change_locale();

#endif