#include "Input.h"
#include "Player.h"

int handle_input_hor(GLFWwindow* window, Player *p, float player_scale, float* amount_x, float size, float* scale, bool is_grounded, int refresh_rate) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		/* std::cout << "Key D pressed\n"; */
		*scale = -player_scale;
		if (refresh_rate == 60) {
			if (is_grounded) *amount_x = (player_scale / 8)*2.4;
			else *amount_x = (player_scale / 10)*2.4;
		}
		else if(refresh_rate == 144)
		{
			if (is_grounded) *amount_x = (player_scale / 8);
			else *amount_x = (player_scale / 10);
		}
		(*p).change_position(glm::vec2(*amount_x, 0.0f));
		return 0;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		/* std::cout << "Key A pressed\n"; */
		*scale = player_scale;
		if (refresh_rate == 60) {
			if (is_grounded) *amount_x = -(player_scale / 8)*2.4;
			else *amount_x = -(player_scale / 10)*2.4;
		}
		else if (refresh_rate == 144)
		{
			if (is_grounded) *amount_x = -(player_scale / 8);
			else *amount_x = -(player_scale / 10);
		}
		(*p).change_position(glm::vec2(*amount_x, 0.0f));
		return 0;
	}

	return -1;
}

int handle_input_vert(GLFWwindow* window, Player *p, float player_scale, float* amount_y, float size, float* scale, bool is_grounded, bool collides, int refresh_rate) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && is_grounded) {
		/* std::cout << "Key W pressed\n"; */
		if (refresh_rate == 60) {
			*amount_y = 4.5+6.8;
		}
		else if (refresh_rate == 144)
		{
			*amount_y = 4.5;
		}
		(*p).change_position(glm::vec2(0.0f, *amount_y));
		*scale = -player_scale;
		return 0;
	}
	else {
		if (refresh_rate == 60) {
			if (!collides) *amount_y += -0.1*6.8;
			else  *amount_y = -1.0;
		}
		else if (refresh_rate == 144)
		{
			if (!collides) *amount_y += -0.1;
			else  *amount_y = -1.0;
		}
		(*p).change_position(glm::vec2(0.0f, *amount_y));
		*scale = player_scale;
		return 0;
	}

}
void displayFPS(int* frame_count, double* previous_time) {
	/* Measure speed */
	double current_time = glfwGetTime();
	(*frame_count)++;
	/* If a second has passed. */
	if (current_time - *previous_time >= 1.0)
	{
		/* Display the frame count here any way you want. */
		std::cout << "FPS:" << *frame_count << "\n";

		*frame_count = 0;
		*previous_time = current_time;
	}
}