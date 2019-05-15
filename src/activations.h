#ifndef __ACTIVATION_H__
#define __ACTIVATION_H__

typedef enum{
    SIGMOID, 
    RELU, 
    IDENTITY
}ACTIVATION;


ACTIVATION get_activation(char *s);
double relu_activation(double x);
double relu_gradient(double x);
double sigmoid_activation(double x);
double sigmoid_gradient(double x);
double identity_activation(double x);
double identity_gradient(double x);

#endif