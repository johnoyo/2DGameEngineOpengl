#pragma once
#include "Renderer.h"

class Player
{
private:
	glm::vec2 position;
	float health;
	float scale;
	float scale_x;
	float scale_y;
	unsigned int texture_id;
	unsigned int buffer_index[4];
	bool teleport = false;

public:
	glm::vec4 color;
	glm::vec2 custom_position_0;
	glm::vec2 custom_position_1;
	glm::vec2 custom_position_2;
	glm::vec2 custom_position_3;
	Player();
	Player(unsigned int texture_id, unsigned int index, glm::vec2 pos, float scale);
	Player(unsigned int texture_id, unsigned int index, glm::vec2 pos, float scale_x, float scale_y);
	Player(unsigned int texture_id, unsigned int index);
	Player(unsigned int texture_id, unsigned int index, glm::vec2 pos);
	Player(unsigned int texture_id, unsigned int index, glm::vec2 pos, int i, int j);
	Player(unsigned int texture_id, glm::vec4 c, glm::vec2 custom_position_0, glm::vec2 custom_position_1, glm::vec2 custom_position_2, glm::vec2 custom_position_3);
	~Player();
	void change_position(glm::vec2 new_pos);
	void change_position_x(float new_pos);
	void change_position_y(float new_pos);
	void fix_position(glm::vec2 new_pos);
	void set_custom_position(glm::vec2 position, float width, float height1, float height2, float offset);
	void set_custom_position(glm::vec2 position, float width, float height, float offset1);
	void respawn(glm::vec2 new_pos);
	glm::vec2 get_position();
	float get_health();
	float get_scale();
	float get_scale_x();
	float get_scale_y();

	unsigned int get_texture_id();
	void set_texture_id(unsigned int tex_id);

	void set_buffer_index(unsigned int indx0, unsigned int indx1, unsigned int indx2, unsigned int indx3);
	unsigned int *get_buffer_index();

	void set_teleport(bool tel);
	bool get_teleport();
	float amount_h = 0.0f;
	float amount_v = 0.0f;
	float scale_h = 0.0f;
	float scale_v = 0.0f;

	void despawn();
	void despawn_custom_sprite();

	glm::vec2 get_custom_position_0();
	glm::vec2 get_custom_position_1();
	glm::vec2 get_custom_position_2();
	glm::vec2 get_custom_position_3();

};
