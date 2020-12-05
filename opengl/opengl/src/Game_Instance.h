#pragma once
#include "Game.h"
#include "Collision_Detection.h"
#include "Input.h"
#include "Line.h"

class Game_Instance : public Game {

	/* This is so the class Game_Instance also inherits the constructor of class Game */
	using Game::Game;

public:
	float amount_x = 0.0f, amount_y = 0.0f, amount_xx = 0.0f, amount_yy = 0.0f, scale_h = 0.0f, scale_v = 0.0f;
	float x = 0.0f, y = 0.0f, y1 = 0.0f, acc = 0.0f;
	int hor = -1;

	double previous_time1 = glfwGetTime();
	int frame_count1 = 0;
	float current_time1 = 0.0f;

	bool despawn = true, rs = false, rw = false, ls = false, lw = false;
	int uses = 0, damage = 0, score = 0;
	glm::vec2 save_next_lvl_pos;

	std::vector<glm::vec2> pos;
	std::string current_level_played;
	Line line;

	void init() override {

		texture_manager.Load_Texture(renderer, "res/textures/factory_tile.png");
		texture_manager.Load_Texture(renderer, "res/textures/main_menu.png");
		texture_manager.Load_Texture(renderer, "res/textures/collectible.png");
		texture_manager.Load_Texture(renderer, "res/textures/enemy.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_r.png");
		texture_manager.Load_Texture(renderer, "res/textures/factory_bg_6.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_l.png");
		texture_manager.Load_Texture(renderer, "res/textures/next_level_button.png");
		texture_manager.Load_Texture(renderer, "res/textures/win_congrats_screen.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_rw.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_lw.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_rs.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_ls.png");
		texture_manager.Load_Texture(renderer, "res/textures/yellow-brick-wall_1.jpg");

		Load_Menu(945.0f, 540.0f, texture_manager.Find("res/textures/main_menu.png"));
		

		p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));

		SoundEngine->play2D("res/audio/breakout.mp3", true);
		//m_Camera.Set_Position({ p1.get_position().x - 945.0f / 2.0f, p1.get_position().y - p1.get_scale(), 0.0f });
		line = Line(buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x , buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y, buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + 100.0f, buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y);

		
	}

	void update() override {

		if (current_level == 1) {
			//std::cout << "Level1\n";
			m_Camera.Set_Position({ 0.0f, 0.0f, 0.0f });
			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
			else if (input_manager.GetKeyPress(get_window(), GLFW_KEY_SPACE)) {
				Load_Menu(945.0f, 540.0f, texture_manager.Find("res/textures/win_congrats_screen.png"));
			}
		} else if (current_level == 2) {
			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
			else if (input_manager.GetKeyPress(get_window(), GLFW_KEY_SPACE)) {
				Load_Menu(945.0f, 540.0f, texture_manager.Find("res/textures/main_menu.png"));
			}
		} else if (current_level == 3) {
			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
			else if (input_manager.GetKeyPress(get_window(), GLFW_KEY_SPACE)) {
				Load_Menu(945.0f, 540.0f, texture_manager.Find("res/textures/win_congrats_screen.png"));
			}
		} else if (current_level == 4) {
			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
			else if (input_manager.GetKeyPress(get_window(), GLFW_KEY_SPACE)) {
				Load_Menu(945.0f, 540.0f, texture_manager.Find("res/textures/main_menu.png"));
			}
		} else if (current_level == 5) {
			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
			else if (input_manager.GetKeyPress(get_window(), GLFW_KEY_SPACE)) {
				current_level_played = "res/levels/test1.txt";

				Player ray = Player(
					0, 
					glm::vec4(1.0f, 0.5f, 1.0f, 1.0f),
					{ 100.0f, 100.0f },
					{ 100.0f + 50.0f, 100.0f },
					{ 100.0f + 50.0f, 100.0f - 10.0f },
					{ 100.0f, 100.0f - 10.0f }
				);
				
				Player ray_uses = Player(
					0,
					glm::vec4(1.0f, 0.5f, 1.0f, 1.0f),
					{ 0.0f, 540.0f },
					{ 135.0f, 540.0f },
					{ 135.0f, 513.0f },
					{ 0.0f, 513.0f }
				);

				Player health = Player(
					0, 
					glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
					{ 135.0f, 540.0f },
					{ 945.0f, 540.0f },
					{ 945.0f, 513.0f },
					{ 135.0f, 513.0f }
				);

				custom_sprite_list.push_back(ray);
				custom_sprite_list.push_back(ray_uses);
				custom_sprite_list.push_back(health);
				Load_Next_Level(current_level_played, 945.0f, 540.0f, 27.0f);
				m_Camera.Set_Position({ p1.get_position().x - (945.0f / 2.0f), 0.0f, 0.0f });

				p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));
				save_next_lvl_pos = Next_Level.get_position();
				Next_Level.despawn();
				score = 0;
				
			}
		}
		else
		{

			if (Is_Grounded_y) {
				if (scale_h < 0) {
					if (amount_x > 0) amount_x -= 0.5f;
					else amount_x = 0;
				} else {
					if (amount_x < 0) amount_x += 0.5f;
					else amount_x = 0;
				}
			}
			else {
				if (scale_h < 0) {
					if (amount_x > 0) amount_x -= 0.5f;
					else amount_x = 0;
				}
				else {
					if (amount_x < 0) amount_x += 0.5f;
					else amount_x = 0;
				}
			}

			/* Check for input on x-axis */
			hor = handle_input_hor(get_window(), &p1, 5.0f, acc, p1.get_scale(), &amount_x, buffer.Get_Size(), &scale_h, Is_Grounded_y, refresh_rate);

			/* Check for input on y-axis */
			handle_input_vert(get_window(), &p1, p1.get_scale(), &amount_y, buffer.Get_Size(), &scale_v, Is_Grounded_y, Collides_y, refresh_rate);

			if (hor == 0) acc = acc + 0.1f;
			else acc = 0.0f;

			/* TODO: find out why "scale_v" it doesnt work by reference */
			if (amount_y > 0) scale_v = -p1.get_scale();
			else scale_v = p1.get_scale();

			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_SPACE) && uses != 5) {

				uses++;
				custom_sprite_list.at(1).custom_position_1.x -= 27.0f;
				custom_sprite_list.at(1).custom_position_2.x -= 27.0f;

				if (scale_h > 0) {
					line = Line(buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x, buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y, buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x - 200.0f, buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y);

					custom_sprite_list.at(0).set_custom_position(buffer.Get_Buffer()[p1.get_buffer_index()[0]].position, -200.0f, -5.0f, -10.0f);
				}
				else {
					line = Line(buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x, buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y, buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + 200.0f, buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y);

					custom_sprite_list.at(0).set_custom_position(buffer.Get_Buffer()[p1.get_buffer_index()[0]].position, 200.0f, -5.0f, 35.0f);
				}
				for (int i = 0; i < enemies_list.size(); i++) {
					if (line.Intersects(enemies_list.at(i), buffer)) {
						SoundEngine->play2D("res/audio/bleep.mp3");
						enemies_list.at(i).despawn();
					}
				}
				
			} else {
				custom_sprite_list.at(0).despawn_custom_sprite();
			}

			//if (scale_h > 0) p1.set_texture_id(texture_manager.Find("res/textures/robot_reversed.png"));
			//else p1.set_texture_id(texture_manager.Find("res/textures/robot.png"));

			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_A)) p1.set_texture_id(texture_manager.Find("res/textures/player_l.png"));

			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_D)) p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));

			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_R)) {
				std::cout << "Restarting level\n";
				amount_x = 0.0f;
				amount_y = 0.0f;
				Next_Level.despawn();
				Load_Next_Level(current_level_played, 945.0f, 540.0f, 27.0f);
				current_level--;
				p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));
				m_Camera.Set_Position({ p1.get_position().x - (945.0f / 2.0f), 0.0f, 0.0f });
				amount_x = 0.0f;
				amount_y = 0.0f;
				save_next_lvl_pos = Next_Level.get_position();
				Next_Level.despawn();
				score = 0;
				uses = 0;
				damage = 0;
			}

			if (score == 3) {
				Next_Level.fix_position(save_next_lvl_pos);
				score = 0;
			}
			
			

			if (amount_x > 5.0f) amount_x = 5.0f;
			if (amount_x < -5.0f) amount_x = -5.0f;


			m_Camera.Set_Position_x(buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + (-945.0f / 2.0f));
			Background.fix_position(glm::vec2({ buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + (-945.0f / 2.0f) , 0.0f }));
			custom_sprite_list.at(1).set_custom_position(glm::vec2({ buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + (-945.0f / 2.0f) , 513.0f }), 135.0f, 27.0f, 0.0f, -uses*27.0f);
			custom_sprite_list.at(2).set_custom_position(glm::vec2({ buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + (-945.0f / 2.0f) + 135.0f , 513.0f }), 823.0f, 27.0f, 0.0f, -damage*10.0f);

			if (damage * 10 > 823.0f) {
				std::cout << "Game Over!\n";
				damage = 0;
				uses = 0;
				amount_x = 0.0f;
				amount_y = 0.0f;
				Game_Over(texture_manager.Find("res/textures/main_menu.png"));
			}

			for (int i = 0; i < enemies_list.size(); i++) {

				if (check_if_obj_collides_with_obj(p1, enemies_list.at(i), buffer.Get_Buffer(), buffer.Get_Size())) {
					damage++;
				}

			}

			for (int i = 0; i < collectible_list.size(); i++) {

				if (check_if_obj_collides_with_obj(p1, collectible_list.at(i), buffer.Get_Buffer(), buffer.Get_Size())) {
					collectible_list.at(i).despawn();
					score++;
					
				}

			}

			for (int i = 0; i < collectible_list1.size(); i++) {

				if (check_if_obj_collides_with_obj(p1, collectible_list1.at(i), buffer.Get_Buffer(), buffer.Get_Size())) {
					collectible_list1.at(i).despawn();
					if(uses > 0) uses--;

				}

			}

			if (input_manager.GetKeyDown(get_window(), GLFW_KEY_W, GLFW_PRESS)) {
				if (scale_h > 0) p1.set_texture_id(texture_manager.Find("res/textures/player_lw.png"));
				else p1.set_texture_id(texture_manager.Find("res/textures/player_rw.png"));
			}
			Repeat_Every(0.3, previous_time1, 
				[&](void) {
					if (input_manager.GetKeyDown(get_window(), GLFW_KEY_A, GLFW_PRESS)) {
						if (p1.get_texture_id() == texture_manager.Find("res/textures/player_l.png")) {
							p1.set_texture_id(texture_manager.Find("res/textures/player_ls.png"));
							ls = false; lw = true;
						} else if (p1.get_texture_id() == texture_manager.Find("res/textures/player_ls.png")) {
							if (lw) p1.set_texture_id(texture_manager.Find("res/textures/player_lw.png"));
							if (ls) p1.set_texture_id(texture_manager.Find("res/textures/player_l.png"));
						} else if (p1.get_texture_id() == texture_manager.Find("res/textures/player_lw.png")) {
							p1.set_texture_id(texture_manager.Find("res/textures/player_ls.png"));
							ls = true; lw = false;
						}
					}
					if (input_manager.GetKeyDown(get_window(), GLFW_KEY_D, GLFW_PRESS)) {
						if (p1.get_texture_id() == texture_manager.Find("res/textures/player_r.png")) {
							p1.set_texture_id(texture_manager.Find("res/textures/player_rs.png"));
							rs = false; rw = true;
						} else if (p1.get_texture_id() == texture_manager.Find("res/textures/player_rs.png")) {
							if (rw) p1.set_texture_id(texture_manager.Find("res/textures/player_rw.png"));
							if (rs) p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));
						} else if (p1.get_texture_id() == texture_manager.Find("res/textures/player_rw.png")) {
							p1.set_texture_id(texture_manager.Find("res/textures/player_rs.png"));
							rs = true; rw = false;
						}
					}
				});

			//std::cout << "Camera pos: " << m_Camera.Get_Position().x << ", " << m_Camera.Get_Position().y << "\n";
			if (current_level == 6) {
				//level 1 of actual game
				if (check_if_obj_collides_with_obj(p1, Next_Level, buffer.Get_Buffer(), buffer.Get_Size())) {

					std::cout << "Changing level\n";
					amount_x = 0.0f;
					amount_y = 0.0f;
					Next_Level.despawn();
					current_level_played = "res/levels/test2.txt";
					Load_Next_Level(current_level_played, 945.0f, 540.0f, 27.0f);
					p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));
					m_Camera.Set_Position({ p1.get_position().x - (945.0f / 2.0f), 0.0f, 0.0f });
					amount_x = 0.0f;
					amount_y = 0.0f;
					save_next_lvl_pos = Next_Level.get_position();
					Next_Level.despawn();
					score = 0;
					uses = 0;
					damage = 0;
				}
			} else if (current_level == 7) {
				//level 2 of actual game
				if (check_if_obj_collides_with_obj(p1, Next_Level, buffer.Get_Buffer(), buffer.Get_Size())) {

					std::cout << "Changing level\n";
					amount_x = 0.0f;
					amount_y = 0.0f;
					Next_Level.despawn();
					current_level_played = "res/levels/test3.txt";
					Load_Next_Level(current_level_played, 945.0f, 540.0f, 27.0f);
					p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));
					m_Camera.Set_Position({ p1.get_position().x - (945.0f / 2.0f), 0.0f, 0.0f });
					amount_x = 0.0f;
					amount_y = 0.0f;
					save_next_lvl_pos = Next_Level.get_position();
					Next_Level.despawn();
					score = 0;
					uses = 0;
					damage = 0;
				}
			} else if (current_level == 8) {
				//level 3 of actual game
			} else if (current_level == 9) {
				//level 4 of actual game
			} else if (current_level == 10) {
				//level 5 of actual game
			} else if (current_level == 11) {
				//end screen of actual game
			} else if (current_level == 12) {
				//outro of tvga
			} else if (current_level == 13) {
				//credit screen kai game ends
			}
		}
	}
};
