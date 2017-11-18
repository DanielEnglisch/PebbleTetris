#if !defined SHAPE_H
#define SHAPE_H

#include "./utilities.h"

typedef struct shape_t
{
  GRect box;
  GColor color_fill;
  GColor color_stroke;
} shape_t;

typedef struct block
{
  shape_t **shapes;
  int num_shapes;
  int rotation;
  int type;
} block;

bool point_in_shape(GPoint const *const p_point, shape_t const *const p_shape);
bool rect_intersects_rect(GRect const *const p_rect_a, GRect const *const p_rect_b);
bool shapes_intersect(shape_t const *const p_shape_a, shape_t const *const p_shape_b);

void draw_shape(int const size, shape_t const *const p_shape, Layer *const p_layer, GContext *const p_context);
void draw_block(int const size, block const *const p_shape, Layer *const p_layer, GContext *const p_context);
void rotateBlock(block *b);
block *make_random_block(int const n, int const min_h, int const max_h);

#endif
