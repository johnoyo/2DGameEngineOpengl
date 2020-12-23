#pragma once
#include "Renderer.h"

class Window {
public:
	Window(float w, float h, const std::string& name, bool fullscreen);
	Window(const std::string& name);

	void Make_Context_Current(int Vsync);
	int Window_Should_Close();
	void Swap_Buffers();
	void Poll_For_Events();
	void Terminate();

	GLFWwindow *Get_Window();
	float Get_Width();
	float Get_Height();

private:
	void Init();

	GLFWwindow * window;
	std::string title;
	float refresh_rate;
	float width;
	float height;
 };