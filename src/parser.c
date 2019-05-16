#include "parser.h"
#include "list.h"
#include "network.h"
#include "activations.h"
#include "convolutional_layer.h"
#include "maxpool_layer.h"
#include "connected_layer.h"
#include "option_list.h"

#include <stdio.h>

typedef struct {
    char *type;
    list *options;
}section;

list *read_cfg(char *filename);
LAYER_TYPE get_layer_type(char* type);

network parse_network_cfg(char* filename){
    list *sections = read_cfg(filename);
    network net = make_network(sections->size);
    int layer_n = 0;
    node *n = sections->front;

    section *layer_section = (section *)n->val;
    list *options = layer_section->options;
    LAYER_TYPE layer_type = get_layer_type(layer_section->type);

    if(layer_type == CONVOLUTIONAL){
        int h;
        int w;
        int c;
        int num_out = option_find_int(options, "filters", 1);
        int kernel_size = option_find_int(options, "size", 1);
        int stride = option_find_int(options, "stride", 1);
        char* activation = option_find_str(options, "activation", "relu");
        ACTIVATION activation_type = get_activation(activation);
        if(layer_n == 0){
            h = option_find_int(options, "height", 300);
            w = option_find_int(options, "width", 300);
            c = option_find_int(options, "channels", 3);
        }
        else{
            image m = get_network_image_layer(net, layer_n-1);
            h = m.h;
            w = m.w;
            c = m.c;
        }
        convolutional_layer *layer = make_convolutional_layer(w, h, c, num_out, kernel_size, stride);
        net.types[layer_n] = CONVOLUTIONAL;
        net.layers[layer_n] = layer;
        option_unused(options);
    }
    else if(layer_type == CONNECTED){
        int num_in;
        int num_out = option_find_int(options, "output", 1);
        char* activation = option_find_str(options, "actiation", "relu");
        ACTIVATION activation_type = get_activation(activation);
        if(layer_n == 0){
            num_in = option_find_int(options, "input", 1);
        }
        else{
            num_in = get_network_output_layer(net, layer_n-1);
        }
        connected_layer *layer = make_connected_layer(num_in, num_out, activation_type);
        net.types[layer_n] = CONNECTED;
        net.layers[layer_n] = layer;
        option_unused(options);
    }
    else if(layer_type == MAXPOOL){
        int h, w, c;
        // int kernel_size;
        int stride;
        // kernel_size = option_find_int(options, "size", 1);
        stride = option_find_int(options, "stride", 1);
        if(layer_n == 0){
            STD_ERR("Pooling layer is not expeced to be first layer. \n");
        }else{
            image m = get_network_image_layer(net, layer_n-1);
            h = m.h;
            w = m.w;
            c = m.c;
        }
        maxpool_layer *layer = make_maxpool_layer(h, w, c, stride);
        net.types[layer_n] = MAXPOOL;
        net.layers[layer_n] = layer;
        option_unused(options);
    }
    else{
        STD_ERR("CUSTOM layer is not supported yet. \n");
        exit(0);
    }

    ++layer_n;
    n = n->next;

    return net;

}

LAYER_TYPE get_layer_type(char* type){
    if(strcmp(type, "convolution") == 0 || strcmp(type, "conv") == 0){
        return CONVOLUTIONAL;
    }
    else if(strcmp(type, "maxpool") == 0 || strcmp(type, "pool")){
        return MAXPOOL;
    }
    else if(strcmp(type, "FC") == 0 || strmp(type, "connected")== 0){
        return CONNECTED;
    }
    else{
        return CUSTOM;
    }
    

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
                // ++nu;
                strip(line);
                if(!read_option(line, current->options)){
                    FILE_ERR("Config file error line: %d, can not parse: %s\n", nu, line);
                    free(line);
                }
                break;
        }
    }
    fclose(filename);
    return sections;
}