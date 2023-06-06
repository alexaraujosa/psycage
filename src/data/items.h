/***************************************************************
 * ITEMS MODULE                                                *
 *                                                             *
 * This module handles loading, fetching and manipulating      *
 * items.                                                      *
 *                                                             *
 * Example of usage:                                           *
 *                                                             *
 * HashMap map = load_items();                                 *
 *                                                             *
 * DataItemNode item1 = get_item_by_id("0000");                *
 * if (item1 == NULL) debug("Item 1 not found.\n");            *
 *                                                             *
 * DataItemNode item2 = get_item_by_name("Test");              *
 * if (item2 == NULL) debug("Item 2 not found.\n");            *
 ***************************************************************/

#ifndef __RL_DATA_ITEMS_H
#define __RL_DATA_ITEMS_H

#include <stdio.h>
#include "common.h"
#include "util/string.h"
#include "util/hashmap.h"
#include "util/math.h"
#include "debug.h"

#define MAX_ITEM_ID_SIZE 19
#define ITEM_VERSION 1

typedef struct ditem_node {
    char* id;
    char* name;
    int damage;
    int armor;
} DATA_ITEM_NODE, *DataItemNode;

HashMap load_items();
DataItemNode parse_item(char* raw, int len);
DataItemNode get_item_by_id(char* name);
DataItemNode get_item_by_name(char* name);
DataItemNode get_random_item();

#endif