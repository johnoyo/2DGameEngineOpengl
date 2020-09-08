#pragma once

#include "glm/glm.hpp"

class Orthographic_Camera {

public:
	Orthographic_Camera(float left, float right, float bottom, float top);

	void Set_Position(const glm::vec3& position) { m_Position = position; Recalculate_View_Matrix(); }
	void Set_Position_x(float position) { m_Position.x = position; Recalculate_View_Matrix(); }
	void Set_Position_y(float position) { m_Position.y = position; Recalculate_View_Matrix(); }
	void Incr_Position(const glm::vec3& position) { m_Position.x += position.x; m_Position.y += position.y; Recalculate_View_Matrix(); }
	const glm::vec3& Get_Position() const { return m_Position; }

	void Set_Rotation(float rotation) { m_Rotation = rotation; Recalculate_View_Matrix(); }
	float Get_Rotation() const { return m_Rotation; }

	const glm::mat4& Get_Projection_Matrix() const { return m_Projection_Matrix; }
	const glm::mat4& Get_View_Matrix() const { return m_View_Matrix; }
	const glm::mat4& Get_View_Projection_Matrix() const { return m_View_Projection_Matrix; }

private:
	void Recalculate_View_Matrix();

private:
	glm::mat4 m_Projection_Matrix;
	glm::mat4 m_View_Matrix;
	glm::mat4 m_View_Projection_Matrix;

	glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
	float m_Rotation = 0.0f;


};