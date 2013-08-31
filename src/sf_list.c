#include <string.h>

#include "sf_list.h"


static inline void insert_node(struct sf_list_node *prev,
                               struct sf_list_node *node,
                               struct sf_list_node *next) {
    prev->next = node;
    node->prev = prev;

    node->next = next;
    next->prev = node;
}

static inline void remove_node(struct sf_list_node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}


struct sf_list *sf_list_create(size_t size) {
    struct sf_list *l;

    l = malloc(sizeof(*l) + sizeof(*(l->head)));

    l->size = size;
    l->nelt = 0;
    l->head->prev = l->head;
    l->head->next = l->head;

    return l;
}

void sf_list_destroy(struct sf_list *l, sf_list_destructor_t *destructor) {
    struct sf_list_node *node;

    while (l->nelt) {
        node = l->head->next;
        if (destructor) {
            destructor(node->elt);
        }
        remove_node(node);
        free(node);
        --l->nelt;
    }
    free(l);
}

void sf_list_push(struct sf_list * l, void *elt) {
    struct sf_list_node *node;

    if (elt == NULL) {
        return;
    }

    node = malloc(sizeof(*node) + l->size);
    if (node == NULL) {
        return;
    }
    memcpy(node->elt, elt, l->size);

    insert_node(l->head->prev, node, l->head);

    ++l->nelt;
}

void sf_list_push_front(struct sf_list * l, void *elt) {
    struct sf_list_node *node;

    if (elt == NULL) {
        return;
    }

    node = malloc(sizeof(*node) + l->size);
    if (node == NULL) {
        return;
    }
    memcpy(node->elt, elt, l->size);

    insert_node(l->head, node, l->head->next);

    ++l->nelt;
}

void sf_list_pop(struct sf_list * l, void *o_elt) {
    struct sf_list_node *node = l->head->prev;

    remove_node(node);
    --l->nelt;

    if (o_elt) {
        memcpy(o_elt, node->elt, l->size);
    }
    free(node);
}

void sf_list_pop_front(struct sf_list * l, void *o_elt) {
    struct sf_list_node *node = l->head->next;

    remove_node(node);
    --l->nelt;

    if (o_elt) {
        memcpy(o_elt, node->elt, l->size);
    }
    free(node);
}
