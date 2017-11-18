#include "shape.h"

bool point_in_shape(GPoint const *const p_point, shape_t const *const p_shape)
{
  assert(p_shape != NULL);
  return grect_contains_point(&(p_shape->box), p_point);
}

bool rect_intersects_rect(GRect const *const p_rect_a, GRect const *const p_rect_b)
{
  assert(p_rect_b != NULL);

  return grect_contains_point(p_rect_a, &GPoint(p_rect_b->origin.x, p_rect_b->origin.y)) || // p_rect_b->origin
         grect_contains_point(p_rect_a, &GPoint(p_rect_b->origin.x + p_rect_b->size.w - 1, p_rect_b->origin.y)) ||
         grect_contains_point(p_rect_a, &GPoint(p_rect_b->origin.x, p_rect_b->origin.y + p_rect_b->size.h - 1)) ||
         grect_contains_point(p_rect_a, &GPoint(p_rect_b->origin.x + p_rect_b->size.w - 1, p_rect_b->origin.y + p_rect_b->size.h - 1));
}

bool shapes_intersect(shape_t const *const p_shape_a, shape_t const *const p_shape_b)
{
  assert(p_shape_a != NULL);
  assert(p_shape_b != NULL);

  return rect_intersects_rect(&(p_shape_b->box), &(p_shape_a->box)) ||
         rect_intersects_rect(&(p_shape_a->box), &(p_shape_b->box));
}



void draw_shape(int const size, shape_t const *const p_shape, Layer *const p_layer, GContext *const p_context)
{
  graphics_context_set_fill_color(p_context, p_shape->color_fill);
  graphics_context_set_stroke_color(p_context, p_shape->color_stroke);
  graphics_context_set_stroke_width(p_context, 1);

  GRect b = p_shape->box;
  GRect g = GRect(b.origin.x * size,
                  b.origin.y * size,
                  b.size.w * size,
                  b.size.h * size);

  graphics_fill_rect(p_context, g, 0, GCornerNone);

  graphics_draw_rect(p_context, g);
}

