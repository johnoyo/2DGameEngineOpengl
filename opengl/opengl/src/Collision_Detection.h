#pragma once
#include "Renderer.h"
#include "Game.h"
#include "Player.h"

struct Vertex_Array* change_position_x(Player p, struct Vertex_Array* vertex, int size, float amount_x, float player_scale);

struct Vertex_Array* change_position_y(Player p, struct Vertex_Array* vertex, int size, float amount_y, float player_scale);

struct Vertex_Array* change_positions(Player p, struct Vertex_Array* vertex, int size, float amount_x, float amount_y);

struct Vertex_Array* check_corner_br_tl(Player p, struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

struct Vertex_Array* check_corner_tr_bl(Player p, struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

struct Vertex_Array* check_corner_tl_br(Player p, struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

struct Vertex_Array* check_corner_bl_tr(Player p, struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

struct Vertex_Array* check_for_collitions(struct Vertex_Array* vertex, Player p, int size, float player_scale, bool* is_grounded, bool* collides, int axis);

bool check_corner_br_tl_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1);

bool check_corner_tr_bl_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1);

bool check_corner_tl_br_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1);

bool check_corner_bl_tr_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1);

bool check_if_obj_collides_with_obj(Player obj1, Player obj2, struct Vertex_Array* vertex, int size);
