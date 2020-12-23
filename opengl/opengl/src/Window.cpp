#include "Window.h"

Window::Window(float w, float h, const std::string& name, bool fullscreen) : width(w), height(h), title(name)
{
	Init();
	if (fullscreen) {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		refresh_rate = mode->refreshRate;
		std::cout << mode->refreshRate << "\n";

		window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	} else {
		window = glfwCreateWindow((int)w, (int)h, title.c_str(), NULL, NULL);
	}
	if (!window) {
		std::cout << "Error Creating window!!!\n";
		Terminate();
		exit(-1);
	}

}

Window::Window(const std::string& name) : title(name)
{
	Init();
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	refresh_rate = mode->refreshRate;
	std::cout << mode->refreshRate << "\n";

	width = mode->width;
	height = mode->height;

	window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);

	if (!window) {
		std::cout << "Error Creating window!!!\n";
		Terminate();
		exit(-1);
	}
}

void Window::Init()
{
	if (!glfwInit()) {
		std::cout << "Error Initializing window!!!\n";
		exit(-1);
	}
}

void Window::Make_Context_Current(int Vsync)
{
	glfwMakeContextCurrent(window);
	glfwSwapInterval(Vsync);
}

int Window::Window_Should_Close()
{
	return glfwWindowShouldClose(window);
}

void Window::Swap_Buffers()
{
	glfwSwapBuffers(window);
}

void Window::Poll_For_Events()
{
	glfwPollEvents();
}

void Window::Terminate()
{
	glfwTerminate();
}

GLFWwindow * Window::Get_Window()
{
	return window;
}

float Window::Get_Width()
{
	return width;
}

float Window::Get_Height()
{
	return height;
}
