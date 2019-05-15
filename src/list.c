#include "list.h"

list *make_list(){
    list *l = calloc(1, sizeof(list));
    l->back = l->front = 0;
    l->size = 0;
    return l;
}

void transfer_node(list *s, list *d, node *n)
{
    node *prev, *next;
    prev = n->prev;
    next = n->next;
    if(prev) prev->next = next;
    if(next) next->prev = prev;
    --s->size;
    if(s->front == n) s->front = next;
    if(s->back == n) s->back = prev;
}

void *list_pop(list* l){
    if(l->size == 0){
        return 0;
    }

    node *tmp = l->back;
    void *val = tmp->val;
    l->back = tmp->prev;
    if(l->back){
        l->back->next = 0;
    }
    --l->size;
    free(tmp);
    return val;
}

void list_insert(list* l, void *val){
    node *new = malloc(sizeof(node));
    new->val = val;
    new->next = 0;
    if(l->front == 0){
        new->prev = 0;
        l->front = new;
    }
    else{
        l->back->next = new;
        new->prev = l->back;
        
    }
    l->back = new;
    ++l->size;
}

void free_node(node* n){
    node *next;
    while(n){
        next = n->next;
        free(n);
        n = next;
    }
}

void free_list(list *l){
    free_node(l->front);
    free(l);
}

void free_list_content(list *l){
    node* next = l->front;
    while(next){
        free(next->val);
        next = next->next;
    }
}

void **list_to_array(list *l)
{
    void **a = calloc(l->size, sizeof(void*));
    int count = 0;
    node *n = l->front;
    while(n){
        a[count++] = n->val;
        n = n->next;
    }
    return a;
}