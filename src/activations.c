#include "activations.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

ACTIVATION get_activation(char* s){
    if(strcmp(s, "sigmoid")==0){
        return SIGMOID;
    }else if(strcmp(s, "relu")==0){
        return RELU;
    }else if(strcmp(s, "identity")==0){
        return IDENTITY;
    }else{
        fprintf(stderr, "Could not find activation %s, going with RELU.\n", s);
        return RELU;
    }
    
}

double identity_activation(double x)
{
    return x;
}
double identity_gradient(double x)
{
    return 1;
}

double relu_activation(double x)
{
    return x*(x>0);
}
double relu_gradient(double x)
{
    return (x>=0);
}

double sigmoid_activation(double x)
{
    return 1./(1.+exp(-x));
}

double sigmoid_gradient(double x)
{
    return x*(1.-x);
}

