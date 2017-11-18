//       $Id: shape.h 36046 2017-11-11 15:09:28Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/bin/cepheiden/vocational/teaching/SE/SWO3/2017-WS/Ue/src/06/tetris-template/src/shape.h $
// $Revision: 36046 $
//     $Date: 2017-11-11 16:09:28 +0100 (Sa., 11 Nov 2017) $
//   Creator: erik.pitzer<AT>fh-hagenberg.at
//            johann.heinzelreiter<AT>fh-hagenberg.at
//            peter.kulczycki<AT>fh-hagenberg.at
//   $Author: p20068 $

#if !defined SHAPE_H
#define      SHAPE_H

#include "./utilities.h"

// -------------------------------------------------------------------------------------------------


typedef struct shape_t {
   GRect  box;
   GColor color_fill;
   GColor color_stroke;
} shape_t;

typedef struct block{
  shape_t** shapes;
  int num_shapes;
  int rotation;
  int type;
}block;



// -------------------------------------------------------------------------------------------------

bool point_in_shape       (GPoint const * const p_point, shape_t const * const p_shape);
bool rect_intersects_rect (GRect const * const p_rect_a, GRect const * const p_rect_b);
bool shapes_intersect     (shape_t const * const p_shape_a, shape_t const * const p_shape_b);

void      draw_shape        (int const size, shape_t const * const p_shape, Layer * const p_layer, GContext * const p_context);
void      draw_block        (int const size, block const * const p_shape, Layer * const p_layer, GContext * const p_context);
void rotateBlock(block* b);
block * make_random_block (int const n, int const min_h, int const max_h);


// -------------------------------------------------------------------------------------------------

#endif   // SHAPE_H
