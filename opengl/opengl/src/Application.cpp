#include "Shader.h"
#include "Collision_Detection.h"
#include "Input.h"
#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <stdio.h>
#include "Game.h"
#include "Game_Instance.h"

int main(void)
{

	/* Initialize the library */
	if (!glfwInit())
		return -1;
#if 0
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window;
	window = glfwCreateWindow(945, 540, "Hello World", NULL, NULL);
	int refresh_rate = 144;
#else
	
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	int refresh_rate = mode->refreshRate;
	std::cout << mode->refreshRate << "\n";

	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My Title", glfwGetPrimaryMonitor(), NULL); 

	
#endif	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error initializing GLEW\n";
	}
	int size = 0;
	std::string level = "res/levels/test1.txt";

	Game_Instance game = Game_Instance(level, window, 945.0f, 540.0f, 27.0f, refresh_rate);

	game.init();

	static double limitFPS = 1.0 / 60.0;

	double lastTime = glfwGetTime();
	double timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// - Measure time
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;

		// - Only update at 60 frames / s
		while (deltaTime >= 1.0) {
			game.update();   // - Update function
			game.handle_collision(game.scale_h, game.scale_v, game.amount_x, game.amount_y, 0);
			updates++;
			deltaTime--;
		}
		
		// - Render at maximum possible frames
		game.render();
		frames++;

		// - Reset after one second
		if (glfwGetTime() - timer > 1.0) {
			timer++;
			std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
			updates = 0, frames = 0;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
