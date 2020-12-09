#include "Renderer2D.h"

void Renderer2D::Initialize(Vertex_Buffer buffer, Index_Buffer index_buffer, Orthographic_Camera m_Camera)
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	/* vertex array object */
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	/* vertex buffer */
	GLCall(glGenBuffers(1, &vb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, buffer.Get_Total_Size() * sizeof(struct Vertex_Array), nullptr, GL_DYNAMIC_DRAW));

	/* vertex attrib positions*/
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, position)));

	/* vertex attrib colors*/
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, color)));

	/* vertex attrib texture coordinates*/
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, tex_coord)));

	/* vertex attrib texture id*/
	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, tex_id)));

	/* index buffer */
	GLCall(glGenBuffers(1, &ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (buffer.Get_Size() / 4) * 6 * sizeof(unsigned int), index_buffer.Get_Index_Buffer(), GL_STATIC_DRAW));

	/* shaders */
	ShaderProgramSource shaderSource = ParseShader("res/shaders/Basic.shader");
	shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(auto loc = glGetUniformLocation(shader, "u_textures"));
	int samplers[32];
	for (unsigned int i = 0; i < 32; i++)
		samplers[i] = i;
	GLCall(glUniform1iv(loc, 32, samplers));

	//glm::vec3 translationA(0, 0, 0);
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	//glm::mat4 projection = glm::ortho(0.0f, 945.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	//glm::mat4 mvp = projection * view * model;

	glm::mat4 vp = m_Camera.Get_View_Projection_Matrix();

	GLCall(int location1 = glGetUniformLocation(shader, "u_VP"));
	if (location1 == -1) {
		std::cout << "Uniform not found!!!\n";
	}
	//GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, &mvp[0][0]));
	GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(vp)));
}

void Renderer2D::Render(Vertex_Buffer buffer, Orthographic_Camera m_Camera, unsigned int current_index)
{
	Update_Camera_Uniform(m_Camera);

	/* set dynamic vertex buffer */
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.Get_Size() * sizeof(Vertex_Array), buffer.Get_Buffer()));

	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT);

	unsigned int i = 0;
	for (i = 0; i < current_index; i++) {
		GLCall(glBindTextureUnit(i, texture_slot[i]));
	}
	
	
	GLCall(glDrawElements(GL_TRIANGLES, (buffer.Get_Size() / 4) * 6, GL_UNSIGNED_INT, NULL));
}

void Renderer2D::Clear()
{
	GLCall(glDeleteBuffers(1, &vb));
	GLCall(glDeleteBuffers(1, &ib));
	GLCall(glDeleteProgram(shader));
	GLCall(glDeleteVertexArrays(1, &vao));

	GLCall(glDeleteTextures(32, texture_slot));
}

void Renderer2D::Update_Camera_Uniform(Orthographic_Camera m_Camera)
{
	glm::mat4 vp = m_Camera.Get_View_Projection_Matrix();
	GLCall(int location1 = glGetUniformLocation(shader, "u_VP"));
	if (location1 == -1) {
		std::cout << "Uniform not found!!!\n";
	}
	GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(vp)));
}

void Renderer2D::Upadte_Index_Buffer(unsigned int size, Index_Buffer index_buffer)
{
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size / 4) * 6 * sizeof(unsigned int), index_buffer.Get_Index_Buffer(), GL_STATIC_DRAW));
}

unsigned int *Renderer2D::Get_Texture_Slot()
{
	return texture_slot;
}
