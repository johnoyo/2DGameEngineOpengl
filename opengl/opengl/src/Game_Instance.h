#pragma once
#include "Game.h"
#include "Collision_Detection.h"
#include "Input.h"

class Game_Instance : public Game {

	/* This is so the class Game_Instance also inherits the constructor of class Game */
	using Game::Game;

public:
	float amount_x = 0.0, amount_y = 0.0, amount_xx = 0.0, amount_yy = 0.0, scale_h = 0.0, scale_v = 0.0;
	float x = 0.0f, y = 0.0f, y1 = 0.0f;

	float xx = 0.0f, yy = 0.0f, angle = 0.0f, angle1 = 180.0f;;

	double previous_time = glfwGetTime();
	int frame_count = 0;
	float current_time = 0.0, dt = 0.0;

	double previous_time1 = glfwGetTime();
	int frame_count1 = 0;
	float current_time1 = 0.0;

	double previous_time2 = glfwGetTime();
	int frame_count2 = 0;
	float current_time2 = 0.0;

	bool despawn = true;

	std::vector<glm::vec2> pos;

	void init() override {
		//std::cout << "Player index: " << p1.get_buffer_index()[0] << ", " << p1.get_buffer_index()[1] << ", " << p1.get_buffer_index()[2] << ", " << p1.get_buffer_index()[3] << "\n";
		//std::cout << "Enemy index: " << enemies_list.at(0).get_buffer_index()[0] << ", " << enemies_list.at(0).get_buffer_index()[1] << ", " << enemies_list.at(0).get_buffer_index()[2] << ", " << enemies_list.at(0).get_buffer_index()[3] << "\n";

		p1.set_texture_id(0);
		
	}

	void update() override {

		/* display fps every sec */
		//displayFPS(&frame_count, &previous_time);

		float new_time = glfwGetTime();
		float frame_time = new_time - current_time;
		current_time = new_time;

		if (current_level == 0) {
			if (glfwGetKey(get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
			else if (glfwGetKey(get_window(), GLFW_KEY_SPACE) == GLFW_PRESS) {
				std::string level1 = "res/levels/test1.txt";
				Player health_bar = Player(5, new_position(0.0, 540.0f), new_position(945.0f, 540.0f), new_position(945.0f, 513.0f), new_position(0.0, 513.0f));
				custom_sprite_list.push_back(health_bar);
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
				// Display the frame count here any way you want.
				if (buffer[custom_sprite_list.at(0).get_buffer_index()[1]].position.x > 0.0f) {

					//std::cout << "Health decreasing...\n";
					Change_Sprite_Scale(custom_sprite_list.at(0), -tile_size);

				}
				else {
					std::cout << "Game Over!!!\n";
					Game_Over(2.0f);
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

			if (scale_h > 0) p1.set_texture_id(7);
			else  p1.set_texture_id(0);

			/* clamp y-axis speed*/
			if (amount_y > 100.0f) amount_y = 100.0f;
			if (amount_y < -100.0f) amount_y = -100.0f;
			
			for (int i = 0; i < enemies_list.size(); i++) {

				if (check_if_obj_collides_with_obj(p1, enemies_list.at(i), buffer, get_size())) {
					//std::cout << "Collition Detected beetwen player and enemy\n";
					p1.fix_position(new_position(81.0f, 27.0f));
					amount_x = 0.0f;
					amount_y = 0.0f;
					Change_Sprite_Scale(custom_sprite_list.at(0), -tile_size/2);
				}

			}

			for (int i = 0; i < collectible_list.size(); i++) {

				if (check_if_obj_collides_with_obj(p1, collectible_list.at(i), buffer, get_size())) {
					collectible_list.at(i).despawn();
					if (buffer[custom_sprite_list.at(0).get_buffer_index()[1]].position.x <= 945.0f) {

						//std::cout << "Health increased...\n";
						Change_Sprite_Scale(custom_sprite_list.at(0), 3*tile_size);

					}
				}

			}

			if (current_level == 1) {

				if (check_if_obj_collides_with_obj(p1, Next_Level, buffer, get_size())) {

					std::cout << "Changing level\n";
					amount_x = 0.0f;
					amount_y = 0.0f;
					Next_Level.despawn();
					std::string level2 = "res/levels/test2.txt";
					Load_Next_Level(level2, 945.0f, 540.0f, 27.0f);
					
				}

			}
			else if (current_level == 2) {

				if (enemies_list.at(enemies_list.size()-1).get_position().x >= 837.0f)
				{
					x = -6.0f;
				}
				else if (enemies_list.at(enemies_list.size() - 1).get_position().x <= 27.0f)
				{
					x = 6.0f;
				}

				enemies_list.at(enemies_list.size() - 1).change_position(new_position(x, 0.0f));

				if (enemies_list.at(0).get_position().y >= 459.0f) y = -2.0f;
				else if (enemies_list.at(0).get_position().y <= 351.0f) y = 2.0f;

				for(int i = 0; i < 8; i++)
				 enemies_list.at(i).change_position(new_position(0.0f, y));

				if (enemies_list.at(8).get_position().y >= 459.0f) y1 = -2.0f;
				else if (enemies_list.at(8).get_position().y <= 351.0f) y1 = 2.0f;

				for (int i = 8; i < 16; i++)
					enemies_list.at(i).change_position(new_position(0.0f, y1));

				if (check_if_obj_collides_with_obj(p1, Next_Level, buffer, get_size())) {

					std::cout << "Changing level\n";
					amount_x = 0.0f;
					amount_y = 0.0f;
					Next_Level.despawn();
					std::string level3 = "res/levels/test3.txt";
					Load_Next_Level(level3, 945.0f, 540.0f, 27.0f);

				}

			}
			else if (current_level == 3) {

				xx = 243.0f + std::cos(angle * PI / 180.0f) * 54.0f;
				yy = 405.0f + std::sin(angle * PI / 180.0f) * 54.0f;

				enemies_list.at(0).fix_position(new_position(xx, yy));

				xx = 243.0f + std::cos(angle1 * PI / 180.0f) * 54.0f;
				yy = 405.0f + std::sin(angle1 * PI / 180.0f) * 54.0f;

				enemies_list.at(1).fix_position(new_position(xx, yy));

				xx = 675.0f + std::cos(angle * PI / 180.0f) * 54.0f;
				yy = 405.0f + std::sin(angle * PI / 180.0f) * 54.0f;

				enemies_list.at(2).fix_position(new_position(xx, yy));

				xx = 675.0f + std::cos(angle1 * PI / 180.0f) * 54.0f;
				yy = 405.0f + std::sin(angle1 * PI / 180.0f) * 54.0f;

				enemies_list.at(3).fix_position(new_position(xx, yy));

				xx = 459.0f + std::cos(angle * PI / 180.0f) * 54.0f;
				yy = 405.0f + std::sin(angle * PI / 180.0f) * 54.0f;

				enemies_list.at(4).fix_position(new_position(xx, yy));

				xx = 459.0f + std::cos(angle1 * PI / 180.0f) * 54.0f;
				yy = 405.0f + std::sin(angle1 * PI / 180.0f) * 54.0f;

				enemies_list.at(5).fix_position(new_position(xx, yy));

				angle += 4;
				if (angle > 360.0f) angle = 0.0f;
				angle1 += 4;
				if (angle1 > 360.0f) angle1 = 0.0f;

				if (enemies_list.at(enemies_list.size() - 4).get_position().x >= 405.0f) 
					x = -6.0f;
				else if (enemies_list.at(enemies_list.size() - 4).get_position().x <= 162.0f) 
					x = 6.0f;

				enemies_list.at(enemies_list.size() - 4).change_position(new_position(x, 0.0f));

				if (enemies_list.at(enemies_list.size() - 3).get_position().x >= 432.0f)
					x = -6.0f;
				else if (enemies_list.at(enemies_list.size() - 3).get_position().x <= 189.0f)
					x = 6.0f;

				enemies_list.at(enemies_list.size() - 3).change_position(new_position(x, 0.0f));


				if (enemies_list.at(enemies_list.size() - 2).get_position().x >= 783.0f)
					x = -6.0f;
				else if (enemies_list.at(enemies_list.size() - 2).get_position().x <= 540.0f)
					x = 6.0f;

				enemies_list.at(enemies_list.size() - 2).change_position(new_position(x, 0.0f));

				if (enemies_list.at(enemies_list.size() - 1).get_position().x >= 810.0f)
					x = -6.0f;
				else if (enemies_list.at(enemies_list.size() - 1).get_position().x <= 567.0f)
					x = 6.0f;

				enemies_list.at(enemies_list.size() - 1).change_position(new_position(x, 0.0f));

				if (enemies_list.at(enemies_list.size() - 5).get_position().x >= 540.0f
				 && enemies_list.at(enemies_list.size() - 5).get_position().y == 189.0f)
					enemies_list.at(enemies_list.size() - 5).change_position(new_position(-6.0f, 0.0f));

				if (enemies_list.at(enemies_list.size() - 5).get_position().x <= 540.0f
				 && enemies_list.at(enemies_list.size() - 5).get_position().y <= 297.0f-6.0f)
					enemies_list.at(enemies_list.size() - 5).change_position(new_position(0.0f, 6.0f));

				if (enemies_list.at(enemies_list.size() - 5).get_position().x <= 675.0f
				 && enemies_list.at(enemies_list.size() - 5).get_position().y >= 297.0f)
					enemies_list.at(enemies_list.size() - 5).change_position(new_position(6.0f, 0.0f));

				if (enemies_list.at(enemies_list.size() - 5).get_position().x >= 675.0f
				 && enemies_list.at(enemies_list.size() - 5).get_position().y >= 189.0f)
					enemies_list.at(enemies_list.size() - 5).change_position(new_position(0.0f, -6.0f));



				if (enemies_list.at(11).get_position().x >= 162.0f
					&& enemies_list.at(11).get_position().y == 189.0f)
					enemies_list.at(11).change_position(new_position(-6.0f, 0.0f));

				if (enemies_list.at(11).get_position().x <= 162.0f
					&& enemies_list.at(11).get_position().y <= 297.0f - 6.0f)
					enemies_list.at(11).change_position(new_position(0.0f, 6.0f));

				if (enemies_list.at(11).get_position().x <= 297.0f
					&& enemies_list.at(11).get_position().y >= 297.0f)
					enemies_list.at(11).change_position(new_position(6.0f, 0.0f));

				if (enemies_list.at(11).get_position().x >= 297.0f
					&& enemies_list.at(11).get_position().y >= 189.0f)
					enemies_list.at(11).change_position(new_position(0.0f, -6.0f));



				if (check_if_obj_collides_with_obj(p1, Next_Level, buffer, get_size())) {

					std::cout << "Changing level\n";
					amount_x = 0.0f;
					amount_y = 0.0f;
					Next_Level.despawn();
					std::string level3 = "res/levels/test4.txt";
					Load_Next_Level(level3, 945.0f, 540.0f, 27.0f);

				}
			}
			else if (current_level == 4) {

				if (enemies_list.at(enemies_list.size() - 11).get_position().x >= 891.0f)
				{
					x = -10.5f;
				}
				else if (enemies_list.at(enemies_list.size() - 11).get_position().x <= 27.0f)
				{
					x = 10.5f;
				}

				enemies_list.at(enemies_list.size() - 11).change_position(new_position(x, 0.0f));



				double current_time2 = glfwGetTime();
				frame_count2++;
				/* If a second has passed. */
				if (current_time2 - previous_time2 >= 0.8)
				{
					// Display the frame count here any way you want.
					
					if (despawn) {
						//std::cout << "Despawing...\n";
						for (int i = 49; i < 64; i++) {
							pos.push_back(enemies_list.at(i).get_position());
							enemies_list.at(i).despawn();
						}
						despawn = false;

					}
					else {
						//std::cout << "Respawing...\n";
						int j = 0;
						for (int i = 49; i < 64; i++) {
							enemies_list.at(i).respawn(pos.at(j));
							j++;
						}
						pos.clear();
						despawn = true;
					}

					frame_count2 = 0;
					previous_time2 = current_time2;
				}

				float new_time2 = glfwGetTime();
				float frame_time2 = new_time2 - current_time2;
				current_time2 = new_time2;

				if (enemies_list.at(48).get_position().x >= 891.0f)
				{
					x = -10.0f;
				}
				else if (enemies_list.at(48).get_position().x <= 27.0f)
				{
					x = 10.0f;
				}

				enemies_list.at(48).change_position(new_position(x, 0.0f));

				if (check_if_obj_collides_with_obj(p1, Next_Level, buffer, get_size())) {

					std::cout << "Congratulations!!! You won!\n";
					Next_Level.despawn();
					Game_Over(9.0f);

				}
			}

		}

	}
};
