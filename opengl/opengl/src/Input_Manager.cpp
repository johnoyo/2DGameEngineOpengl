#include "Input_Manager.h"

int Input_Manager::GetKeyDown(GLFWwindow *window, int Key_Code, int Mode)
{
	int result;
	result = (glfwGetKey(window, Key_Code) == Mode);
	return result;
}

int Input_Manager::GetKeyPress(GLFWwindow* window, int Key_Code)
{
	int result = 0;
	if(Check_State(window, Key_Code) == GLFW_PRESS && last_state_p == GLFW_RELEASE) result = GetKeyDown(window, Key_Code, GLFW_PRESS);
	last_state_p = Check_State(window, Key_Code);
	return result;
}

int Input_Manager::GetKeyRelease(GLFWwindow* window, int Key_Code)
{
	int result = 0;
	if (Check_State(window, Key_Code) == GLFW_RELEASE && last_state_r == GLFW_PRESS) result = GetKeyDown(window, Key_Code, GLFW_RELEASE);
	last_state_r = Check_State(window, Key_Code);
	return result;
}

int Input_Manager::Check_State(GLFWwindow* window, int Key_Code)
{
	return glfwGetKey(window, Key_Code);
}
