#include "block.h"
#include "tetriminos.h"

// To know which type was lastly generated
int type = 0;

void draw_block(int const size, block const *const b, Layer *const p_layer, GContext *const p_context)
{
  for (int i = 0; i < b->num_shapes; i++)
    draw_shape(size, b->shapes[i], p_layer, p_context);
}

// Returns int array of randonly chosen block
int* getRandomBlock()
{
  int const c = random(num_tetriminos + 1);

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
  // Only 4 possible orientations
  if (b->rotation == 3)
    b->rotation = 0;
  else
    b->rotation++;

  // Save constant block data
  int xOffset = b->shapes[0]->box.origin.x;
  int yOffset = b->shapes[0]->box.origin.y;
  GColor fill = b->shapes[0]->color_fill;
  GColor stroke = b->shapes[0]->color_stroke;

  // Free old shape
  for (int i = 0; i < b->num_shapes; i++)
    free(b->shapes[i]);
  free(b->shapes);

  // Generate new shape based on rotaiton and type
  // Count number of 1s in array
  int num_shapes = 0;
  for (int i = 0; i < TERM_ARR_SIZE; i++)
  {
    if (shape_l[b->rotation][i] == 1)
      num_shapes++;
  }
  
  // Get new orientation of current type
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

  // Create and populate shape array
  shape_t **shapearr = (shape_t **)malloc(sizeof(shape_t *) * num_shapes);
  int arr_ind = 0;
  for (int y = 0; y < NUM_ROW_COL; y++)
  {
    for (int x = 0; x < NUM_ROW_COL; x++)
    {
      if (myType[y * NUM_ROW_COL + x] == 1)
      {
        shape_t *newShape = (shape_t *)malloc(sizeof(shape_t));
        newShape->color_fill = fill;
        newShape->color_stroke = stroke;
        newShape->box = GRect(x + xOffset, y + yOffset, 1, 1);
        shapearr[arr_ind++] = newShape;
      }
    }
  }
  
  // Adjust numbers
  b->num_shapes = num_shapes;
  b->shapes = shapearr;
}

block *make_random_block(int const n, int const min_h, int const max_h)
{

  int *shape = getRandomBlock();
  int num_shapes = 0;
  for (int i = 0; i < TERM_ARR_SIZE; i++)
    if (shape[i] == 1)
      num_shapes++;
  
  // Generate random color
  int const r = random(256);
  int const g = random(256);
  int const b = random(256);

  int const h = random_range(min_h, max_h);
  int const yOffset = random(n - h);
  
  // Generate and populate shapearray
  shape_t **shapearr = (shape_t **)malloc(sizeof(shape_t *) * num_shapes);
  int arr_ind = 0;
  for (int y = 0; y < NUM_ROW_COL; y++)
  {
    for (int x = 0; x < NUM_ROW_COL; x++)
    {
      if (shape[y * NUM_ROW_COL + x] == 1)
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


block *copyBlock(block *b)
{
  block *newBlock = (block *)malloc(sizeof(block));
  newBlock->num_shapes = b->num_shapes;
  newBlock->rotation = b->rotation;
  newBlock->type = b->type;
  newBlock->shapes = (shape_t **)malloc(sizeof(shape_t *) * b->num_shapes);
  for (int i = 0; i < b->num_shapes; i++)
  {
    shape_t *newShape = (shape_t *)malloc(sizeof(shape_t));
    newShape->color_fill = b->shapes[i]->color_fill;
    newShape->color_stroke = b->shapes[i]->color_stroke;
    newShape->box = b->shapes[i]->box;
    newBlock->shapes[i] = newShape;
  }
  return newBlock;
}

void freeBlock(block *b)
{
  for (int k = 0; k < b->num_shapes; k++)
    free(b->shapes[k]);
  free(b);
}