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
#include <irrKlang.h>
#include <functional>
//#pragma comment(lib, "irrKlang.lib")



class Game {
public:

	Vertex_Buffer buffer;
	Index_Buffer index_buffer;
	Renderer2D renderer;
	Texture_Manager texture_manager;
	Input_Manager input_manager;
	Orthographic_Camera m_Camera;
	irrklang::ISoundEngine* SoundEngine;

	std::vector<std::vector<Player>> list;

	std::vector<Player> enemies_list;
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
	GLFWwindow * window;

	unsigned int vao;
	unsigned int vb;
	unsigned int ib;
	unsigned int shader;
	unsigned int level_h;
	unsigned int level_w;

	sCell* world = NULL;
	std::vector<sEdge> vecEdges;
	std::vector<sEdge> endingEdges;
	std::vector<glm::vec2> edges;
	std::vector<float> angles;
	std::vector<first_col> duo;

	unsigned int buffer_size;
	unsigned int total_buffer_size;
	unsigned int white_texture_id;
	unsigned int size_without_shadows = 0;

public:
	Game(std::string& level_path, GLFWwindow *win, float width, float height, float character_scale, float refresh_rate);
	~Game();
	void Init_Transparent_Texture();
	void load_level(Vertex_Array* vertex, std::string& level_path, float width, float height, float character_scale);
	void Load_Menu(float width, float height, float text_id);
	void Game_Over(float text_id);
	
	void Make_Custom_Sprite(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, float tex_id, glm::vec4 color);
	void Change_Sprite_Scale(Player sp, float x);

	void Load_Next_Level(std::string& level_path, float width, float height, float character_scale);

	void set_window(GLFWwindow *win);
	GLFWwindow *get_window();

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
	void render();
	void clean();
	void handle_collision(float scale_h, float scale_v, float amount_x, float amount_y, unsigned int axis);
	void update_buffer();
	void CalculateVisibilityPolygon(float ox, float oy, float radius);
	void convert_quads_to_polygons(int sx, int sy, int w, int h, float fBlockWidth, int pitch);
	std::pdd lineLineIntersection0(std::pdd A, std::pdd B, std::pdd C, std::pdd D);
	std::pdd lineLineIntersection1(std::pdd A, std::pdd B, std::pdd C, std::pdd D);
	bool lineLineIntersection2(std::pdd A, std::pdd B, std::pdd C, std::pdd D);
	void Init_Shadows();
	void Calculate_Shadows0();
	void Calculate_Shadows1();
	void Init_Shadow_points();
	void Calculate_Shadow_points();
	bool is_Edge_Connected(float x1, float y1, float x2, float y2);
	sEdge Find_Next_Edge(sEdge e2);
	sEdge Find_Starting_Edge(float x, float y);


protected:
	void Update_Camera_Uniform();
	void handle_opengl();
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