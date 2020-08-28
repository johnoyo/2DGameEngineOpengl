#include "Player.h"

Player::Player()
{
	health = -100.0f;
}

Player::Player(unsigned int texture_id, glm::vec2 pos) : texture_id(texture_id), position(pos)
{
	health = 100.0f;
}

Player::~Player()
{
}

void Player::change_position(glm::vec2 new_pos)
{
	position.x += new_pos.x;
	position.y += new_pos.y;
}

void Player::fix_position(glm::vec2 new_pos)
{
	position.x = new_pos.x;
	position.y = new_pos.y;
}

glm::vec2 Player::get_position()
{
	std::cout << "Player: {" << position.x << ", " << position.y << "}\n";
	return position;
}

float Player::get_health()
{
	return health;
}

unsigned int Player::get_texture_id()
{
	return texture_id;
}

void Player::set_texture_id(unsigned int tex_id)
{
	texture_id = tex_id;
}

void Player::set_buffer_index(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2)
{
	buffer_index[0] = x1;
	buffer_index[1] = x2;
	buffer_index[2] = y1;
	buffer_index[3] = y2;
}

unsigned int * Player::get_buffer_index()
{
	return buffer_index;
}
