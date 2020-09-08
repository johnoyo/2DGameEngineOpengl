#include "Orthographic_Camera.h"

#include "glm/gtc/matrix_transform.hpp"

Orthographic_Camera::Orthographic_Camera(float left, float right, float bottom, float top)
	: m_Projection_Matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_View_Matrix(1.0f)
{
	m_View_Projection_Matrix = m_Projection_Matrix * m_View_Matrix;
}

void Orthographic_Camera::Recalculate_View_Matrix()
{
	glm::mat4 tranform = glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

	m_View_Matrix = glm::inverse(tranform);
	m_View_Projection_Matrix = m_Projection_Matrix * m_View_Matrix;
}
