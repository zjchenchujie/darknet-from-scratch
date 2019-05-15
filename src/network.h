// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H

#include "image.h"

typedef enum {
    CONVOLUTIONAL,
    CONNECTED,
    MAXPOOL
} LAYER_TYPE;

typedef struct {
    int n; // 网络层数, 不包括activation, 因为activation已经包含在卷积层中
    void **layers;
    LAYER_TYPE *types;
} network;

network make_network(int n_layer);
void run_network(image input, network net);
void learn_network(image input, network net);
void update_network(network net, double step);
double* get_network_output(network net);
double* get_network_output_layer(network net, int i);

image get_network_image(network net);
image get_network_image_layer(network net, int i);

#endif

