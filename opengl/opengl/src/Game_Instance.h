#pragma once
#include "Game.h"
#include "Collision_Detection.h"
#include "Input.h"

class Game_Instance : public Game {

	/* This is so the class Game_Instance also inherits the constructor of class Game */
	using Game::Game;

public:
	float amount_x = 0.0, amount_y = 0.0, amount_xx = 0.0, amount_yy = 0.0, scale_h = 0.0, scale_v = 0.0;

	double previous_time = glfwGetTime();
	int frame_count = 0;
	float current_time = 0.0, dt = 0.0;

	double previous_time1 = glfwGetTime();
	int frame_count1 = 0;
	float current_time1 = 0.0;

	void init() override {
		//std::cout << "Player index: " << p1.get_buffer_index()[0] << ", " << p1.get_buffer_index()[1] << ", " << p1.get_buffer_index()[2] << ", " << p1.get_buffer_index()[3] << "\n";
		//std::cout << "Enemy index: " << enemies_list.at(0).get_buffer_index()[0] << ", " << enemies_list.at(0).get_buffer_index()[1] << ", " << enemies_list.at(0).get_buffer_index()[2] << ", " << enemies_list.at(0).get_buffer_index()[3] << "\n";

		//p1.set_texture_id(texture_slot[0] - 1);
	}

	void update() override {

		/* display fps every sec */
		displayFPS(&frame_count, &previous_time);

		float new_time = glfwGetTime();
		float frame_time = new_time - current_time;
		current_time = new_time;

		if (current_level == 0) {
			if (glfwGetKey(get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
			else if (glfwGetKey(get_window(), GLFW_KEY_SPACE) == GLFW_PRESS) {
				std::string level1 = "res/levels/test1.txt";
				Load_Next_Level(level1, 945.0f, 540.0f, 27.0f);
			}
		}
		else
		{
			double current_time1 = glfwGetTime();
			frame_count1++;
			/* If a second has passed. */
			if (current_time1 - previous_time1 >= 1.0)
			{
				/* Display the frame count here any way you want. */
				if (buffer[5].position.x > 0.0f) {

					std::cout << "Health decreasing...\n";
					buffer[5].position.x -= tile_size;
					buffer[6].position.x -= tile_size;

				}
				else {
					std::cout << "Game Over!!!\n";
					Game_Over();
				}

				frame_count1 = 0;
				previous_time1 = current_time1;
			}

			float new_time1 = glfwGetTime();
			float frame_time1 = new_time1 - current_time1;
			current_time1 = new_time1;

			if (Is_Grounded_y) amount_x = 0.0;

			/* Check for input on x-axis */
			handle_input_hor(get_window(), &p1, 26.0f, &amount_x, get_size(), &scale_h, Is_Grounded_y, refresh_rate);

			/* Check for input on y-axis */
			handle_input_vert(get_window(), &p1, 26.0f, &amount_y, get_size(), &scale_v, Is_Grounded_y, Collides_y, refresh_rate);

			/* TODO: find out why "scale_v" it doesnt work by reference */
			if (amount_y > 0) scale_v = -26.0f;
			else scale_v = 26.0f;

			/* clamp y-axis speed*/
			if (amount_y > 100.0f) amount_y = 100.0f;
			if (amount_y < -100.0f) amount_y = -100.0f;
			
			for (int i = 0; i < enemies_list.size(); i++) {

				if (check_if_obj_collides_with_obj(p1, enemies_list.at(i), buffer, get_size())) {
					std::cout << "Collition Detected beetwen player and enemy\n";
					p1.fix_position(new_position(81.0f, 27.0f));
				}

			}

			for (int i = 0; i < collectible_list.size(); i++) {

				if (check_if_obj_collides_with_obj(p1, collectible_list.at(i), buffer, get_size())) {
					collectible_list.at(i).despawn();
					if (buffer[5].position.x <= 945.0f) {

						std::cout << "Health increased...\n";
						buffer[5].position.x += 3*tile_size;
						buffer[6].position.x += 3*tile_size;

					}
				}

			}

			if (current_level == 1) {

				if (check_if_obj_collides_with_obj(p1, Next_Level, buffer, get_size())) {

					std::cout << "Changing level\n";
					Next_Level.despawn();
					std::string level2 = "res/levels/test2.txt";
					Load_Next_Level(level2, 945.0f, 540.0f, 27.0f);
					
				}

			}
			else if (current_level == 2) {



			}

		}


	}
};
