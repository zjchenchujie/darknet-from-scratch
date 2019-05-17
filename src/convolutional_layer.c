#include "convolutional_layer.h"
#include "activations.h"
#include "common.h"

// get convolutional layer output image
image get_convolutional_image(convolutional_layer layer){
    image out;
    int h = (layer.h-layer.kernel_size)/layer.stride + 1;
    int w = (layer.w-layer.kernel_size)/layer.stride + 1;
    int c = layer.n;
    out = double_to_image(h, w, c, layer.output);
    return out;
}

// 由层参数得到本层输出端的delta
image get_convolutional_delta(convolutional_layer layer){
    int h = (layer.h - layer.kernel_size)/layer.stride + 1;
    int w = (layer.w - layer.kernel_size)/layer.stride + 1;
    int c = layer.n;
    return double_to_image(h, w, c, layer.delta);
}

convolutional_layer* make_convolutional_layer(int h, int w, int c, int num_out, int size, int stride, ACTIVATION activator)
{
    int i;
    convolutional_layer* layer = calloc(1, sizeof(convolutional_layer));
    layer->h = h;
    layer->w = w;
    layer->c = c;
    layer->n = num_out;
    layer->stride = stride;
    layer->kernel_size = size;

    layer->kernels = calloc(layer->n, sizeof(image));
    layer->kernel_updates = calloc(layer->n, sizeof(image));
    layer->bias = calloc(layer->n, sizeof(double));
    layer->bias_updates = calloc(layer->n, sizeof(double));

    //init weight and bias
    for(int i=0; i<layer->n; ++i){
        layer->bias[i] = 0;
        layer->kernels[i] = make_random_kernel(size, c);
        layer->kernel_updates[i] = make_random_kernel(size, c);
    }

    layer->delta = calloc(((h-size)/stride+1) * ((w-size)/stride + 1) * num_out, sizeof(double));
    layer->output = calloc(((h-size)/stride+1) * ((w-size)/stride + 1) * num_out, sizeof(double));

    layer->upsampled = make_image(h, w, c);

    if(activator == SIGMOID){
        layer->activation = sigmoid_activation;
        layer->gradient = sigmoid_gradient;
    }else if(activator == RELU){
        layer->activation = relu_activation;
        layer->gradient = relu_gradient;
    }else if(activator == IDENTITY){
        layer->activation = identity_activation;
        layer->gradient = identity_gradient;
    }
    return layer;
}

void forward_convolutional_layer(const convolutional_layer layer, double* in)
{
    image input = double_to_image(layer.h, layer.w, layer.c, in);
    image output = get_convolutional_image(layer);

    int i;
    for(i = 0; i < layer.n; ++i){
        convolve(input, layer.kernels[i], layer.stride, i, output);
    }
    for(int i=0; i < output.c; ++i){
        for(int j=0; j < output.h; ++j){
            for(int k=0; k<output.w; ++k){
                int index = i*output.h+output.w + j*output.w + k;
                layer.output[index] = get_pixel(output, j, k, i) + layer.bias[i];
                layer.output[index] = layer.activation(layer.output[index]);
            }
        }
    }
}

// 反向传播， 主要是计算delta
// 这里的input是当前层的输入， layer.delta是层输出的delta（delta（l+1)), delta是前一层的delta
void backward_convolutonal_layer(convolutional_layer layer, double *input, double OUT *delta)
{
    int i;

    image input_img = double_to_image(layer.h, layer.w, layer.c, input);
    image output_delta = get_convolutional_delta(layer);
    image in_delta = double_to_image(layer.h, layer.w, layer.c, delta); //输入端的delta
    zero_image(in_delta);
    for(i = 0; i < layer.n; ++i){
        back_convolve(in_delta, layer.kernels[i], layer.stride, i, output_delta); // TODO. kernel rotate?
    }
    for(int j=0; j<layer.h*layer.w*layer.c; ++j){
        in_delta.data[j] *= layer.gradient(input_img.data[j]);
    }
}

#if 0
void backpropagate_convolutional_layer_convolve(image input, convolutional_layer layer)
{
    int i,j;
    for(i = 0; i < layer.n; ++i){
        rotate_image(layer.kernels[i]);
    }

    zero_image(input);
    upsample_image(layer.output, layer.stride, layer.upsampled);
    for(j = 0; j < input.c; ++j){
        for(i = 0; i < layer.n; ++i){
            two_d_convolve(layer.upsampled, i, layer.kernels[i], j, 1, input, j);
        }
    }

    for(i = 0; i < layer.n; ++i){
        rotate_image(layer.kernels[i]);
    }
}
#endif

// 通过delta计算权重和偏置的更新项（即导数）
void learn_convolutional_layer(convolutional_layer layer, double *input)
{
    int i;
    image out_delta = get_convolutional_delta(layer);
    image input_img = double_to_image(layer.h, layer.w, layer.c, input);

    for(i = 0; i < layer.n; ++i){
        kernel_update(input_img, layer.kernel_updates[i], layer.stride, i, out_delta);
        layer.bias_updates[i] += avg_image_layer(out_delta, i);
    }
}

// step即learning rate
void update_convolutional_layer(convolutional_layer layer, double step)
{
    int i,j;
    for(i = 0; i < layer.n; ++i){
        int pixels = layer.kernels[i].h*layer.kernels[i].w*layer.kernels[i].c;
        for(j = 0; j < pixels; ++j){
            layer.kernels[i].data[j] += step*layer.kernel_updates[i].data[j];
        }
        layer.bias[i] += step *layer.bias_updates[i];
        zero_image(layer.kernel_updates[i]);
        layer.bias_updates[i] = 0;
    }
}

void visualize_convolutional_layer(convolutional_layer layer)
{
    int i;
    char buff[256];
    //image vis = make_image(layer.n*layer.size, layer.size*layer.kernels[0].c, 3);
    for(i = 0; i < layer.n; ++i){
        image k = layer.kernels[i];
        sprintf(buff, "Kernel %d", i);
        if(k.c <= 3) show_image(k, buff);
        else show_image_layers(k, buff);
    }
}
