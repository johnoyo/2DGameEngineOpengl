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
	float x = 0.0f, y = 0.0f, y1 = 0.0f, acc = 0.0f, acc1 = 0.0f;
	int hor = -1, ver = -1;

	bool start_slowdown_r = false, start_slowdown_l = false;

	double previous_time1 = glfwGetTime();
	int frame_count1 = 0;
	float current_time1 = 0.0f;

	bool despawn = true, rs = false, rw = false, ls = false, lw = false;
	int coins_collected = 0, uses = 0, damage = 0, score = 0;
	glm::vec2 save_next_lvl_pos;

	std::vector<glm::vec2> pos;
	std::string current_level_played;
	Line line;

	void init() override {

		texture_manager.Load_Texture(renderer, "res/textures/brick_3.png");
		texture_manager.Load_Texture(renderer, "res/textures/start_screen_1.png");
		texture_manager.Load_Texture(renderer, "res/textures/coinA.png");
		texture_manager.Load_Texture(renderer, "res/textures/enemy.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_r.png");
		texture_manager.Load_Texture(renderer, "res/textures/how_to_screen.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_l.png");
		texture_manager.Load_Texture(renderer, "res/textures/next_level_button.png");
		texture_manager.Load_Texture(renderer, "res/textures/start_screen_2.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_rw.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_lw.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_rs.png");
		texture_manager.Load_Texture(renderer, "res/textures/player_ls.png");
		texture_manager.Load_Texture(renderer, "res/textures/brickWall_3.jpg");
		texture_manager.Load_Texture(renderer, "res/textures/rayA.png");
		texture_manager.Load_Texture(renderer, "res/textures/enemy_1A.png");
		texture_manager.Load_Texture(renderer, "res/textures/start_screen_3.png");
		texture_manager.Load_Texture(renderer, "res/textures/start_screen_4.png");
		texture_manager.Load_Texture(renderer, "res/textures/start_screen_5.png");
		texture_manager.Load_Texture(renderer, "res/textures/main_menu_screen.png");
		texture_manager.Load_Texture(renderer, "res/textures/win_screen.png");
		texture_manager.Load_Texture(renderer, "res/textures/end_screen.png");

		std::string level1 = "res/levels/test1.txt";
		Load_Next_Level(level1, window.Get_Width(), window.Get_Height(), 27.0f);

		p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));

		shadows.convert_quads_to_polygons(0.0f, 0.0f, 35.0f, 20.0f, 27.0f, 35.0f);
		shadows.CalculateVisibilityPolygon(p1.get_position().x, p1.get_position().y, 1000.0f);
		shadows.Init_Shadows(buffer, index_buffer, renderer, shadow_quad_list);

		for (int i = 0; i < shadows.endingEdges.size(); i++) {
			std::cout << "sx,sy: " << shadows.endingEdges.at(i).sx << ", " << shadows.endingEdges.at(i).sy << "\n";
			std::cout << "ex,ey: " << shadows.endingEdges.at(i).ex << ", " << shadows.endingEdges.at(i).ey << "\n";
			std::cout << "Theta: " << shadows.endingEdges.at(i).theta << "\n";
			std::cout << "Is external: " << shadows.endingEdges.at(i).is_external << "\n\n\n";
		}

	}

	void update() override {

		

			/*if (input_manager.GetKeyRelease(get_window(), GLFW_KEY_D)) {
				std::cout << "D\n";
				start_slowdown_r = true;
			} 

			if(start_slowdown_r && amount_x > 0) amount_x -= 0.5f;
			else {
				amount_x = 0;
				start_slowdown_r = false;
			}

			if (input_manager.GetKeyRelease(get_window(), GLFW_KEY_A)) {
				std::cout << "A\n";
				start_slowdown_l = true;
			}
			if (start_slowdown_l && amount_x < 0) amount_x += 0.5f;
			else {
				amount_x = 0;
				start_slowdown_l = false;
			}*/
			if (scale_h < 0) {
				if (amount_x > 0) amount_x -= 0.5f;
				else amount_x = 0;
			}
			else {
				if (amount_x < 0) amount_x += 0.5f;
				else amount_x = 0;
			}
			if (scale_v < 0) {
				if (amount_y > 0) amount_y -= 0.5f;
				else amount_y = 0;
			}
			else {
				if (amount_y < 0) amount_y += 0.5f;
				else amount_y = 0;
			}
			//amount_x = 0;
			//amount_y = 0;

			/* Check for input on x-axis */

			hor = handle_input_hor(window.Get_Window(), &p1, 5.0f, acc, p1.get_scale_x(), &amount_x, buffer.Get_Size(), &scale_h, Is_Grounded_y, refresh_rate);

			if (hor == 0) acc = acc + 0.1f;
			else acc = 0.0f;

			if (amount_x > 5.0f) amount_x = 5.0f;
			if (amount_x < -5.0f) amount_x = -5.0f;

			//p1.change_position({ amount_x, 0.0f });

			/* Check for input on y-axis */
			ver = handle_input_vert_no_gravity(window.Get_Window(), &p1, 5.0f, acc1, p1.get_scale_y(), &amount_y, buffer.Get_Size(), &scale_v, true, refresh_rate);

			
			//handle_input_vert(window.Get_Window(), &p1, p1.get_scale_y(), &amount_y, buffer.Get_Size(), &scale_v, Is_Grounded_y, Collides_y, refresh_rate);

			if (ver == 0) acc1 = acc1 + 0.1f;
			else acc1 = 0.0f;

			if (amount_y > 5.0f) amount_y = 5.0f;
			if (amount_y < -5.0f) amount_y = -5.0f;

			p1.change_position({ amount_x, amount_y });

			/* TODO: find out why "scale_v" it doesnt work by reference */
			if (amount_y > 0) scale_v = -p1.get_scale_y();
			else scale_v = p1.get_scale_y();

			if (input_manager.GetKeyPress(window.Get_Window(), GLFW_KEY_A)) p1.set_texture_id(texture_manager.Find("res/textures/player_l.png"));

			if (input_manager.GetKeyPress(window.Get_Window(), GLFW_KEY_D)) p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));

			if (input_manager.GetKeyPress(window.Get_Window(), GLFW_KEY_R) || p1.get_position().y < -window.Get_Height()) {
				std::cout << "Restarting level\n";
				amount_x = 0.0f;
				amount_y = 0.0f;
				Next_Level.despawn();
				Load_Next_Level(current_level_played, window.Get_Width(), window.Get_Height(), 27.0f);
				current_level--;
				p1.set_texture_id(texture_manager.Find("res/textures/player_r.png"));
				m_Camera.Set_Position({ p1.get_position().x - (window.Get_Width() / 2.0f), 0.0f, 0.0f });
				amount_x = 0.0f;
				amount_y = 0.0f;
				save_next_lvl_pos = Next_Level.get_position();
				Next_Level.despawn();
				score = 0;
				uses = 0;
				damage = 0;
				x = 0.0f;
				y = 0.0f;
				y1 = 0.0f;
				coins_collected = 0;
			}
			//m_Camera.Set_Position_x(buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + (-window.Get_Width() / 2.0f));
			//m_Camera.Set_Position_x(p1.get_position().x + (-window.Get_Width() / 2.0f));
			//Background.fix_position(glm::vec2({ buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + (-window.Get_Width() / 2.0f) , 0.0f }));
			//Background.fix_position(glm::vec2(p1.get_position().x + (-window.Get_Width() / 2.0f), 0.0f));

			shadows.CalculateVisibilityPolygon(p1.get_position().x, p1.get_position().y, 1000.0f);
			//shadows.Calculate_Shadows0(buffer, shadow_quad_list);
			shadows.Calculate_Shadows1(buffer);
			
	}
};
