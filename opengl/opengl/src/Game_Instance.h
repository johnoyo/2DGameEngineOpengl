#pragma once
#include "Game.h"
#include "Collision_Detection.h"
#include "Input.h"

class Game_Instance : public Game {

	/* This is so the class Game_Instance also inherits the constructor of class Game */
	using Game::Game;

public:
	float amount_x = 0.0, amount_y = 0.0, amount_xx = 0.0, amount_yy = 0.0, scale_h = 0.0, scale_v = 0.0;
	//float refresh_rate = 60.0f;

	double previous_time = glfwGetTime();
	int frame_count = 0;
	float current_time = 0.0, dt = 0.0;

	void init() override {
		std::cout << "Player index: " << p1.get_buffer_index()[0] << ", " << p1.get_buffer_index()[1] << ", " << p1.get_buffer_index()[2] << ", " << p1.get_buffer_index()[3] << "\n";
		std::cout << "Enemy index: " << enemies_list.at(0).get_buffer_index()[0] << ", " << enemies_list.at(0).get_buffer_index()[1] << ", " << enemies_list.at(0).get_buffer_index()[2] << ", " << enemies_list.at(0).get_buffer_index()[3] << "\n";

		//p1.set_texture_id(texture_slot[0] - 1);
	}

	void update() override {
		/* display fps every sec */
		displayFPS(&frame_count, &previous_time);

		float new_time = glfwGetTime();
		float frame_time = new_time - current_time;
		current_time = new_time;

		if (Is_Grounded_y) amount_x = 0.0;

		/* Check for input on x-axis */
		handle_input_hor(get_window(), &p1, 26.0f, &amount_x, get_size(), &scale_h, Is_Grounded_y, refresh_rate);
		//p1.change_position(glm::vec2(amount_x, 0.0f));

		/* Check for input on y-axis */
		handle_input_vert(get_window(), &p1, 26.0f, &amount_y, get_size(), &scale_v, Is_Grounded_y, Collides_y, refresh_rate);
		//p1.change_position(glm::vec2(0.0f, amount_y));

		//std::cout << p1.get_position().x << ", " << p1.get_position().y << "\n";

		/* TODO: find out why "scale_v" it doesnt work by reference */
		if (amount_y > 0) scale_v = -26.0f;
		else scale_v = 26.0f;

		/* clamp y-axis speed*/
		if (amount_y > 100.0f) amount_y = 100.0f;
		if (amount_y < -100.0f) amount_y = -100.0f;

		
		if (check_if_obj_collides_with_obj(p1, enemies_list.at(0), buffer, get_size())) {
			std::cout << "Collition Detected beetwen player and enemy\n";
			enemies_list.at(0).change_position(new_position(27.0f, 0.0f));
			p1.fix_position(new_position(270.0f, 270.0f));
		}

	}
};