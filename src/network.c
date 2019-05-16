#include "network.h"
#include "image.h"

#include "connected_layer.h"
#include "convolutional_layer.h"
#include "maxpool_layer.h"
network make_network(int n){
    network net;
    net.n = n;
    net.layers = calloc(n, sizeof(void*));
    net.types = calloc(n, sizeof(LAYER_TYPE));
}
void run_network(image input, network net)
{
    int i;
    double *input_d = input.data;
    for(i = 0; i < net.n; ++i){
        if(net.types[i] == CONVOLUTIONAL){
            convolutional_layer layer = *(convolutional_layer *)net.layers[i];
            run_convolutional_layer(input, layer);
            input = layer.output;
            input_d = layer.output.data;
        }
        else if(net.types[i] == CONNECTED){
            connected_layer layer = *(connected_layer *)net.layers[i];
            run_connected_layer(input_d, layer);
            input_d = layer.output;
        }
        else if(net.types[i] == MAXPOOL){
            maxpool_layer layer = *(maxpool_layer *)net.layers[i];
            run_maxpool_layer(input, layer);
            input = layer.output;
            input_d = layer.output.data;
        }
    }
}

void update_network(network net, double step)
{
    int i;
    for(i = 0; i < net.n; ++i){
        if(net.types[i] == CONVOLUTIONAL){
            convolutional_layer layer = *(convolutional_layer *)net.layers[i];
            update_convolutional_layer(layer, step);
        }
        else if(net.types[i] == MAXPOOL){
            //maxpool_layer layer = *(maxpool_layer *)net.layers[i];
        }
        else if(net.types[i] == CONNECTED){
            connected_layer layer = *(connected_layer *)net.layers[i];
            update_connected_layer(layer, step);
        }
    }
}

void learn_network(image input, network net)
{
    int i;
    image prev;
    double *prev_p;
    for(i = net.n-1; i >= 0; --i){
        if(i == 0){
            prev = input;
            prev_p = prev.data;
        } else if(net.types[i-1] == CONVOLUTIONAL){
            convolutional_layer layer = *(convolutional_layer *)net.layers[i-1];
            prev = layer.output;
            prev_p = prev.data;
        } else if(net.types[i-1] == MAXPOOL){
            maxpool_layer layer = *(maxpool_layer *)net.layers[i-1];
            prev = layer.output;
            prev_p = prev.data;
        } else if(net.types[i-1] == CONNECTED){
            connected_layer layer = *(connected_layer *)net.layers[i-1];
            prev_p = layer.output;
        }

        if(net.types[i] == CONVOLUTIONAL){
            convolutional_layer layer = *(convolutional_layer *)net.layers[i];
            learn_convolutional_layer(prev, layer);
        }
        else if(net.types[i] == MAXPOOL){
            //maxpool_layer layer = *(maxpool_layer *)net.layers[i];
        }
        else if(net.types[i] == CONNECTED){
            connected_layer layer = *(connected_layer *)net.layers[i];
            learn_connected_layer(prev_p, layer);
        }
    }
}

// 第i层网络输出
double *get_network_output_layer(network net, int i){
    // int layer_n = i;
    if(net.types[i]==CONVOLUTIONAL){
        convolutional_layer layer = *((convolutional_layer *)net.layers[i]);
        return layer.output.data;
    }
    else if(net.types[i]==MAXPOOL){
        maxpool_layer layer = *((maxpool_layer *)net.layers[i]);
        return layer.output.data;
    }
    else if(net.layers[i] == CONNECTED){
        connected_layer layer = *((connected_layer *)net.layers[i]);
        return layer.output;
    }
    return 0;
} 

// 网络最终输出
double *get_network_output(network net)
{
    int i = net.n-1;
    return get_network_output_layer(net, i);
}

// i 层网络输出大小
int get_network_output_size_layer(network net, int i)
{
    if(net.types[i] == CONVOLUTIONAL){
        convolutional_layer layer = *(convolutional_layer *)net.layers[i];
        return layer.output.h*layer.output.w*layer.output.c;
    }
    else if(net.types[i] == MAXPOOL){
        maxpool_layer layer = *(maxpool_layer *)net.layers[i];
        return layer.output.h*layer.output.w*layer.output.c;
    }
    else if(net.types[i] == CONNECTED){
        connected_layer layer = *(connected_layer *)net.layers[i];
        return layer.outputs;
    }
    return 0;
}

image get_network_image_layer(network net, int i){
    if(net.types[i]==CONVOLUTIONAL){
        convolutional_layer* layer = (convolutional_layer*)net.layers[i];
        return layer->output;
    }
    else if(net.types[i]==MAXPOOL){
        maxpool_layer* layer = (maxpool_layer*)net.layers[i];
        return layer->output;
    }
    return make_image(0, 0, 0);
}

image get_network_image(network net)
{
    int i = net.n -1;
    return get_network_image_layer(net, i);
}

