#pragma once
#include "Game.h"
#include "Collision_Detection.h"
#include "Input.h"

class Game_Instance : public Game {

	/* This is so the class Game_Instance also inherits the constructor of class Game */
	using Game::Game;

public:
	float amount_x = 0.0f, amount_y = 0.0f, amount_xx = 0.0f, amount_yy = 0.0f, scale_h = 0.0f, scale_v = 0.0f;
	float x = 0.0f, y = 0.0f, y1 = 0.0f, acc = 0.0f;
	int hor = -1;

	float xx = 0.0f, yy = 0.0f, angle = 0.0f, angle1 = 180.0f;;

	double previous_time = glfwGetTime();
	int frame_count = 0;
	float current_time = 0.0f, dt = 0.0f;

	double previous_time1 = glfwGetTime();
	int frame_count1 = 0;
	float current_time1 = 0.0f;

	double previous_time2 = glfwGetTime();
	int frame_count2 = 0;
	float current_time2 = 0.0f;

	bool despawn = true;

	std::vector<glm::vec2> pos;

	void init() override {


		texture_manager.Load_Texture(renderer, "res/textures/factory_tile.png");
		texture_manager.Load_Texture(renderer, "res/textures/main_menu.png");
		texture_manager.Load_Texture(renderer, "res/textures/collectible.png");
		texture_manager.Load_Texture(renderer, "res/textures/enemy.png");
		texture_manager.Load_Texture(renderer, "res/textures/robot.png");
		texture_manager.Load_Texture(renderer, "res/textures/factory_bg_6.png");
		texture_manager.Load_Texture(renderer, "res/textures/robot_reversed.png");
		texture_manager.Load_Texture(renderer, "res/textures/next_level_button.png");
		texture_manager.Load_Texture(renderer, "res/textures/win_congrats_screen.png");

		Load_Menu(945.0f, 540.0f, texture_manager.Find("res/textures/main_menu.png"));
		

		p1.set_texture_id(texture_manager.Find("res/textures/robot.png"));

		SoundEngine->play2D("res/audio/breakout.mp3", true);
		//m_Camera.Set_Position({ p1.get_position().x - 945.0f / 2.0f, p1.get_position().y - p1.get_scale(), 0.0f });
		
	}

	void update() override {

		if (current_level == 0) {
			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(get_window(), GLFW_TRUE);
			else if (input_manager.GetKeyPress(get_window(), GLFW_KEY_SPACE)) {
				std::string level1 = "res/levels/test1.txt";
				Load_Next_Level(level1, 945.0f, 540.0f, 27.0f);
				m_Camera.Set_Position({ p1.get_position().x - 945.0f / 2.0f, p1.get_position().y - p1.get_scale(), 0.0f });
			}
		}
		else
		{
			if (input_manager.GetKeyPress(get_window(), GLFW_KEY_E)) {
				std::cout << "Pressed" << GLFW_KEY_E << std::endl;
				SoundEngine->play2D("res/audio/bleep.mp3");
			}

			if (Is_Grounded_y) {
				if (scale_h < 0) {
					if (amount_x > 0) amount_x -= 0.5f;
					else amount_x = 0;
				} else {
					if (amount_x < 0) amount_x += 0.5f;
					else amount_x = 0;
				}
			}

			/* Check for input on x-axis */
			hor = handle_input_hor(get_window(), &p1, 5.0f, acc, 26.0f, &amount_x, buffer.Get_Size(), &scale_h, Is_Grounded_y, refresh_rate);

			/* Check for input on y-axis */
			handle_input_vert(get_window(), &p1, 26.0f, &amount_y, buffer.Get_Size(), &scale_v, Is_Grounded_y, Collides_y, refresh_rate);

			if (hor == 0) acc = acc + 0.1f;
			else acc = 0.0f;

			/* TODO: find out why "scale_v" it doesnt work by reference */
			if (amount_y > 0) scale_v = -26.0f;
			else scale_v = 26.0f;

			if (scale_h > 0) p1.set_texture_id(texture_manager.Find("res/textures/robot_reversed.png"));
			else  p1.set_texture_id(texture_manager.Find("res/textures/robot.png"));

			if (amount_x > 5.0f) amount_x = 5.0f;
			if (amount_x < -5.0f) amount_x = -5.0f;

			//m_Camera.Set_Position_y(p1.get_position().y - (540.0f/2.0f));
			//m_Camera.Incr_Position({ 0.0f, amount_y, 0.0f });
			//m_Camera.Incr_Position({ amount_x, 0.0f, 0.0f });
			m_Camera.Set_Position_x(buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + (-945.0f / 2.0f));
			Background.fix_position(glm::vec2({ buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x + (-945.0f / 2.0f) , 0.0f }));

			//if (m_Camera.Get_Position().x < (-945.0f / 2.0f)) m_Camera.Set_Position_x(-945.0f / 2.0f);
			//if (m_Camera.Get_Position().x > (945.0f / 2.0f)) m_Camera.Set_Position_x(945.0f / 2.0f);

			//std::cout << "Camera pos: " << m_Camera.Get_Position().x << ", " << m_Camera.Get_Position().y << "\n";
			if (check_if_obj_collides_with_obj(p1, Next_Level, buffer.Get_Buffer(), buffer.Get_Size())) {

				std::cout << "Changing level\n";
				amount_x = 0.0f;
				amount_y = 0.0f;
				Next_Level.despawn();
				std::string level3 = "res/levels/test3.txt";
				Load_Next_Level(level3, 945.0f, 540.0f, 27.0f);
				m_Camera.Set_Position({ p1.get_position().x - 945.0f / 2.0f, p1.get_position().y - p1.get_scale(), 0.0f });
			}
		}
	}
};
