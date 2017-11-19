#include "tetris.h"

uint16_t const BUTTON_INTERVAL = 50; // [ms]
int const SHAPE_MAX_H = 5;           // maximum height of a shape [squares]
int const SHAPE_MIN_H = 2;           // minimum height of a shape [squares]
int16_t const SQUARE_SIZE = 10;      // size of a square on the board [pels]
int const TEXT_LAYER_HEIGHT = 25;    // [pels]
uint32_t const TICK_INTERVAL = 600;  // [ms]

const int MAX_NUM_BLOCKS = 100;     // Max number of block in pile

int g_n_cols = 0; // number of columns on the board
int g_n_rows = 0; // number of rows on the board

Layer *g_p_layer = NULL;
TextLayer *g_p_text_layer = NULL;
AppTimer *g_p_timer = NULL;
Window *g_p_window = NULL;

block *g_p_current_shape = NULL;  // Currently active block
block **pile = NULL;              // Pile with static blocks
int num_pile = 0;                 // Number of static blocks
int score = 0;                    // Score
int difficulty = 0;               // Faster tick rate [ms]

int main()
{
  app_initialize();
  app_event_loop();
  app_cleanup();
}

void app_cleanup()
{
  APP_LOG_DEBUG("cleaning-up app ...");

  light_enable(false);
  window_destroy(g_p_window);

  APP_LOG_DEBUG("done cleaning-up app ...");
}

void app_initialize()
{
  APP_LOG_DEBUG("initializing app ...");

  // Initializing pile
  pile = (block **)malloc(sizeof(block *) * MAX_NUM_BLOCKS);

  WindowHandlers handlers = {
      on_window_load,  // load
      NULL,            // appear
      NULL,            // disappear
      on_window_unload // unload
  };

  g_p_window = window_create();

  window_set_click_config_provider(g_p_window, click_config_provider);
  window_set_window_handlers(g_p_window, handlers);

  window_stack_push(g_p_window, true);
  light_enable(true);

  APP_LOG_DEBUG("done initializing app ...");
  APP_LOG_DEBUG("enjoy playing ...");
}

void draw_grid(Layer *const p_layer, GContext *const p_context)
{
  graphics_context_set_fill_color(p_context, GColorWhite);
  graphics_context_set_stroke_color(p_context, GColorLightGray);
  graphics_context_set_stroke_width(p_context, 1);

  GRect const layer_bounds = layer_get_bounds(p_layer);

  graphics_fill_rect(p_context, layer_bounds, 0, GCornerNone);

  for (int i = 1; i < g_n_rows; ++i)
  {
    graphics_draw_line(
        p_context,
        GPoint(layer_bounds.origin.x + i * SQUARE_SIZE, layer_bounds.origin.y),
        GPoint(layer_bounds.origin.x + i * SQUARE_SIZE, layer_bounds.origin.y + layer_bounds.size.h));
  }

  for (int i = 1; i < g_n_cols; ++i)
  {
    graphics_draw_line(
        p_context,
        GPoint(layer_bounds.origin.x, layer_bounds.origin.y + i * SQUARE_SIZE),
        GPoint(layer_bounds.origin.x + layer_bounds.size.w, layer_bounds.origin.y + i * SQUARE_SIZE));
  }
}

bool is_end_position_block(block const *const b)
{
  for (int i = 0; i < b->num_shapes; i++)
    if (is_end_position(b->shapes[i]))
      return true;
  
  return false;
}

bool is_end_position(shape_t const *const p_shape)
{
  assert(p_shape != NULL);

  shape_t moved_shape = *p_shape;
  ++moved_shape.box.origin.x;

  return !is_valid_position(&moved_shape);
}

bool is_game_over(block const *const b)
{
  for (int i = 0; i < b->num_shapes; i++)
    if (!is_valid_position(b->shapes[i]))
      return true;
  
  return false;
}

bool is_valid_position(shape_t const *const p_shape)
{
  assert(p_shape != NULL);

  return !((p_shape->box.origin.x < 0) || (p_shape->box.origin.x + p_shape->box.size.w > g_n_cols - 1) ||
           (p_shape->box.origin.y < 0) || (p_shape->box.origin.y + p_shape->box.size.h > g_n_rows - 1));
}

bool blockIntersectsPileOrInvalidPos(block *b)
{
  // For every shape in block
  for (int i = 0; i < b->num_shapes; i++)
  {
    shape_t *s = b->shapes[i];
    // Check if not outside playfield
    if (!is_valid_position(s))
      return true;
    // Check intersection with other blocks
    for (int j = 0; j < num_pile; j++)
    {
      block *p = pile[j];
      for (int k = 0; k < p->num_shapes; k++)
        if (shapes_intersect(s, p->shapes[k]))
          return true;
    }
  }
  return false;
}

bool only_try_move_block(block *const b, int const dx, int const dy)
{
  // Try to move copy
  for (int i = 0; i < b->num_shapes; i++)
  {
    shape_t tmp = *(b->shapes[i]);
    if (!try_move_shape(&tmp, dx, dy))
      return false;
    else
    {
      // Check intersection with other blocks
      for (int j = 0; j < num_pile; j++)
      {
        block *b = pile[j];
        for (int k = 0; k < b->num_shapes; k++)
          if (shapes_intersect(&tmp, b->shapes[k]))
            return false;
      }
    }
  }

  return true;
}

bool try_move_block(block *const b, int const dx, int const dy)
{
   if(only_try_move_block(b,dx,dy)){
    for (int i = 0; i < b->num_shapes; i++)
      try_move_shape(b->shapes[i], dx, dy);
   }else
     return false;

  return true;
}

bool try_move_shape(shape_t *const p_shape, int const dx, int const dy)
{
  assert(p_shape != NULL);

  shape_t moved_shape = *p_shape;

  moved_shape.box.origin.x += dx;
  moved_shape.box.origin.y += dy;

  if (is_valid_position(&moved_shape))
  {
    *p_shape = moved_shape;
    return true;
  }

  return false;
}

void click_config_provider(void *const p_context)
{
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, BUTTON_INTERVAL, on_key_down_pressed);
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, BUTTON_INTERVAL, on_key_select_pressed);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, BUTTON_INTERVAL, on_key_up_pressed);
  window_single_click_subscribe(BUTTON_ID_BACK, on_key_back_pressed);
}

void on_key_back_pressed(ClickRecognizerRef recognizer, void *const p_context)
{

  if ((g_p_current_shape != NULL))
  {
    block *tmp = copyBlock(g_p_current_shape);

    rotateBlock(tmp);
    if (!blockIntersectsPileOrInvalidPos(tmp))
    {
      freeBlock(g_p_current_shape);
      g_p_current_shape = tmp;
      text_layer_set_text(g_p_text_layer, "Rotated block!");
      layer_mark_dirty(g_p_layer);
    }
    else
    {
      text_layer_set_text(g_p_text_layer, "Couldn't rotate block!");
      freeBlock(tmp);
    }
  }
}

void on_key_down_pressed(ClickRecognizerRef recognizer, void *const p_context)
{
  text_layer_set_text(g_p_text_layer, "Moving left");

  if ((g_p_current_shape != NULL) && try_move_block(g_p_current_shape, 0, 1))
    layer_mark_dirty(g_p_layer);
  
}

void on_key_select_pressed(ClickRecognizerRef recognizer, void *const p_context)
{
  text_layer_set_text(g_p_text_layer, "Drop it");

  if ((g_p_current_shape != NULL) && try_move_block(g_p_current_shape, 1, 0))
    layer_mark_dirty(g_p_layer);
  
}

void on_key_up_pressed(ClickRecognizerRef recognizer, void *const p_context)
{
  text_layer_set_text(g_p_text_layer, "Moving right");

  if ((g_p_current_shape != NULL) && try_move_block(g_p_current_shape, 0, -1))
    layer_mark_dirty(g_p_layer);
}

void on_layer_update(Layer *const p_layer, GContext *const p_context)
{
  draw_grid(p_layer, p_context);

  if (g_p_current_shape != NULL)
  {
    draw_block(SQUARE_SIZE, g_p_current_shape, p_layer, p_context);
    for (int i = 0; i < num_pile; i++)
      draw_block(SQUARE_SIZE, pile[i], p_layer, p_context);
    
  }
}

void resetGame()
{
  // Free Pile
  for (int i = 0; i < num_pile; i++)
    freeBlock(pile[i]);
  free(pile);
  
  pile = (block **)malloc(sizeof(block *) * MAX_NUM_BLOCKS);
  num_pile = 0;
  difficulty = 0;
  score = 0;
}

bool isColFull(int c)
{
  int lastLine_num = 0;
  for (int i = 0; i < num_pile; i++)
  {
    block *b = pile[i];
    for (int k = 0; k < b->num_shapes; k++)
    {
      shape_t *s = b->shapes[k];
      if (s->box.origin.x + s->box.size.w == c)
        lastLine_num++;
    }
  }
  return (g_n_cols - 1 == lastLine_num);
}

void removeShapesInCol(int c)
{
  // Remove blocks
  for (int i = 0; i < num_pile; i++)
  {
    block *b = pile[i];
    int removedshaped = 0;
    int oldNumShapes = b->num_shapes;
    for (int k = 0; k < b->num_shapes; k++)
    {
      shape_t *s = b->shapes[k];

      if (s->box.origin.x + s->box.size.w == c)
      {
        free(s);
        b->shapes[k] = NULL;
        removedshaped++;
      }
    }
    shape_t **newShapes = (shape_t **)malloc(sizeof(shape_t *) * b->num_shapes);
    int ind = 0;
    for (int l = 0; l < oldNumShapes; l++)
    {
      if (b->shapes[l] != NULL)
      {
        newShapes[ind++] = b->shapes[l];
      }
    }
    b->shapes = newShapes;
    b->num_shapes = oldNumShapes - removedshaped;
  }
}

void checkIfFullCol()
{

  for (int c = 0; c < g_n_cols; c++)
  {
    if (isColFull(c))
    {
      printf("Removin col %d\n", c);
      removeShapesInCol(c);
      
      // Move all shapes before c one down
      for(int i = 0; i < num_pile; i++){
        block* b = pile[i];
        for(int k = 0; k < b->num_shapes; k++){
          shape_t* s = b->shapes[k];
          if(s->box.origin.x < c){
            s->box.origin.x++;
          }
        }
      }
      
      //Update Score
      score += g_n_cols;
      printf("New Score %d\n", score);
      layer_mark_dirty(g_p_layer);
    }
  }
}

void on_timer_tick(void *const p_data)
{
  g_p_timer = app_timer_register(TICK_INTERVAL-difficulty, on_timer_tick, NULL);

  // Create new block
  if (g_p_current_shape == NULL)
  {
    g_p_current_shape = make_random_block(g_n_rows, SHAPE_MIN_H, SHAPE_MAX_H);
    layer_mark_dirty(g_p_layer);
    // If new block cant be moved -> game over
    if (!only_try_move_block(g_p_current_shape, 1, 0))
    {
      resetGame();
      text_layer_set_text(g_p_text_layer, "GAME OVER");
    }
    layer_mark_dirty(g_p_layer);
  }
  // Move block and display current score
  else if (g_p_current_shape && try_move_block(g_p_current_shape, 1, 0))
  {
    // Display Score
    char *buffer = (char *)malloc(sizeof(char) * 100);
    snprintf(buffer, 100, "Score: %d", score);
    text_layer_set_text(g_p_text_layer, buffer);
    layer_mark_dirty(g_p_layer);
  }
  // If it can't be moved add to pile
  else
  {
    pile[num_pile++] = g_p_current_shape;
    g_p_current_shape = NULL;

    // Check if a column is full
    checkIfFullCol();
    
    // Speed increase
    if(difficulty < 300)
      difficulty+=10;
    
  }
}

void on_window_load(Window *const p_window)
{
  APP_LOG_DEBUG("loading window ...");

  Layer *const p_root_layer = window_get_root_layer(p_window);
  GRect const root_bounds = layer_get_bounds(p_root_layer);

  int const w = floor_div_mult(root_bounds.size.w, SQUARE_SIZE) + 1;                     // width of the board [pels]
  int const h = floor_div_mult(root_bounds.size.h - TEXT_LAYER_HEIGHT, SQUARE_SIZE) + 1; // height of the board [pels]

  g_p_layer = layer_create(GRect(0, 0, w, h));
  g_p_text_layer = text_layer_create(GRect(0, root_bounds.size.h - TEXT_LAYER_HEIGHT + 1, w, TEXT_LAYER_HEIGHT));

  text_layer_set_text(g_p_text_layer, "Enjoy playing!");
  text_layer_set_text_alignment(g_p_text_layer, GTextAlignmentCenter);

  layer_add_child(p_root_layer, g_p_layer);
  layer_add_child(p_root_layer, text_layer_get_layer(g_p_text_layer));

  GRect const layer_bounds = layer_get_bounds(g_p_layer);

  g_n_rows = ceil_div(layer_bounds.size.h, SQUARE_SIZE);
  g_n_cols = ceil_div(layer_bounds.size.w, SQUARE_SIZE);

  g_p_timer = app_timer_register(TICK_INTERVAL, on_timer_tick, NULL);

  layer_set_update_proc(g_p_layer, on_layer_update);

  APP_LOG_DEBUG("done loading window ...");
}

void on_window_unload(Window *const p_window)
{
  APP_LOG_DEBUG("unloading window ...");

  if (g_p_current_shape != NULL)
  {
    free(g_p_current_shape);
    g_p_current_shape = NULL;
  }

  if (g_p_timer != NULL)
  {
    app_timer_cancel(g_p_timer);
  }

  if (g_p_text_layer != NULL)
  {
    text_layer_destroy(g_p_text_layer);
  }

  if (g_p_layer != NULL)
  {
    layer_destroy(g_p_layer);
  }

  APP_LOG_DEBUG("done unloading window ...");
}
