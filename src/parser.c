#include "parser.h"
#include "list.h"

#include <stdio.h>

typedef struct {
    char *type;
    list *options;
}section;

list *read_cfg(char *filename);

network parse_network_cfg(char* filename){
    list *sections = read_cfg(filename);
    // FILE *fd = fopen(filename);

}

int read_option(char* line, list* options){
    int i;
    int len = strlen(line);
    char* val = 0;
    for(i=0; i<len; ++i){
        if(line[i] == '='){
            line[i] = '\0';
            // char* key = line;
            val = line + i + 1; // or &line[i+1]
            break;
        }
    }
    if(i == len-1){
        return 0;
    }
    char* key = line;

    option_insert(options, key, val);
}

list *read_cfg(char *filename){
    FILE *file = fopen(filename, 'r');
    if(file == NULL)
    {
        FILE_ERR("Open config file failed.\n");
    }
    char* line;
    int nu = 0;

    list *sections = make_list();
    section *current = NULL;
    while((line = fgetl(file)) != 0){
        switch(line[0]){
            case '[':
                ++nu;
                strip(line);
                current = malloc(sizeof(section));
                if(current == NULL){
                    MEM_ERR("Memory allocate failed \n");
                    return 0;
                }
                current->type = line;
                current->options = make_list();
                list_insert(sections, current);
                // free(line);
                break;
            case '#':
            case ';':
            case '\0':
                free(line);
                break;
            default:
                ++nu;
                strip(line);
                if(!read_option(line, current->options)){
                    FILE_ERR("Config file error line: %d, can not parse: %s\n", nu, line);
                    free(line);
                }
                break;
        }
    }


}