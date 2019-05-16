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

char* option_find(list *l, char *key){
    node *nd = l->front;
    char* v;
    while(nd){
        kvp *key_val = (kvp *)nd->val;
        if(strcmp(key, key_val->key) == 0){
            v = key_val->val;
            key_val->used = 1;
            return v;
        }
        nd = nd->next;
    }
    return 0;
}

int option_find_int(list *l, char *key, int default_val){
    
    int num_out = default_val;
    char *val = option_find(l, key);
    if(val){
        num_out = atoi(val);
    }
    return num_out;
}

char* option_find_str(list *l, char* key, char* default_str){
    char* v = option_find(l, key);
    if(v){
        return v;
    }
    return default_str;
}

void option_unused(list *l)
{
    node *n = l->front;
    while(n){
        kvp *p = (kvp *)n->val;
        if(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->key, p->val);
        }
        n = n->next;
    }
}
