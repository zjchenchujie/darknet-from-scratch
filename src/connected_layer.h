#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#include "activations.h"

typedef struct{
    int inputs;
    int outputs;
    double *weights;
    double *biases;
    double *weight_updates;
    double *bias_updates;
    double *output;

    double (* activation)();
    double (* gradient)();
} connected_layer;

// init layer
connected_layer* make_connected_layer(int inputs, int outputs, ACTIVATION activator);

// forward
void run_connected_layer(double *input, connected_layer layer);

// compute updates of weight and bias
void learn_connected_layer(double *input, connected_layer layer);

// weight and bias update
void update_connected_layer(connected_layer layer, double step);

// compute gradient
void backpropagate_connected_layer(double *input, connected_layer layer);

// compute delta
void calculate_update_connected_layer(double *input, connected_layer layer);

#endif