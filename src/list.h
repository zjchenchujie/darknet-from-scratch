#ifndef __LIST_H__
#define __LIST_H__

typedef struct node{
    void* val;
    struct node *prev;
    struct node *next;
}node;

typedef struct list{
    node* front;
    node* back;
    int size;
}list;

list* make_list();
// int list_find(list *l, void *val);
void* list_pop(list* l);
void list_insert(list *, void *);

void **list_to_array(list *l);

void free_list(list *l);
void free_list_contents(list *l);


#endif
