#pragma once
#include "Renderer.h"
#include "Player.h"
#include "Enemies.h"
#include "Shader.h"
#include "Collision_Detection.h"
#include <vector>
#include "Orthographic_Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "Vertex_Buffer.h"
#include "Index_Buffer.h"
#include "Renderer2D.h"
#include "Texture_Manager.h"
#include "Input_Manager.h"
#include "Collision_Manager.h"
#include "Shadow_Casting.h"
#include "Window.h"
#include <irrKlang.h>
#include <functional>
//#pragma comment(lib, "irrKlang.lib")



class Game {
public:
	Window window;

	Vertex_Buffer buffer;
	Index_Buffer index_buffer;
	Renderer2D renderer;
	
	Texture_Manager texture_manager;
	Input_Manager input_manager;
	Collision_Manager collision_manager;
	
	Orthographic_Camera m_Camera;
	Shadow_Casting shadows;
	irrklang::ISoundEngine* SoundEngine;

	std::vector<std::vector<Player>> list;

	std::vector<Player> enemies_list;
	std::vector<Player> enemies_list1;
	std::vector<Player> collectible_list;
	std::vector<Player> collectible_list1;
	std::vector<Player> custom_sprite_list;
	std::vector<Player> shadow_quad_list;

	Player Next_Level = Player(-11, 0);
	Player Background = Player(-11, 0);
	float tile_size;
protected:
	unsigned int texture_slot[32];
	Player p1;

	bool Is_Grounded_x = false;
	bool Collides_x = false;

	bool Is_Grounded_y = false;
	bool Collides_y = false;

	float refresh_rate;
	int current_level = 0;
	bool collision = true;

private:

	unsigned int level_h;
	unsigned int level_w;

	unsigned int buffer_size;
	unsigned int total_buffer_size;
	unsigned int white_texture_id;
	unsigned int size_without_shadows = 0;

public:
	Game(Window& window, float character_scale);
	~Game();

	void Load_Next_Level(std::string& level_path, float width, float height, float character_scale);
	void load_level(Vertex_Array* vertex, std::string& level_path, float width, float height, float character_scale);
	void Load_Menu(float width, float height, float text_id);
	void Game_Over(float text_id);
	
	void Make_Custom_Sprite(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, float tex_id, glm::vec4 color);
	void Change_Sprite_Scale(Player sp, float x);

	void Repeat_Every(double time_step, double& previous_time, std::function<void(void)> f);

	std::vector<int> l;

	void set_player(Player p);
	Player get_player();
	void set_enemies(std::vector<Player> enemies_list);
	virtual void init();
	virtual void update();
	void update_player_position(float amount_x, float amount_y);
	void update_player_position_x();
	void update_player_position_y();
	void clean();
	void handle_collision(float scale_h, float scale_v, float amount_x, float amount_y, unsigned int axis);
	void handle_collision_new();
	void update_buffer();

////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Init_Shadow_points();
	void Calculate_Shadow_points();
///////////////////////////////////////////////////////////////////////////////////////////////////////////
};



/*
-------------------------------
Player p1 = Player();
Game(p1) {
	buffer = load_level();
	p1();
	for_each(enemy){
		enemy();
	}
	handle_opengl();
}
init() {
	
}

--------------------------------

Game game();

game.init();

while(){
	while(){
		game.update();
	}
	game.update_buffer();
	game.render();
}

---------------------------------
*/