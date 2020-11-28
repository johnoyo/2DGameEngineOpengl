#include "Vertex_Buffer.h"

void Vertex_Buffer::Initialize(int total_buffer_size)
{
	total_size = total_buffer_size;
	buffer = (struct Vertex_Array*)malloc(total_buffer_size * sizeof(struct Vertex_Array));
}

void Vertex_Buffer::New_Position(unsigned int indx, float width, float height)
{
	buffer[indx].position = glm::vec2(width, height);
}

void Vertex_Buffer::New_Color(unsigned int indx, float r, float g, float b, float a)
{
	buffer[indx].color = glm::vec4(r, g, b, a);
}

void Vertex_Buffer::New_Tex_Coord(unsigned int indx, float x, float y)
{
	buffer[indx].tex_coord = glm::vec2(x, y);
}

void Vertex_Buffer::New_Tex_Id(unsigned int indx, float id)
{
	buffer[indx].tex_id = id;
}

void Vertex_Buffer::Fill_Buffer(glm::vec2 position, glm::vec4 color, glm::vec2 tex_coords, float tex_id)
{
	buffer[index].position = position;
	buffer[index].color = color;
	buffer[index].tex_coord = tex_coords;
	buffer[index].tex_id = tex_id;
	index++;
}

Vertex_Array* Vertex_Buffer::Get_Buffer()
{
	return buffer;
}

void Vertex_Buffer::Set_Buffer(Vertex_Array* new_buffer)
{
	buffer = new_buffer;
}

unsigned int Vertex_Buffer::Get_Size()
{
	return index;
}

void Vertex_Buffer::Set_Size(unsigned int size)
{
	index = size;
}

unsigned int Vertex_Buffer::Get_Total_Size()
{
	return total_size;
}

void Vertex_Buffer::Reset()
{
	index = 0;
}
