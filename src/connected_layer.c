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

void learn_connected_layer(double *input, connected_layer layer)
{
    calculate_update_connected_layer(input, layer);
    backpropagate_connected_layer(input, layer);
}

// 更新权重
void update_connected_layer(connected_layer layer, double step)
{
    int i,j;
    for(i = 0; i < layer.outputs; ++i){
        layer.biases[i] += step*layer.bias_updates[i];
        for(j = 0; j < layer.inputs; ++j){
            int index = i*layer.outputs+j;
            layer.weights[index] += step*layer.weight_updates[index];
        }
    }
    memset(layer.bias_updates, 0, layer.outputs*sizeof(double));
    memset(layer.weight_updates, 0, layer.outputs*layer.inputs*sizeof(double));
}

// 计算反向传播公式中的 loss相对于权重的偏导数
void calculate_updates_connected_layer(double *input, connected_layer layer)
{
    int i, j;
    for(i = 0; i < layer.outputs; ++i){
        layer.bias_updates[i] += layer.output[i]; // TODO. gradient
        for(j = 0; j < layer.inputs; ++j){
            layer.weight_updates[i*layer.outputs + j] += layer.output[i]*input[j];
        }
    }
}

// 由delta(l+1) 计算 delta（l), 更新前的input为delta（l+1)
void backpropagate_connected_layer(double *input, connected_layer layer)
{
    int i, j;
    
    for(j = 0; j < layer.inputs; ++i){
        double grad = layer.gradient(input[j]);
        input[j] = 0;
        for(i = 0; i < layer.outputs; ++i){
            input[j] += layer.output[i]*layer.weights[i*layer.outputs + j];
        }
        input[j] *= grad;
    }
}





