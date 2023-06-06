#ifndef __RL_UTIL_LIST_H
#define __RL_UTIL_LIST_H

#include "common.h"

typedef struct list_node {
    void* data;
    struct list_node* next;
} LIST_NODE, *ListNode;

typedef struct linked_list {
    ListNode head;
    ListNode tail;
    int size;
} LINKED_LIST, *LinkedList;

static inline LinkedList list_create() {
    LinkedList list = (LinkedList)malloc(sizeof(LINKED_LIST));
    if (list != NULL) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
    return list;
}

static inline void list_destroy(LinkedList list) {
    if (list == NULL) return;

    ListNode current = list->head;
    while (current != NULL) {
        ListNode next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

static inline int list_size(LinkedList list) {
    if (list == NULL) return 0;
    return list->size;
}

static inline int list_is_empty(LinkedList list) {
    if (list == NULL) return 1;
    return (list->size == 0);
}

static inline ListNode list_add(LinkedList list, void* data) {
    if (list == NULL || data == NULL) return 0;

    ListNode new_node = (ListNode)malloc(sizeof(LIST_NODE));
    if (new_node == NULL) return NULL;

    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return new_node;
}

static inline void* list_get(LinkedList list, int index) {
    if (list == NULL || index < 0 || index >= list->size) return NULL;

    ListNode current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

static inline int list_set(LinkedList list, int index, void* data) {
    if (list == NULL || index < 0 || index >= list->size || data == NULL) return 1;

    ListNode current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    current->data = data;
    return 0;
}

static inline int list_remove(LinkedList list, int index) {
    if (list == NULL || index < 0 || index >= list->size) return 1;

    ListNode current = list->head;
    ListNode previous = NULL;

    for (int i = 0; i < index; i++) {
        previous = current;
        current = current->next;
    }

    if (current == list->head) {
        list->head = current->next;
    } else if (current == list->tail) {
        list->tail = previous;
        list->tail->next = NULL;
    } else {
        previous->next = current->next;
    }

    free(current);
    list->size--;
    return 0;
}

static inline int list_index(LinkedList list, void* value) {
    if (list == NULL) return -1;

    ListNode current = list->head;
    int index = 0;

    while (current != NULL) {
        if (current->data == value) {
            return index;
        }

        current = current->next;
        index++;
    }

    return -1;
}

static inline int list_has(LinkedList list, void* value) {
    if (list == NULL) return 0;

    ListNode current = list->head;

    while (current != NULL) {
        if (current->data == value) {
            return 1;
        }

        current = current->next;
    }

    return 0;
}


#endif