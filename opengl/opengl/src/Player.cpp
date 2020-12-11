#include "Player.h"

Player::Player()
{
	health = -100.0f;
}

Player::Player(unsigned int texture_id, unsigned int index, glm::vec2 pos, float scale) : texture_id(texture_id), position(pos), scale(scale)
{
	std::cout << "Player: " << position.x << "," << position.y << "\n";
	health = 100.0f;
	set_buffer_index(index, index + 1, index + 2, index + 3);
}

Player::Player(unsigned int texture_id, unsigned int index, glm::vec2 pos) : texture_id(texture_id), position(pos)
{
	health = 100.0f;
	set_buffer_index(index, index + 1, index + 2, index + 3);
}

Player::Player(unsigned int texture_id, unsigned int index, glm::vec2 pos, int i, int j) : texture_id(texture_id), position(pos)
{
	health = 100.0f;
	set_buffer_index(index, index + 1, index + 2, index + 3);
}

Player::Player(unsigned int texture_id, glm::vec4 c, glm::vec2 custom_position_0, glm::vec2 custom_position_1, glm::vec2 custom_position_2, glm::vec2 custom_position_3)
	: texture_id(texture_id), color(c), position(custom_position_3), custom_position_0(custom_position_0),
	  custom_position_1(custom_position_1), custom_position_2(custom_position_2), custom_position_3(custom_position_3)
{
	position = custom_position_3;
	health = 100.0f;
}

Player::Player(unsigned int texture_id, unsigned int index) : texture_id(texture_id)
{
	health = 100.0f;
	set_buffer_index(index, index + 1, index + 2, index + 3);
}

Player::~Player()
{
}

void Player::change_position(glm::vec2 new_pos)
{
	position.x += new_pos.x;
	position.y += new_pos.y;
	amount_h = new_pos.x;
	amount_v = new_pos.y;
}

void Player::change_position_x(float new_pos)
{
	position.x += new_pos;
}

void Player::change_position_y(float new_pos)
{
	position.y += new_pos;
}


void Player::fix_position(glm::vec2 new_pos)
{
	position.x = new_pos.x;
	position.y = new_pos.y;
	teleport = true;
}

void Player::set_custom_position(glm::vec2 position, float width, float height1, float height2, float offset)
{
	custom_position_0 = { position.x + offset, position.y + height1 };
	custom_position_1 = { position.x + width,  position.y + height1 };
	custom_position_2 = { position.x + width,  position.y + height2 };
	custom_position_3 = { position.x + offset, position.y + height2 };
}

void Player::set_custom_position(glm::vec2 position, float width, float height, float offset1)
{
	custom_position_0 = { position.x, position.y };
	custom_position_1 = { position.x + width + offset1,  position.y };
	custom_position_2 = { position.x + width + offset1,  position.y + height };
	custom_position_3 = { position.x, position.y + height };
}

void Player::respawn(glm::vec2 new_pos)
{
	position = new_pos;
}

void Player::despawn_custom_sprite()
{
	custom_position_0 = glm::vec2(0.0f, 0.0f);
	custom_position_1 = glm::vec2(0.0f, 0.0f);
	custom_position_2 = glm::vec2(0.0f, 0.0f);
	custom_position_3 = glm::vec2(0.0f, 0.0f);
}

glm::vec2 Player::get_position()
{
	//std::cout << "Player: {" << position.x << ", " << position.y << "}\n";
	return position;
}

float Player::get_health()
{
	return health;
}

float Player::get_scale()
{
	return scale;
}

unsigned int Player::get_texture_id()
{
	return texture_id;
}

void Player::set_texture_id(unsigned int tex_id)
{
	texture_id = tex_id;
}

void Player::set_buffer_index(unsigned int indx0, unsigned int indx1, unsigned int indx2, unsigned int indx3)
{
	buffer_index[0] = indx0;
	buffer_index[1] = indx1;
	buffer_index[2] = indx2;
	buffer_index[3] = indx3;
}

unsigned int * Player::get_buffer_index()
{
	return buffer_index;
}

void Player::set_teleport(bool tel)
{
	teleport = tel;
}

bool Player::get_teleport()
{
	return teleport;
}

void Player::despawn()
{
	fix_position(glm::vec2(-27.0f, -27.0f));
}

glm::vec2 Player::get_custom_position_0()
{
	return custom_position_0;
}

glm::vec2 Player::get_custom_position_1()
{
	return custom_position_1;
}

glm::vec2 Player::get_custom_position_2()
{
	return custom_position_2;
}

glm::vec2 Player::get_custom_position_3()
{
	return custom_position_3;
}
