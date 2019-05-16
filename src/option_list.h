#ifndef __OPTION_LIST__H__
#define __OPTION_LIST__H__

#include "list.h"

typedef struct {
    char* key;
    char* val;
    int used;
}kvp;

void option_insert(list* l, char* key, char* val);


#endif