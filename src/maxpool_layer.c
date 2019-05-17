#include "maxpool_layer.h"

// get maxpool ouput image
image get_maxpool_image(maxpool_layer layer){
    int h = (layer.h-1)/layer.stride + 1;
    int w = (layer.w-1)/layer.stride + 1;
    int c = layer.c;
    return double_to_image(h,w,c,layer.output);
}

maxpool_layer* make_maxpool_layer(int h, int w, int c, int stride)
{
    printf("Maxpool Layer: %d x %d x %d image, %d stride\n", h,w,c,stride);
    maxpool_layer* layer = calloc(1, sizeof(maxpool_layer));
    layer->h = h;
    layer->w = w;
    layer->c = c;
    layer->stride = stride;
    layer->delta = calloc(((h -1 )/stride + 1) * ((w-1)/stride + 1) * c, sizeof(double));
    layer->output = calloc(((h -1 )/stride + 1) * ((w-1)/stride + 1) * c, sizeof(double));
    return layer;
}

void forward_maxpool_layer(const maxpool_layer layer, double *input)
{
    image input_img = double_to_image(layer.h, layer.w, layer.c, input);
    image output = get_maxpool_image(layer);
    int n;
    for(n=0; n<output.h*output.w*output.c; ++n){
        output.data[n] = -DBL_MAX;
    }
    int i, j, k;
    for(k=0; k<input_img.c; ++k){
        for(j=0; j<input_img.h; ++j){
            for(i=0; i<input_img.w; ++i){
                double in_val = get_pixel(input_img, i, j, k);
                double out_cur = get_pixel(output, i/layer.stride, j/layer.stride, k);
                if(in_val > out_cur){
                    set_pixel(output, i/layer.stride, j/layer.stride, k, in_val);
                }
            }
        }
    }
}
