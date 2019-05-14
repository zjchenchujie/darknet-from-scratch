#include "connected_layer.h"

#include <stdlib.h>
#include <string.h>

connected_layer make_connected_layer(int inputs, int outputs, ACTIVATOR_TYPE activator)
{
    int i;
    connected_layer layer;
    layer.inputs = inputs;
    layer.outputs = outputs;

    layer.output = calloc(outputs, sizeof(double*));

    layer.weight_updates = calloc(inputs*outputs, sizeof(double));
    layer.weights = calloc(inputs*outputs, sizeof(double));
    for(i = 0; i < inputs*outputs; ++i)
        layer.weights[i] = .5 - (double)rand()/RAND_MAX;

    layer.bias_updates = calloc(outputs, sizeof(double));
    layer.biases = calloc(outputs, sizeof(double));
    for(i = 0; i < outputs; ++i)
        layer.biases[i] = (double)rand()/RAND_MAX;

    if(activator == SIGMOID){
        layer.activation = sigmoid_activation;
        layer.gradient = sigmoid_gradient;
    }else if(activator == RELU){
        layer.activation = relu_activation;
        layer.gradient = relu_gradient;
    }else if(activator == IDENTITY){
        layer.activation = identity_activation;
        layer.gradient = identity_gradient;
    }
    
    return layer;
}

void run_connected_layer(double *input, connected_layer layer)
{
    int i, j;
    for(i = 0; i < layer.outputs; ++i){
        layer.output[i] = layer.biases[i];
        for(j = 0; j < layer.inputs; ++j){
            layer.output[i] += input[j]*layer.weights[i*layer.inputs + j]; // full connected
        }
        layer.output[i] = layer.activation(layer.output[i]); //activation
    }
}

void backpropagate_connected_layer(double *input, connected_layer layer)
{
    int i, j;
    double *old_input = calloc(layer.inputs, sizeof(double));
    memcpy(old_input, input, layer.inputs*sizeof(double));
    memset(input, 0, layer.inputs*sizeof(double));

    for(i = 0; i < layer.outputs; ++i){
        for(j = 0; j < layer.inputs; ++j){
            input[j] += layer.output[i]*layer.weights[i*layer.outputs + j];
        }
    }
    for(j = 0; j < layer.inputs; ++j){
        input[j] = input[j]*gradient(old_input[j]);
    }
    free(old_input);
}

void calculate_updates_connected_layer(double *input, connected_layer layer)
{
    int i, j;
    for(i = 0; i < layer.outputs; ++i){
        layer.bias_updates[i] += layer.output[i];
        for(j = 0; j < layer.inputs; ++j){
            layer.weight_updates[i*layer.outputs + j] += layer.output[i]*input[j];
        }
    }
}

void update_connected_layer(connected_layer layer, double step)
{
    int i,j;
    for(i = 0; i < layer.outputs; ++i){
        layer.biases[i] += step*layer.bias_updates[i];
        for(j = 0; j < layer.inputs; ++j){
            int index = i*layer.outputs+j;
            layer.weights[index] = layer.weight_updates[index];
        }
    }
    memset(layer.bias_updates, 0, layer.outputs*sizeof(double));
    memset(layer.weight_updates, 0, layer.outputs*layer.inputs*sizeof(double));
}

