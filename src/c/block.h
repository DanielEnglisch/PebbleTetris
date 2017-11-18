#ifndef BLOCK_H
#define BLOCK_H

#include "shape.h"

// My Methods:
typedef struct block
{
  shape_t **shapes;
  int num_shapes;
  int rotation;
  int type;
} block;

void draw_block(int const size, block const *const b, Layer *const p_layer, GContext *const p_context);
block *make_random_block(int const n, int const min_h, int const max_h);
void rotateBlock(block *b);
int *getRandomBlock();
block *copyBlock(block *b);
void freeBlock(block *b);


#endif