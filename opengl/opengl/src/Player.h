#pragma once
#include "Renderer.h"

class Player
{
private:
	glm::vec2 position;
	float health;
	float scale;
	unsigned int texture_id;
	unsigned int buffer_index[4];
	bool teleport = false;
	glm::vec2 custom_position_0;
	glm::vec2 custom_position_1;
	glm::vec2 custom_position_2;
	glm::vec2 custom_position_3;
public:
	Player();
	Player(unsigned int texture_id, glm::vec2 pos, float scale);
	Player(unsigned int texture_id, unsigned int index);
	Player(unsigned int texture_id, unsigned int index, glm::vec2 pos);
	Player(unsigned int texture_id, glm::vec2 custom_position_0, glm::vec2 custom_position_1, glm::vec2 custom_position_2, glm::vec2 custom_position_3);
	~Player();
	void change_position(glm::vec2 new_pos);
	void fix_position(glm::vec2 new_pos);
	void respawn(glm::vec2 new_pos);
	glm::vec2 get_position();
	float get_health();
	float get_scale();

	unsigned int get_texture_id();
	void set_texture_id(unsigned int tex_id);

	void set_buffer_index(unsigned int indx0, unsigned int indx1, unsigned int indx2, unsigned int indx3);
	unsigned int *get_buffer_index();

	void set_teleport(bool tel);
	bool get_teleport();

	void despawn();

	glm::vec2 get_custom_position_0();
	glm::vec2 get_custom_position_1();
	glm::vec2 get_custom_position_2();
	glm::vec2 get_custom_position_3();

};
