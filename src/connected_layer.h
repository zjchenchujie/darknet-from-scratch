#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#include "activations.h"

typedef struct{
    int inputs;
    int outputs;

    double *weights;
    double *biases;

    double *weight_momentum;
    double *bias_momentum;

    double *weight_updates;
    double *bias_updates;

    double *output;
    double *delta; 

    double (* activation)();
    double (* gradient)();
} connected_layer;

// init layer
connected_layer* make_connected_layer(int inputs, int outputs, ACTIVATION activator);

// forward
void forward_connected_layer(connected_layer layer, double *input);

// compute updates of weight and bias
void learn_connected_layer(connected_layer layer, double *input);

// weight and bias update
void update_connected_layer(connected_layer layer, double step, double momentum, double decay);

// compute gradient
void backward_connected_layer(connected_layer layer, double *input, double *delta);

// compute delta
void calculate_update_connected_layer(double *input, connected_layer layer);

#endif