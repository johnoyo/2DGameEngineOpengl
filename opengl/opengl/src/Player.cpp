#include "Player.h"

Player::Player()
{
	health = -100.0f;
}

Player::Player(unsigned int texture_id, glm::vec2 pos, float scale) : texture_id(texture_id), position(pos), scale(scale)
{
	health = 100.0f;
}

Player::Player(unsigned int texture_id, unsigned int index, glm::vec2 pos) : texture_id(texture_id), position(pos)
{
	health = 100.0f;
	set_buffer_index(index, index + 1, index + 2, index + 3);
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
}

void Player::fix_position(glm::vec2 new_pos)
{
	position.x = new_pos.x;
	position.y = new_pos.y;
	teleport = true;
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
	fix_position(glm::vec2(0.0f, 0.0f));
}
