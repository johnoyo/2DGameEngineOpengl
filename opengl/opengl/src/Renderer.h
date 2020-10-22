#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

#define X_AXIS 1
#define Y_AXIS 2
#define PI 3.14159265f

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3


#define DEBUG

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
#define GLCall(x) GLClearErrors();x;ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif // DEBUG

struct Vertex_Array {
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 tex_coord;
	float tex_id;
};

struct sEdge
{
	float sx, sy; // Start coordinate
	float ex, ey; // End coordinate
	float theta;
};

struct sCell
{
	int edge_id[4];
	bool edge_exist[4];
	bool exist = false;
};

struct first_col { float x; float y; float xy; };

#define pdd pair<float, float>

static void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "):" << function << " " << file << ":" << line << "\n";
		return false;
	}
	return true;
}