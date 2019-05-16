#include "option_list.h"
#include "utils.h"

void option_insert(list* l, char* key, char* val){
    kvp *key_val = calloc(1, sizeof(kvp));
    if(key_val == NULL){
        MEM_ERR("Memory allocate failed.\n");
        exit(0);
    }
    key_val->key = key;
    key_val->val = val;
    key_val->used = 0;
    list_insert(l, key_val);
}
