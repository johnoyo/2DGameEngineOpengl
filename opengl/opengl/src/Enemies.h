#pragma once
#include "Renderer.h"
#include "Game.h"

class Enemies {
private:
	glm::vec2 position;
	float health;
public:
	Enemies(struct Vertex_Array *buffer, int index);
	~Enemies();
	void change_position(glm::vec2 new_pos);
	void fix_position(glm::vec2 new_pos);
	glm::vec2 get_position();
	float get_health();

};