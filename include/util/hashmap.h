#ifndef __RL_UTIL_HASHMAP_H
#define __RL_UTIL_HASHMAP_H

#include "common.h"
#include "util/string.h"

#define CAPACITY_MULTIPLIER 2

typedef struct hm_node {
    char *key;
    void *val;
    struct hm_node *next;
} HM_NODE, *Hm_Node;

/**
 * map_t contains an array of nodes with a length 
 * and a capacity.
 */
typedef struct {
    int len;
    int cap;
    Hm_Node* list;
} HASHMAP, *HashMap; 

typedef struct {
    char* key;
    void* val;
} KeyValuePair;

static inline HashMap hm_new(const unsigned int size);
static inline void hm_free(HashMap m);
static inline int hm_has(HashMap m, char* key);
static inline void* hm_get(HashMap m, char* key);
static inline int hm_set(HashMap m, char* key, void* val);
static inline void hm_del(HashMap m, char* key);
static inline int hm_len(HashMap m);


static inline void hm_free_list(Hm_Node n) {
    if (!n) return;

    Hm_Node tmp;
    while (n != NULL) {
        tmp = n;
        n = n->next;
        free(tmp);
    }

    free(n);
}

static inline void hm_free(HashMap m) {
    if (!m)  return;

    if (m->list) {
        hm_free_list(*m->list);
    }

    free(m);
}

static inline HashMap hm_new(const unsigned int size) {
    HashMap m = (HashMap)malloc(sizeof(HASHMAP));
    if (!m) return NULL;

    memset(m, 0, sizeof(HASHMAP));

    m->cap = size;
    m->len = 0;

    m->list = (Hm_Node*)malloc(sizeof(Hm_Node) * m->cap);
    memset(m->list, 0, sizeof(Hm_Node) * m->cap);

    for (int i = 0; i < m->cap; i++) {
        m->list[i] = NULL;
    }

    return m;
}

static inline int hm_resize(HashMap m, int new_cap) {
    HashMap nm = hm_new(new_cap);

    if (!nm)  return -1;

    for (int i = 0; i < m->len; i++) {
        Hm_Node list = m->list[i];
        Hm_Node temp = list;

        while (temp) {
            int st = hm_set(nm, temp->key, temp->val);
            if (st != 0) {
                return -1;
            }
            temp = temp->next;
        }
    }

    hm_free_list(*m->list);
    *m = *nm;
    return 0;
}

static inline unsigned int hash(HashMap m, char* key) {
    unsigned int hashval = 0;
    for (size_t i = 0; i < strlen(key); i++) {
        hashval = key[i] + (hashval << 5) - hashval;
    }

    return hashval % m->cap;
}

static inline int hm_has(HashMap m, char* key) {
    return (hm_get(m, key) != NULL);
}

static inline void* hm_get(HashMap m, char* key) {
    unsigned int pos = hash(m, key);
    Hm_Node list = m->list[pos];
    Hm_Node temp = list;

    while (temp) {
        if (strcmp(temp->key, key) == 0)  return temp->val;

        temp = temp->next;
    }

    return NULL;
}

static inline int hm_set(HashMap m, char* key, void* val) {
    if (m->len == m->cap) {
        if (hm_resize(m, m->cap*CAPACITY_MULTIPLIER) == -1) return -1;
    }

    unsigned int pos = hash(m, key);              
    Hm_Node list = m->list[pos]; 
    Hm_Node temp = list;    

    while (temp) {                 
        if (strcmp(temp->key, key) == 0) {
            temp->val = val;
            return 0;
        }

        temp = temp->next;
    }

    Hm_Node new_node = (Hm_Node)malloc(sizeof(HM_NODE)); 
    if (!new_node)  return -1;

    memset(new_node, 0, sizeof(HM_NODE));

    new_node->key = strdup(key);
    new_node->val = val;
    new_node->next = list;

    m->list[pos] = new_node;
    m->len++;

    return 0;
}

static inline void hm_del(HashMap m, char* key) {
    unsigned int pos = hash(m, key);
    Hm_Node* n = &m->list[pos];

    while (*n) {
        Hm_Node temp = *n; 

        if (strcmp(temp->key, key) == 0) {
            *n = temp->next;
            break;
        } else {
            temp = (*n)->next;
        }
    }

    m->len--;
}

static inline int hm_len(HashMap m) {
    return m->len;
}

static inline KeyValuePair* hm_entries(HashMap m) {
    KeyValuePair* entries = malloc(sizeof(KeyValuePair) * m->len);
    int i = 0;

    for (int j = 0; j < m->cap; j++) {
        Hm_Node list = m->list[j];
        Hm_Node temp = list;

        while (temp) {
            entries[i].key = temp->key;
            entries[i].val = temp->val;
            i++;
            temp = temp->next;
        }
    }

    return entries;
}


#endif