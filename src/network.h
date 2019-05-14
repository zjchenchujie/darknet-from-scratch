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

void run_network(image input, network net);
image get_network_image(network net);

#endif

