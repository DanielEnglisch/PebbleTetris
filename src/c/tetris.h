#ifndef TETRIS_H
#define TETRIS_H

#include "block.h"

#include <stdio.h>
void app_cleanup();
void app_initialize();
void draw_grid(Layer *const p_layer, GContext *const p_context);
bool is_end_position(shape_t const *const p_shape);
bool is_valid_position(shape_t const *const p_shape);
bool try_move_shape(shape_t *const p_shape, int const dx, int const dy);
void click_config_provider(void *const p_context);

void on_key_back_pressed(ClickRecognizerRef recognizer, void *const p_context);
void on_key_down_pressed(ClickRecognizerRef recognizer, void *const p_context);
void on_key_select_pressed(ClickRecognizerRef recognizer, void *const p_context);
void on_key_up_pressed(ClickRecognizerRef recognizer, void *const p_context);
void on_layer_update(Layer *const p_layer, GContext *const p_context);
void on_timer_tick(void *const p_data);
void on_window_load(Window *const p_window);
void on_window_unload(Window *const p_window);

// My Methods
bool is_end_position_block(block const *const b);
bool only_try_move_block(block *const b, int const dx, int const dy);
bool try_move_block(block *const b, int const dx, int const dy);
bool is_game_over(block const *const b);
bool blockIntersectsPileOrInvalidPos(block *b);
void clear();
bool isColFull(int c);
void removeShapesInCol(int c);
void tryMoveAllShapes();
void checkCutter();
#endif