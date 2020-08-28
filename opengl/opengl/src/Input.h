#pragma once
#include "Renderer.h"
#include "Player.h"

int handle_input_hor(GLFWwindow* window, Player *p, float player_scale, float* amount_x, float size, float* scale, bool is_grounded, int refresh_rate);
int handle_input_vert(GLFWwindow* window, Player *p, float player_scale, float* amount_y, float size, float* scale, bool is_grounded, bool collides, int refresh_rate);

void displayFPS(int* frame_count, double* previous_time);