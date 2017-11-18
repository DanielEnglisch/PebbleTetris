#include "./shape.h"

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

void draw_block(int const size, block const *const p_shape, Layer *const p_layer, GContext *const p_context)
{
  for (int i = 0; i < p_shape->num_shapes; i++)
  {
    draw_shape(size, p_shape->shapes[i], p_layer, p_context);
  }
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

#include "tetriminos.h"
int type = 0;

int *getRandomBlock()
{
  int const c = random(num_tetriminos + 1);
  printf("Chose block %d", c);
  switch (c)
  {
  case 1:
    type = 0;
    return shape_l[0];
    break;
  case 2:
    type = 1;
    return shape_i[0];
    break;
  case 3:
    type = 2;
    return shape_l_inv[0];
    break;
  case 4:
    type = 3;
    return shape_q[0];
    break;
  case 5:
    type = 4;
    return shape_sn[0];
    break;
  case 6:
    type = 5;
    return shape_sn_inv[0];
    break;
  case 7:
    type = 6;
    return shape_tee[0];
    break;
  }
  type = 0;
  return shape_l[0];
}

void rotateBlock(block *b)
{
  if (b->rotation == 3)
  {
    b->rotation = 0;
  }
  else
    b->rotation++;

  int xOffset = b->shapes[0]->box.origin.x;
  int yOffset = b->shapes[0]->box.origin.y;
  GColor fill = b->shapes[0]->color_fill;
  GColor stroke = b->shapes[0]->color_stroke;

  // Free old shape
  for (int i = 0; i < b->num_shapes; i++)
  {
    free(b->shapes[i]);
  }
  free(b->shapes);

  // Generate new shape based on rotaiton and type
  int num_shapes = 0;
  for (int i = 0; i < 16; i++)
  {
    if (shape_l[b->rotation][i] == 1)
      num_shapes++;
  }

  int *myType = NULL;
  switch (b->type)
  {
  case 0:
    myType = shape_l[b->rotation];
    break;
  case 1:
    myType = shape_i[b->rotation];
    break;
  case 2:
    myType = shape_l_inv[b->rotation];
    break;
  case 3:
    myType = shape_q[b->rotation];
    break;
  case 4:
    myType = shape_sn[b->rotation];
    break;
  case 5:
    myType = shape_sn_inv[b->rotation];
    break;
  case 6:
    myType = shape_tee[b->rotation];
    break;
  }

  shape_t **shapearr = (shape_t **)malloc(sizeof(shape_t *) * num_shapes);
  int arr_ind = 0;
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 4; x++)
    {
      if (myType[y * 4 + x] == 1)
      {
        shape_t *newShape = (shape_t *)malloc(sizeof(shape_t));
        newShape->color_fill = fill;
        newShape->color_stroke = stroke;
        newShape->box = GRect(x + xOffset, y + yOffset, 1, 1);
        shapearr[arr_ind++] = newShape;
      }
    }
  }

  b->num_shapes = num_shapes;
  b->shapes = shapearr;
}

block *make_random_block(int const n, int const min_h, int const max_h)
{

  int *shape1 = getRandomBlock();
  int num_shapes = 0;
  for (int i = 0; i < 16; i++)
  {
    if (shape1[i] == 1)
      num_shapes++;
  }

  int const r = random(256);
  int const g = random(256);
  int const b = random(256);

  int const h = random_range(min_h, max_h);
  int const yOffset = random(n - h);

  shape_t **shapearr = (shape_t **)malloc(sizeof(shape_t *) * num_shapes);
  int arr_ind = 0;
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 4; x++)
    {
      if (shape1[y * 4 + x] == 1)
      {
        {
          shape_t *newShape = (shape_t *)malloc(sizeof(shape_t));
          newShape->color_fill = GColorFromRGB(r - 1, g - 1, b - 1);
          newShape->color_stroke = GColorFromRGB(0, 0, 0);
          newShape->box = GRect(x, y + yOffset, 1, 1);
          shapearr[arr_ind++] = newShape;
        }
      }
    }
  }

  block *newBlock = (block *)malloc(sizeof(block));
  newBlock->num_shapes = num_shapes;
  newBlock->shapes = shapearr;
  newBlock->type = type;
  newBlock->rotation = 0;
  return newBlock;
}
