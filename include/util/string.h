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

typedef struct char_node {
    char* value;
    struct char_node* next;
} CharNode;


inline CharNode* char_node(char* value) {
    CharNode* n = calloc(1, sizeof(struct char_node));
    n->value = value;
    n->next = NULL;
    return n;
}

#endif