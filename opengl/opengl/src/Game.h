#pragma once
#include "Renderer.h"
#include "Player.h"
#include "Enemies.h"
#include "Shader.h"
#include "Collision_Detection.h"
#include<vector>



class Game {
private:
	GLFWwindow * window;

	unsigned int vao;
	unsigned int vb;
	unsigned int ib;
	unsigned int shader;
public:
	struct Vertex_Array * buffer;
	std::vector<Player> enemies_list;
	std::vector<Player> collectible_list;
	std::vector<Player> custom_sprite_list;
	Player Next_Level;
	float tile_size;
private:
	unsigned int buffer_size;
	unsigned int total_buffer_size;
	unsigned int * index_buffer;

protected:
	unsigned int texture_slot[32];
	Player p1;

	bool Is_Grounded_x = false;
	bool Collides_x = false;

	bool Is_Grounded_y = false;
	bool Collides_y = false;

	float refresh_rate;
	int current_level;

public:
	Game(std::string& level_path, GLFWwindow *win, float width, float height, float character_scale, float refresh_rate);
	~Game();
	struct Vertex_Array *load_level(Vertex_Array* vertex, std::string& level_path, float width, float height, float character_scale);
	struct Vertex_Array* Load_Menu(float width, float height, float text_id);
	void Game_Over(float text_id);
	unsigned int *make_indecies(int size);
	
	Vertex_Array* Make_Custom_Sprite(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, float tex_id);
	void Change_Sprite_Scale(Player sp, float x);

	void Load_Next_Level(std::string& level_path, float width, float height, float character_scale);

	void set_window(GLFWwindow *win);
	GLFWwindow *get_window();

	void set_size(unsigned int size);
	unsigned int get_size();
	struct Vertex_Array *get_buffer();

	void set_player(Player p);
	Player get_player();
	void set_enemies(std::vector<Player> enemies_list);
	virtual void init();
	virtual void update();
	void update_player_position(float amount_x, float amount_y);
	void update_player_position_x();
	void update_player_position_y();
	void render();
	void handle_collision(float scale_h, float scale_v, float amount_x, float amount_y, unsigned int axis);
	void update_buffer();

protected:
	void handle_opengl();
	Vertex_Array * fill_buffer(Vertex_Array *vertex, int *index, glm::vec2 new_position, glm::vec4 new_color, glm::vec2 new_tex_coord, float new_tex_id);

	glm::vec2 new_position(float width, float height);
	glm::vec4 new_color(float r, float g, float b, float a);
	glm::vec2 new_tex_coord(float x, float y);
	float new_tex_id(float id);

	void set_vao(unsigned int);
	void get_vao(unsigned int);
	void set_vb(unsigned int);
	void get_vb(unsigned int);
	void set_ib(unsigned int);
	void get_ib(unsigned int);
	void set_shader(unsigned int);
	void get_shader(unsigned int);
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