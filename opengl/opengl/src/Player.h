#pragma once
#include "Renderer.h"

class Player
{
private:
	glm::vec2 position;
	float health;
	unsigned int texture_id;
	unsigned int buffer_index[4];
public:
	Player();
	Player(unsigned int texture_id, glm::vec2 pos);
	~Player();
	void change_position(glm::vec2 new_pos);
	void fix_position(glm::vec2 new_pos);
	glm::vec2 get_position();
	float get_health();

	unsigned int get_texture_id();
	void set_texture_id(unsigned int tex_id);

	void set_buffer_index(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2);
	unsigned int *get_buffer_index();

};
