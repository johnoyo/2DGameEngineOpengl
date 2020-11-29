#pragma once
#include<string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#include "stb_image/stb_image.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

/* Reads the shader from the filepath and parses it into fragment and vertex, which are strings
 * @filepath: the path to the shader file
 */
ShaderProgramSource ParseShader(const std::string& filepath);
/* Compiles the shader and returns its id
 * @type: the type of the shader, either fragment or vertex
 * @source: the string which contains the shader code
 */
unsigned int CompileShader(unsigned int type, const std::string& source);
/* Creates the shader and returns the programs id
 * @vertexShader: the string which contains the vertex shader code
 * @fragmentShader: the string which contains the fragment shader code
 */
unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
