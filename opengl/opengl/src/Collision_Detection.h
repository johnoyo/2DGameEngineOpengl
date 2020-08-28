#pragma once
#include "Renderer.h"
#include "Game.h"
#include "Player.h"

struct Vertex_Array* change_position_x(struct Vertex_Array* vertex, int size, float amount_x, float player_scale);

struct Vertex_Array* change_position_y(struct Vertex_Array* vertex, int size, float amount_y, float player_scale);

struct Vertex_Array* change_positions(struct Vertex_Array* vertex, int size, float amount_x, float amount_y);

struct Vertex_Array* check_corner_br_tl(struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

struct Vertex_Array* check_corner_tr_bl(struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

struct Vertex_Array* check_corner_tl_br(struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

struct Vertex_Array* check_corner_bl_tr(struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

struct Vertex_Array* check_for_collitions(struct Vertex_Array* vertex, int size, float player_scale, bool* is_grounded, bool* collides, int axis);
