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

		std::string level1 = "res/levels/test1.txt";

		Load_Next_Level(level1, 945.0f, 540.0f, 27.0f);
		convert_quads_to_polygons(0, 0, 35, 20, 27.0f, 35);
		CalculateVisibilityPolygon(p1.get_position().x + (p1.get_scale()/2), p1.get_position().y + (p1.get_scale() / 2), 1500.0f);
	
		Init_Shadows();
		//Init_Shadow_points();

		p1.set_texture_id(5);
		//m_Camera.Set_Position({ p1.get_position().x - 945.0f / 2.0f, p1.get_position().y - 540.0f / 2.0f, 0.0f });
		
	}

	void update() override {
		//std::cout << p1.get_position().x << "..." << p1.get_position().y << "\n";
		amount_x = 0.0;
		amount_y = 0.0;

		/* Check for input on x-axis */
		handle_input_hor(get_window(), &p1, 26.0f, &amount_x, get_size(), &scale_h, Is_Grounded_y, refresh_rate);

		/* Check for input on y-axis */
		handle_input_vert_no_gravity(get_window(), &p1, 26.0f, &amount_y, get_size(), &scale_v, Is_Grounded_y, refresh_rate);


		/* TODO: find out why "scale_v" it doesnt work by reference */
		if (amount_y > 0) scale_v = -26.0f;
		else scale_v = 26.0f;

		if (scale_h > 0) p1.set_texture_id(7);
		else  p1.set_texture_id(5);

		CalculateVisibilityPolygon(p1.get_position().x + (p1.get_scale() / 2), p1.get_position().y + (p1.get_scale() / 2), 1500.0f);
		Calculate_Shadows1();
		//Calculate_Shadow_points();

		//m_Camera.Set_Position_y(p1.get_position().y - (540.0f/2.0f));
		//m_Camera.Incr_Position({ 0.0f, amount_y, 0.0f });
		//m_Camera.Incr_Position({ amount_x, 0.0f, 0.0f });

		//if (m_Camera.Get_Position().x < (-945.0f / 2.0f)) m_Camera.Set_Position_x(-945.0f / 2.0f);
		//if (m_Camera.Get_Position().x > (945.0f / 2.0f)) m_Camera.Set_Position_x(945.0f / 2.0f);

		//std::cout << "Camera pos: " << m_Camera.Get_Position().x << ", " << m_Camera.Get_Position().y << "\n";

	}
};
