#pragma once
#include "Renderer.h"

class Input_Manager {
public:
	/* Returns true i.e. 1, as long as the specified key is pressed */
	int GetKeyDown(GLFWwindow* window, int Key_Code, int Mode);
	/* Returns true i.e. 1, only the moment that the key is pressed */
	int GetKeyPress(GLFWwindow* window, int Key_Code);
	/* Returns true i.e. 1, only the moment that the key is released */
	int GetKeyRelease(GLFWwindow* window, int Key_Code);
	int Check_State(GLFWwindow* window, int Key_Code);
private:
	int last_state_p = GLFW_RELEASE;
	int last_state_r = GLFW_PRESS;
};
