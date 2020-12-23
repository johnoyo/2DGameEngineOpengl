#pragma once
#include "Renderer.h"
#include "Shader.h"

class Vertex_Buffer {
public:
	
	void Initialize(int total_buffer_size);
	void New_Position(unsigned int indx, float width, float height);
	void New_Color(unsigned int indx, float r, float g, float b, float a);
	void New_Tex_Coord(unsigned int indx, float x, float y);
	void New_Tex_Id(unsigned int indx, float id);
	void Fill_Buffer(glm::vec2 position, glm::vec4 color, glm::vec2 tex_coords, float tex_id);
	void Set_Buffer(struct Vertex_Array* new_buffer);
	struct Vertex_Array* Get_Buffer();
	unsigned int Get_Size();
	void Set_Size(unsigned int size);
	unsigned int Get_Total_Size();
	void Reset();

	friend class Game;
	friend class Shadow_Casting;

private:
	struct Vertex_Array* buffer = NULL;
	unsigned int index = 0;
	unsigned int total_size = 0;

};