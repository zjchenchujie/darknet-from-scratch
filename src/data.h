#ifndef DATA_H
#define DATA_H

#include "image.h"

typedef struct{
    int n; // batch_size
    image *images; // image
    double **truth; // label
} batch;

batch get_all_data(char *filename);
batch random_batch(char *filename, int n);
batch get_batch(char *filename, int curr, int total);
void free_batch(batch b);


#endif
