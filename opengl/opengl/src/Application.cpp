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
#include "Window.h"

int main(void)
{

//	/* Initialize the library */
//	if (!glfwInit())
//		return -1;
//#if 1
//	/* Create a windowed mode window and its OpenGL context */
//	GLFWwindow* window;
//	window = glfwCreateWindow(945, 540, "My VGA 2020", NULL, NULL);
//	int refresh_rate = 144;
//#else
//	
//	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//
//	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
//	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
//	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
//	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
//
//	int refresh_rate = mode->refreshRate;
//	std::cout << mode->refreshRate << "\n";
//
//	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My VGA 2020", glfwGetPrimaryMonitor(), NULL); 
//
//	
//#endif	
//
//	
//
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//	/* Make the window's context current */
//	glfwMakeContextCurrent(window);
//	glfwSwapInterval(1);

	Window window = Window(945.0f, 540.0f, "Game", false);
	//Window window = Window(945.0f, 540.0f, "Game", true);

	window.Make_Context_Current(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error initializing GLEW\n";
	}
	int size = 0;
	Game_Instance game = Game_Instance(window, 27.0f);

	game.init();

	static double limitFPS = 1.0 / 60.0;

	double lastTime = glfwGetTime();
	double timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	/* Loop until the user closes the window */
	while (!game.window.Window_Should_Close())
	{
		// - Measure time
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;

		// - Only update at 60 frames / s
		while (deltaTime >= 1.0) {
			game.update();   // - Update function
			game.update_buffer();
			//game.handle_collision(game.scale_h, game.scale_v, game.get_player().amount_h, game.get_player().amount_v, 0);
			game.handle_collision_new();
			updates++;
			deltaTime--;
		}
		
		// - Render at maximum possible frames
		game.renderer.Render(game.buffer, game.m_Camera, game.texture_manager.current_index);
		//game.render();
		frames++;

		// - Reset after one second
		if (glfwGetTime() - timer > 1.0) {
			timer++;
			std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
			updates = 0, frames = 0;
		}

		/* Swap front and back buffers */
		game.window.Swap_Buffers();
		//glfwSwapBuffers(window);

		/* Poll for and process events */
		game.window.Poll_For_Events();
		//glfwPollEvents();
	}

	game.clean();
	game.window.Terminate();
	//glfwTerminate();
	return 0;
}
