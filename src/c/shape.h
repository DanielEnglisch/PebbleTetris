#ifndef SHAPE_H
#define SHAPE_H

#include "utilities.h"

typedef struct shape_t
{
  GRect box;
  GColor color_fill;
  GColor color_stroke;
} shape_t;


void draw_shape(int const size, shape_t const *const p_shape, Layer *const p_layer, GContext *const p_context);
bool shapes_intersect(shape_t const *const p_shape_a, shape_t const *const p_shape_b);
bool rect_intersects_rect(GRect const *const p_rect_a, GRect const *const p_rect_b);
bool point_in_shape(GPoint const *const p_point, shape_t const *const p_shape);


#endif
