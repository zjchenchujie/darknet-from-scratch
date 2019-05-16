#ifndef __OPTION_LIST__H__
#define __OPTION_LIST__H__

#include "list.h"

typedef struct {
    char* key;
    char* val;
    int used;
}kvp;

void option_insert(list* l, char* key, char* val);
int option_find_int(list *l, char *key, int default_val);
char* option_find_str(list *l, char* key, char* default_str);
void option_unused(list *l);


#endif