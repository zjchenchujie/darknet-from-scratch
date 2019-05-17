#ifndef __CONVOLUTIONAL_LAYER_H__
#define __CONVOLUTIONAL_LAYER_H__

#include "image.h"
#include "activations.h"

typedef struct {
    int h, w, c; // input img dimension
    int n;
    int stride;
    int kernel_size;

    image* kernels;
    image* kernel_updates;
    double* bias;
    double* bias_updates;

    image upsampled;
    double* delta;
    double* output;
    
    double (* activation)();
    double (* gradient)();

}convolutional_layer;

convolutional_layer* make_convolutional_layer(int w, int h, int c, int num_out, int size, int stride, ACTIVATION activator);
void forward_convolutional_layer(const convolutional_layer layer, double* in);
void backward_convolutional_layer(convolutional_layer layer, double *input, double *delta);
void learn_convolutional_layer(convolutional_layer layer, double *input);
void update_convolutional_layer(convolutional_layer layer, double step);
void visualize_convolutional_layer(convolutional_layer layer);

image get_convolutional_image(convolutional_layer layer);
// void backpropagate_convolutional_layer_convolve(image input, convolutional_layer layer);


#endif