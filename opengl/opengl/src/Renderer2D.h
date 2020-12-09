#pragma once
#include "Renderer.h"
#include "Index_Buffer.h"
#include "Vertex_Buffer.h"
#include "Orthographic_Camera.h"
#include "glm/gtc/type_ptr.hpp"

class Renderer2D {
public:
	void Initialize(Vertex_Buffer buffer, Index_Buffer index_buffer, Orthographic_Camera m_Camera);
	void Render(Vertex_Buffer buffer, Orthographic_Camera m_Camera, unsigned int current_index);
	void Clear();
	void Upadte_Index_Buffer(unsigned int size, Index_Buffer index_buffer);
	unsigned int *Get_Texture_Slot();

private:
	unsigned int vao;
	unsigned int vb;
	unsigned int ib;
	unsigned int shader;
	unsigned int white_texture_id;
	unsigned int texture_slot[32];

	void Update_Camera_Uniform(Orthographic_Camera m_Camera);
};
