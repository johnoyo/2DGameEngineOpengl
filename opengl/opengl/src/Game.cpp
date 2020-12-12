#include "Game.h"

Game::Game(std::string& level_path, GLFWwindow *win, float width, float height, float character_scale, float refresh_rate) 
	: window(win), refresh_rate(refresh_rate), tile_size(character_scale), m_Camera(0.0f, width, 0.0f, height)
{

	/* Init sound Engine */
	SoundEngine = irrklang::createIrrKlangDevice();
	if (!SoundEngine)
	{
		std::cout << "Error: Could not create Sound Engine" << std::endl;
	}

	/* TODO: make this better */
	total_buffer_size = (100 * 100) * 4;

	buffer.Initialize(total_buffer_size);
	index_buffer.Make_Indecies(buffer.Get_Size());

	renderer.Initialize(buffer, index_buffer, m_Camera);
	texture_manager.Init_Transparent_Texture(renderer);

}

void Game::Load_Menu(float width, float height, float text_id) {

	buffer.Set_Size(4);

	/* Background data(position, color, texture) gets added first to the vertex buffer */
	buffer.Fill_Buffer(glm::vec2(0.0f, height), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), text_id);
	buffer.Fill_Buffer(glm::vec2(width, height), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), text_id);
	buffer.Fill_Buffer(glm::vec2(width, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), text_id);
	buffer.Fill_Buffer(glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), text_id);

	index_buffer.Clean();
	index_buffer.Make_Indecies(buffer.Get_Size());
	renderer.Upadte_Index_Buffer(buffer.Get_Size(), index_buffer);
	current_level++;

}

void Game::Game_Over(float text_id) {
	custom_sprite_list.clear();
	current_level = 0;
	Load_Menu(945.0f, 540.0f, text_id);

	index_buffer.Clean();
	index_buffer.Make_Indecies(buffer.Get_Size());
	renderer.Upadte_Index_Buffer(buffer.Get_Size(), index_buffer);
}



void Game::Load_Next_Level(std::string& level_path, float width, float height, float character_scale) {

	collectible_list.clear();
	collectible_list1.clear();
	enemies_list.clear();
	enemies_list1.clear();

	load_level(buffer.Get_Buffer(), level_path, width, height, character_scale);
	std::cout << "size: " << buffer.Get_Size() << ", total size: " << total_buffer_size << "\n";

	index_buffer.Clean();
	index_buffer.Make_Indecies(buffer.Get_Size());
	renderer.Upadte_Index_Buffer(buffer.Get_Size(), index_buffer);

	current_level++;
}

void Game::Make_Custom_Sprite(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, float tex_id, glm::vec4 color) {

	int index = buffer.Get_Size();
	int size = buffer.Get_Size();

	buffer.Fill_Buffer(tl, color, glm::vec2(0.0f, 1.0f), tex_id);
	buffer.Fill_Buffer(tr, color, glm::vec2(1.0f, 1.0f), tex_id);
	buffer.Fill_Buffer(br, color, glm::vec2(1.0f, 0.0f), tex_id);
	buffer.Fill_Buffer(bl, color, glm::vec2(0.0f, 0.0f), tex_id);

}

void Game::Change_Sprite_Scale(Player sp, float x)
{
	buffer.Get_Buffer()[sp.get_buffer_index()[1]].position.x += x;
	buffer.Get_Buffer()[sp.get_buffer_index()[2]].position.x += x;
}

Game::~Game()
{
}


void Game::load_level(Vertex_Array* vertex, std::string & level_path, float width, float height, float character_scale)
{
	FILE* f;
	fopen_s(&f, level_path.c_str(), "r");

	if (!f) return;
	char c;
	char acc[3];
	int i = 0, j = 0, s = 0, h = 0, w = 0;
	int index = 0;

	while ((c = fgetc(f)) != '\n') {
		if (c == ',') {
			index = 0;
			h = atoi(acc);
			continue;
		}
		if (c == '.') {
			index = 0;
			w = atoi(acc);
			continue;
		}
		acc[index] = c;
		index++;
	}

	if (world != NULL) free(world);
	world = (sCell*)malloc(h * w * sizeof(sCell));
	for (int i = 0; i < h * w; i++)
		world[i].exist = false;

	i = h - 1;

	struct pos {
		int i;
		int j;
		float k;
	};

	std::vector<pos> p;
	while ((c = fgetc(f)) != EOF) {
		if (c == 'B') {
			p.push_back({ i,j,16.0f });
		}
		else if (c == 'G') {
			p.push_back({ i,j,1.0f });
		}
		else if (c == 'C')
		{
			p.push_back({ i,j,3.0f });
		}
		else if (c == 'R')
		{
			p.push_back({ i,j,15.0f });
		}
		else if (c == 'E')
		{
			p.push_back({ i,j,4.0f });
		}
		else if (c == '>')
		{
			p.push_back({ i,j,8.0f });
		}
		else if (c == 'P')
		{
			p.push_back({ i,j,5.0f });
			std::cout << "i,j = " << i << ", " << j << "\n";
			s = p.size() - 1;
		}
		if (j == w) {

			j = 0;
			i--;
			continue;
		}
		j++;
	}

	//struct Vertex_Array *vertex = (struct Vertex_Array *)malloc(((p.size() + 1) * 4) * sizeof(struct Vertex_Array));
	buffer.Reset();

	/* Background data(position, color, texture) gets added first to the vertex buffer */
	Background = Player(6.0f, buffer.Get_Size(), glm::vec2(0.0f, 0.0f));
	buffer.Fill_Buffer(glm::vec2(0.0f, height), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 14.0f);
	buffer.Fill_Buffer(glm::vec2(width, height), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 14.0f);
	buffer.Fill_Buffer(glm::vec2(width, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 14.0f);
	buffer.Fill_Buffer(glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 14.0f);

	
	for (int k = 0; k < p.size(); k++) {

		if (k == s) continue;
		/*std::cout << p.at(k).i << "," << p.at(k).j << "\n"; */
		if (p.at(k).k == 3.0f) {

			collectible_list.push_back(Player(3, buffer.Get_Size(), glm::vec2(p.at(k).j* character_scale, p.at(k).i* character_scale)));
		}
		else if (p.at(k).k == 4.0f) {

			enemies_list.push_back(Player(4, buffer.Get_Size(), glm::vec2(p.at(k).j* character_scale, p.at(k).i* character_scale)));
		}
		else if (p.at(k).k == 16.0f) {

			enemies_list1.push_back(Player(16, buffer.Get_Size(), glm::vec2(p.at(k).j * character_scale, p.at(k).i * character_scale)));
		}
		else if (p.at(k).k == 8.0f) {

			Next_Level = Player(8, buffer.Get_Size(), glm::vec2(p.at(k).j * character_scale, p.at(k).i * character_scale));
		} else if (p.at(k).k == 15.0f) {

			collectible_list1.push_back(Player(15, buffer.Get_Size(), glm::vec2(p.at(k).j* character_scale, p.at(k).i* character_scale)));
		}

		buffer.Fill_Buffer(glm::vec2(p.at(k).j * character_scale, (p.at(k).i + 1) * character_scale), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), p.at(k).k);
		buffer.Fill_Buffer(glm::vec2((p.at(k).j + 1) * character_scale, (p.at(k).i + 1) * character_scale), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), p.at(k).k);
		buffer.Fill_Buffer(glm::vec2((p.at(k).j + 1) * character_scale, p.at(k).i * character_scale), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), p.at(k).k);
		buffer.Fill_Buffer(glm::vec2(p.at(k).j* character_scale, p.at(k).i * character_scale), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), p.at(k).k);
			
		world[p.at(k).i * w + p.at(k).j].exist = true;
	}

	for (int i = 0; i < custom_sprite_list.size(); i++) {
		std::cout << "Making custom sprite...\n";
		custom_sprite_list.at(i).set_buffer_index(buffer.Get_Size(), buffer.Get_Size() + 1, buffer.Get_Size() + 2, buffer.Get_Size() + 3);
		
		Make_Custom_Sprite(custom_sprite_list.at(i).get_custom_position_0(), custom_sprite_list.at(i).get_custom_position_1(),
			custom_sprite_list.at(i).get_custom_position_2(), custom_sprite_list.at(i).get_custom_position_3(),
			custom_sprite_list.at(i).get_texture_id(), custom_sprite_list.at(i).color);
	}

	/**
	* NOTE: if the player size is equal to the tile size, we have bugs. Just dont do that!
	*/

	/* Player data (position, color, texture) gets added last to the vertex buffer */
	p1 = Player(5, buffer.Get_Size(), glm::vec2(p.at(s).j * character_scale - 1.0f, p.at(s).i * character_scale - 1.0f), character_scale - 1.0f, character_scale - 1.0f);
	p1.fix_position(glm::vec2(p.at(s).j* character_scale - 1.0f, p.at(s).i* character_scale - 1.0f));

	buffer.Fill_Buffer(glm::vec2(p.at(s).j * character_scale - 1.0f, (p.at(s).i + 1) * character_scale - 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 5.0f);
	buffer.Fill_Buffer(glm::vec2((p.at(s).j + 1) * character_scale - 1.0f, (p.at(s).i + 1) * character_scale - 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 5.0f);
	buffer.Fill_Buffer(glm::vec2((p.at(s).j + 1) * character_scale - 1.0f, p.at(s).i * character_scale - 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 5.0f);
	buffer.Fill_Buffer(glm::vec2(p.at(s).j * character_scale - 1.0f, p.at(s).i * character_scale - 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 5.0f);

	std::cout << buffer.Get_Size() << std::endl;
	//for (int k = 0; k < ((p.size() + 1)*4); k++) {
	//	std::cout << vertex[k].position.x << ", " << vertex[k].position.y << "\n";
	//}

	fclose(f);
}

void Game::set_window(GLFWwindow * win)
{
	window = win;
}

GLFWwindow * Game::get_window()
{
	return window;
}

void Game::Repeat_Every(double time_step, double& previous_time, std::function<void(void)> f)
{
	double current_time = glfwGetTime();
	/* If a second has passed. */
	if (current_time - previous_time >= time_step)
	{
		// do stuff here
		f();

		previous_time = current_time;
	}

	float new_time = glfwGetTime();
	float frame_time = new_time - current_time;
	current_time = new_time;
}

void Game::set_player(Player p)
{
	p1 = p;
}

Player Game::get_player()
{
	return p1;
}

void Game::set_enemies(std::vector<Player> enemies_list)
{
}

void Game::init()
{
	
}

void Game::update()
{
}


void Game::update_buffer()
{
	if (p1.get_teleport())
	{
		std::cout << "Fix:" << p1.get_position().x << "," << p1.get_position().y << "\n";
		buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x = p1.get_position().x;
		buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y = p1.get_position().y + p1.get_scale_y();
			   										  
		buffer.Get_Buffer()[p1.get_buffer_index()[1]].position.x = p1.get_position().x + p1.get_scale_x();
		buffer.Get_Buffer()[p1.get_buffer_index()[1]].position.y = p1.get_position().y + p1.get_scale_y();
			   										  
		buffer.Get_Buffer()[p1.get_buffer_index()[2]].position.x = p1.get_position().x + p1.get_scale_x();
		buffer.Get_Buffer()[p1.get_buffer_index()[2]].position.y = p1.get_position().y;
			   										  
		buffer.Get_Buffer()[p1.get_buffer_index()[3]].position.x = p1.get_position().x;
		buffer.Get_Buffer()[p1.get_buffer_index()[3]].position.y = p1.get_position().y;

		p1.set_teleport(false);
	}
	
	
	buffer.Get_Buffer()[p1.get_buffer_index()[0]].tex_id = p1.get_texture_id();
	buffer.Get_Buffer()[p1.get_buffer_index()[1]].tex_id = p1.get_texture_id();
	buffer.Get_Buffer()[p1.get_buffer_index()[2]].tex_id = p1.get_texture_id();
	buffer.Get_Buffer()[p1.get_buffer_index()[3]].tex_id = p1.get_texture_id();

	if (Next_Level.get_texture_id() != -11) {
		buffer.Get_Buffer()[Next_Level.get_buffer_index()[0]].position.x = Next_Level.get_position().x;
		buffer.Get_Buffer()[Next_Level.get_buffer_index()[0]].position.y = Next_Level.get_position().y + tile_size;

		buffer.Get_Buffer()[Next_Level.get_buffer_index()[1]].position.x = Next_Level.get_position().x + tile_size;
		buffer.Get_Buffer()[Next_Level.get_buffer_index()[1]].position.y = Next_Level.get_position().y + tile_size;
		   									  
		buffer.Get_Buffer()[Next_Level.get_buffer_index()[2]].position.x = Next_Level.get_position().x + tile_size;
		buffer.Get_Buffer()[Next_Level.get_buffer_index()[2]].position.y = Next_Level.get_position().y;
		   									  
		buffer.Get_Buffer()[Next_Level.get_buffer_index()[3]].position.x = Next_Level.get_position().x;
		buffer.Get_Buffer()[Next_Level.get_buffer_index()[3]].position.y = Next_Level.get_position().y;
	}

	if (Background.get_texture_id() != -11) {
		buffer.Get_Buffer()[Background.get_buffer_index()[0]].position.x = Background.get_position().x;
		buffer.Get_Buffer()[Background.get_buffer_index()[0]].position.y = Background.get_position().y + 540.0f;

		buffer.Get_Buffer()[Background.get_buffer_index()[1]].position.x = Background.get_position().x + 945.0f;
		buffer.Get_Buffer()[Background.get_buffer_index()[1]].position.y = Background.get_position().y + 540.0f;

		buffer.Get_Buffer()[Background.get_buffer_index()[2]].position.x = Background.get_position().x + 945.0f;
		buffer.Get_Buffer()[Background.get_buffer_index()[2]].position.y = Background.get_position().y;

		buffer.Get_Buffer()[Background.get_buffer_index()[3]].position.x = Background.get_position().x;
		buffer.Get_Buffer()[Background.get_buffer_index()[3]].position.y = Background.get_position().y;
	}

	for (int i = 0; i < enemies_list.size(); i++) {
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[0]].position.x = enemies_list.at(i).get_position().x;
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[0]].position.y = enemies_list.at(i).get_position().y + tile_size;
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[0]].tex_id = enemies_list.at(i).get_texture_id();

		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[1]].position.x = enemies_list.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[1]].position.y = enemies_list.at(i).get_position().y + tile_size;
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[1]].tex_id = enemies_list.at(i).get_texture_id();

		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[2]].position.x = enemies_list.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[2]].position.y = enemies_list.at(i).get_position().y;
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[2]].tex_id = enemies_list.at(i).get_texture_id();

		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[3]].position.x = enemies_list.at(i).get_position().x;
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[3]].position.y = enemies_list.at(i).get_position().y;
		buffer.Get_Buffer()[enemies_list.at(i).get_buffer_index()[3]].tex_id = enemies_list.at(i).get_texture_id();
	}

	for (int i = 0; i < enemies_list1.size(); i++) {
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[0]].position.x = enemies_list1.at(i).get_position().x;
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[0]].position.y = enemies_list1.at(i).get_position().y + tile_size;
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[0]].tex_id = enemies_list1.at(i).get_texture_id();

		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[1]].position.x = enemies_list1.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[1]].position.y = enemies_list1.at(i).get_position().y + tile_size;
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[1]].tex_id = enemies_list1.at(i).get_texture_id();

		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[2]].position.x = enemies_list1.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[2]].position.y = enemies_list1.at(i).get_position().y;
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[2]].tex_id = enemies_list1.at(i).get_texture_id();

		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[3]].position.x = enemies_list1.at(i).get_position().x;
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[3]].position.y = enemies_list1.at(i).get_position().y;
		buffer.Get_Buffer()[enemies_list1.at(i).get_buffer_index()[3]].tex_id = enemies_list1.at(i).get_texture_id();
	}

	for (int i = 0; i < collectible_list.size(); i++) {
		buffer.Get_Buffer()[collectible_list.at(i).get_buffer_index()[0]].position.x = collectible_list.at(i).get_position().x;
		buffer.Get_Buffer()[collectible_list.at(i).get_buffer_index()[0]].position.y = collectible_list.at(i).get_position().y + tile_size;

		buffer.Get_Buffer()[collectible_list.at(i).get_buffer_index()[1]].position.x = collectible_list.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[collectible_list.at(i).get_buffer_index()[1]].position.y = collectible_list.at(i).get_position().y + tile_size;

		buffer.Get_Buffer()[collectible_list.at(i).get_buffer_index()[2]].position.x = collectible_list.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[collectible_list.at(i).get_buffer_index()[2]].position.y = collectible_list.at(i).get_position().y;

		buffer.Get_Buffer()[collectible_list.at(i).get_buffer_index()[3]].position.x = collectible_list.at(i).get_position().x;
		buffer.Get_Buffer()[collectible_list.at(i).get_buffer_index()[3]].position.y = collectible_list.at(i).get_position().y;
	}

	for (int i = 0; i < collectible_list1.size(); i++) {
		buffer.Get_Buffer()[collectible_list1.at(i).get_buffer_index()[0]].position.x = collectible_list1.at(i).get_position().x;
		buffer.Get_Buffer()[collectible_list1.at(i).get_buffer_index()[0]].position.y = collectible_list1.at(i).get_position().y + tile_size;

		buffer.Get_Buffer()[collectible_list1.at(i).get_buffer_index()[1]].position.x = collectible_list1.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[collectible_list1.at(i).get_buffer_index()[1]].position.y = collectible_list1.at(i).get_position().y + tile_size;

		buffer.Get_Buffer()[collectible_list1.at(i).get_buffer_index()[2]].position.x = collectible_list1.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[collectible_list1.at(i).get_buffer_index()[2]].position.y = collectible_list1.at(i).get_position().y;

		buffer.Get_Buffer()[collectible_list1.at(i).get_buffer_index()[3]].position.x = collectible_list1.at(i).get_position().x;
		buffer.Get_Buffer()[collectible_list1.at(i).get_buffer_index()[3]].position.y = collectible_list1.at(i).get_position().y;
	}

	/*for (int i = 0; i < list.size(); i++) {
		for (int j = 0; i < list.at(i).size(); j++) {

			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[0]].position.x = list.at(i).at(j).get_position().x;
			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[0]].position.y = list.at(i).at(j).get_position().y + tile_size;
			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[0]].tex_id = list.at(i).at(j).get_texture_id();

			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[1]].position.x = list.at(i).at(j).get_position().x + tile_size;
			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[1]].position.y = list.at(i).at(j).get_position().y + tile_size;
			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[1]].tex_id = list.at(i).at(j).get_texture_id();

			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[2]].position.x = list.at(i).at(j).get_position().x + tile_size;
			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[2]].position.y = list.at(i).at(j).get_position().y;
			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[2]].tex_id = list.at(i).at(j).get_texture_id();

			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[3]].position.x = list.at(i).at(j).get_position().x;
			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[3]].position.y = list.at(i).at(j).get_position().y;
			buffer.Get_Buffer()[list.at(i).at(j).get_buffer_index()[3]].tex_id = list.at(i).at(j).get_texture_id();
		}
	}*/
	
	for (int i = 0; i < custom_sprite_list.size(); i++) {
		
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[0]].position.x = custom_sprite_list.at(i).get_custom_position_0().x;
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[0]].position.y = custom_sprite_list.at(i).get_custom_position_0().y;

		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[1]].position.x = custom_sprite_list.at(i).get_custom_position_1().x;
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[1]].position.y = custom_sprite_list.at(i).get_custom_position_1().y;

		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[2]].position.x = custom_sprite_list.at(i).get_custom_position_2().x;
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[2]].position.y = custom_sprite_list.at(i).get_custom_position_2().y;

		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[3]].position.x = custom_sprite_list.at(i).get_custom_position_3().x;
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[3]].position.y = custom_sprite_list.at(i).get_custom_position_3().y;

		/*buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[0]].position.x = custom_sprite_list.at(i).get_position().x;
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[0]].position.y = custom_sprite_list.at(i).get_position().y + tile_size;
																													 
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[1]].position.x = custom_sprite_list.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[1]].position.y = custom_sprite_list.at(i).get_position().y + tile_size;
																													 
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[2]].position.x = custom_sprite_list.at(i).get_position().x + tile_size;
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[2]].position.y = custom_sprite_list.at(i).get_position().y;
																													 
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[3]].position.x = custom_sprite_list.at(i).get_position().x;
		buffer.Get_Buffer()[custom_sprite_list.at(i).get_buffer_index()[3]].position.y = custom_sprite_list.at(i).get_position().y;*/
	}
	
}

void Game::clean()
{
	std::cout << "Cleaning..." << std::endl;
	renderer.Clear();
	if (buffer.Get_Buffer() != NULL) free(buffer.Get_Buffer());
	if (index_buffer.Get_Index_Buffer() != NULL) free(index_buffer.Get_Index_Buffer());
	if (world != NULL) free(world);
}

void Game::update_player_position(float amount_x, float amount_y)
{
	//p1.change_position(glm::vec2(amount_x, amount_y));
	//std::cout << p1.get_scale();

	buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x += amount_x;
	buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y += amount_y;
	buffer.Get_Buffer()[p1.get_buffer_index()[1]].position.x += amount_x;
	buffer.Get_Buffer()[p1.get_buffer_index()[1]].position.y += amount_y;
	buffer.Get_Buffer()[p1.get_buffer_index()[2]].position.x += amount_x;
	buffer.Get_Buffer()[p1.get_buffer_index()[2]].position.y += amount_y;
	buffer.Get_Buffer()[p1.get_buffer_index()[3]].position.x += amount_x;
	buffer.Get_Buffer()[p1.get_buffer_index()[3]].position.y += amount_y;

}

void Game::update_player_position_x()
{
	std::cout << p1.get_scale();
	buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.x = p1.get_position().x;
	buffer.Get_Buffer()[p1.get_buffer_index()[1]].position.x = p1.get_position().x + p1.get_scale_x();
	buffer.Get_Buffer()[p1.get_buffer_index()[2]].position.x = p1.get_position().x + p1.get_scale_x();
	buffer.Get_Buffer()[p1.get_buffer_index()[3]].position.x = p1.get_position().x;

}

void Game::update_player_position_y()
{
	std::cout << p1.get_scale();
	buffer.Get_Buffer()[p1.get_buffer_index()[0]].position.y = p1.get_position().y + p1.get_scale_y();
	buffer.Get_Buffer()[p1.get_buffer_index()[1]].position.y = p1.get_position().y + p1.get_scale_y();
	buffer.Get_Buffer()[p1.get_buffer_index()[2]].position.y = p1.get_position().y;
	buffer.Get_Buffer()[p1.get_buffer_index()[3]].position.y = p1.get_position().y;

}


/*
How is the Collision supposed to work now:
1) I move the player in the x and y axis according to input

2) Handle movement in the x-axis

	2a) Update the vertex buffer with amount player has move on x-axis
	2b) Check the player index in the buffer(i.e the player positions) if 
		he collides with any other tile
	2c) If he collides, change the player x-position so they are just touching 
		with the colliding tile
	2d) Update the vertex buffer with the new x-position of the player

3) Handle movement in the y-axis

	3a) Update the vertex buffer with amount player has move on y-axis
	3b) Check the player index in the buffer(i.e the player positions) if 
		he collides with any other tile
	3c) If he collides, change the player y-position so they are just touching 
		with the colliding tile
	3d) Update the vertex buffer with the new y-position of the player

4) Render

In short:
- move player
- update buffer only for x-axis movement
- collision handling and correction on x-axis
	- update player x-position (if we have collision)
	- update buffer only for x-axis with new position (if we have collision)
- update buffer only for y-axis movement
- collision detection and correction on y-axis
	- update player y-position (if we have collision)
	- update buffer only for y-axis with new position (if we have collision)
- render

*/

/* 
TODO: IMPORTANT!!! I FOUND THE BUG WHERE THE PLAYER FALLS THROUGH THE GROUND.
IT HAPPENS WHEN ALL THE CORNERS OF THE PLAYER ARE INSIDE A TILE(SINCE THE PLAYER SCALE IS SMALLER BY 1)

ILLUSTARTION:
								ppp
								ppp
								ppp
							   ggggg
							   ggggg
							   ggggg
FIX THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/


void Game::handle_collision(float scale_h, float scale_v, float amount_x, float amount_y, unsigned int axis)
{
	/* change the position of the player in the x-axis (i.e last quad in vertex buffer) according to input */
	update_player_position(amount_x, 0.0f);

	/* check if there is collision on x-axis */
	/* NOTE: in line 577 and 583 the size was size_without_shows and i changed it to get_size() */
	buffer.Set_Buffer(check_for_collitions(buffer.Get_Buffer(), &p1, buffer.Get_Size(), scale_h, &Is_Grounded_x, &Collides_x, X_AXIS));

	/* change the position of the player in the y-axis (i.e last quad in vertex buffer) according to input */
	update_player_position(0.0f, amount_y);

	/* check if there is collision on y-axis */
	buffer.Set_Buffer(check_for_collitions(buffer.Get_Buffer(), &p1, buffer.Get_Size(), scale_v, &Is_Grounded_y, &Collides_y, Y_AXIS));

	p1.set_teleport(false);
}

void Game::handle_collision_new()
{
	/* change the position of the player in the x-axis according to input */
	update_player_position(p1.amount_h, 0.0f);
	/* Here we change the position x of p1 if the there is collision */
	collision_manager.Check_For_Collisions(&buffer.buffer, &p1, buffer.Get_Size(), tile_size, &Is_Grounded_x, &Collides_x, X_AXIS);
	//buffer.Set_Buffer(collision_manager.Check_For_Collisions(buffer.Get_Buffer(), &p1, buffer.Get_Size(), &Is_Grounded_x, &Collides_x, X_AXIS));

	/* change the position of the player in the y-axis (i.e last quad in vertex buffer) according to input */
	update_player_position(0.0f, p1.amount_v);
	/* Here we change the position y of p1 if the there is collision */
	collision_manager.Check_For_Collisions(&buffer.buffer, &p1, buffer.Get_Size(), tile_size, &Is_Grounded_y, &Collides_y, Y_AXIS);
	//buffer.Set_Buffer(collision_manager.Check_For_Collisions(buffer.Get_Buffer(), &p1, buffer.Get_Size(), &Is_Grounded_y, &Collides_y, Y_AXIS));
}

void Game::convert_quads_to_polygons(int sx, int sy, int w, int h, float fBlockWidth, int pitch)
{
	// Clear "PolyMap"
	vecEdges.clear();

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			for (int j = 0; j < 4; j++)
			{
				world[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
				world[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
			}

	// Iterate through region from top left to bottom right
	for (int x = 1; x < w - 1; x++)
		for (int y = 1; y < h - 1; y++)
		{
			// Create some convenient indices
			int i = (y + sy) * pitch + (x + sx);			// This
			int n = (y + sy - 1) * pitch + (x + sx);		// Northern Neighbour
			int s = (y + sy + 1) * pitch + (x + sx);		// Southern Neighbour
			int w = (y + sy) * pitch + (x + sx - 1);	// Western Neighbour
			int e = (y + sy) * pitch + (x + sx + 1);	// Eastern Neighbour

			// If this cell exists, check if it needs edges
			if (world[i].exist)
			{
				// If this cell has no western neighbour, it needs a western edge
				if (!world[w].exist)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (world[n].edge_exist[WEST])
					{
						// Northern neighbour has a western edge, so grow it downwards
						vecEdges[world[n].edge_id[WEST]].ey += fBlockWidth;
						world[i].edge_id[WEST] = world[n].edge_id[WEST];
						world[i].edge_exist[WEST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						sEdge edge;
						edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
						edge.ex = edge.sx; edge.ey = edge.sy + fBlockWidth;

						// Add edge to Polygon Pool
						int edge_id = vecEdges.size();
						vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[WEST] = edge_id;
						world[i].edge_exist[WEST] = true;
					}
				}

				// If this cell dont have an eastern neignbour, It needs a eastern edge
				if (!world[e].exist)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (world[n].edge_exist[EAST])
					{
						// Northern neighbour has one, so grow it downwards
						vecEdges[world[n].edge_id[EAST]].ey += fBlockWidth;
						world[i].edge_id[EAST] = world[n].edge_id[EAST];
						world[i].edge_exist[EAST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						sEdge edge;
						edge.sx = (sx + x + 1) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
						edge.ex = edge.sx; edge.ey = edge.sy + fBlockWidth;

						// Add edge to Polygon Pool
						int edge_id = vecEdges.size();
						vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[EAST] = edge_id;
						world[i].edge_exist[EAST] = true;
					}
				}

				// If this cell doesnt have a northern neignbour, It needs a northern edge
				if (!world[n].exist)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (world[w].edge_exist[NORTH])
					{
						// Western neighbour has one, so grow it eastwards
						vecEdges[world[w].edge_id[NORTH]].ex += fBlockWidth;
						world[i].edge_id[NORTH] = world[w].edge_id[NORTH];
						world[i].edge_exist[NORTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so create one
						sEdge edge;
						edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
						edge.ex = edge.sx + fBlockWidth; edge.ey = edge.sy;

						// Add edge to Polygon Pool
						int edge_id = vecEdges.size();
						vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[NORTH] = edge_id;
						world[i].edge_exist[NORTH] = true;
					}
				}

				// If this cell doesnt have a southern neignbour, It needs a southern edge
				if (!world[s].exist)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (world[w].edge_exist[SOUTH])
					{
						// Western neighbour has one, so grow it eastwards
						vecEdges[world[w].edge_id[SOUTH]].ex += fBlockWidth;
						world[i].edge_id[SOUTH] = world[w].edge_id[SOUTH];
						world[i].edge_exist[SOUTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so I need to create one
						sEdge edge;
						edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y + 1) * fBlockWidth;
						edge.ex = edge.sx + fBlockWidth; edge.ey = edge.sy;

						
						// Add edge to Polygon Pool
						int edge_id = vecEdges.size();
						vecEdges.push_back(edge);
						

						// Update tile information with edge information
						world[i].edge_id[SOUTH] = edge_id;
						world[i].edge_exist[SOUTH] = true;
					}
				}
			}
		}

	bool flag1 = false, flag2 = false;
	edges.clear();
	for (auto& e1 : vecEdges)
	{
		for (auto& e2 : edges) {
			if (edges.size() != 0) {
				if (e1.sx == e2.x && e1.sy == e2.y) flag1 = true;
				if (e1.ex == e2.x && e1.ey == e2.y) flag2 = true;
			}
			else {
				edges.push_back({ e1.sx , e1.sy });
				edges.push_back({ e1.ex , e1.ey });
			}
		}
		if (!flag1) edges.push_back({ e1.sx, e1.sy });
		if (!flag2) edges.push_back({ e1.ex, e1.ey });

		flag1 = false;
		flag2 = false;
		
	}

	for (auto& e2 : edges)
		std::cout << e2.x << ",, " << e2.y << "\n";

		

}

inline float Det(float a, float b, float c, float d)
{
	return a * d - b * c;
}

///Calculate intersection of two lines.
///\return true if found, false if not found or error
bool LineLineIntersect(std::pdd A, std::pdd B, std::pdd C, std::pdd D, float& ixOut, float& iyOut) //Output 
{
	//http://mathworld.wolfram.com/Line-LineIntersection.html

	float detL1 = Det(A.first, A.second, B.first, B.second);
	float detL2 = Det(C.first, C.second, D.first, D.second);
	float x1mx2 = A.first - B.first;
	float x3mx4 = C.first - D.first;
	float y1my2 = A.second - B.second;
	float y3my4 = C.second - D.second;
	
	float xnom = Det(detL1, x1mx2, detL2, x3mx4);
	float ynom = Det(detL1, y1my2, detL2, y3my4);
	float denom = Det(x1mx2, y1my2, x3mx4, y3my4);
	if (denom == 0.0)//Lines don't seem to cross
	{
		ixOut = NAN;
		iyOut = NAN;
		return false;
	}

	ixOut = xnom / denom;
	iyOut = ynom / denom;
	if (!isfinite(ixOut) || !isfinite(iyOut)) //Probably a numerical issue
		return false;

	return true; //All OK
}

std::pdd Game::lineLineIntersection0(std::pdd A, std::pdd B, std::pdd C, std::pdd D)
{
	// Line AB represented as a1x + b1y = c1 
	float a1 = B.second - A.second;
	float b1 = A.first - B.first;
	float c1 = a1 * (A.first) + b1 * (A.second);

	// Line CD represented as a2x + b2y = c2 
	float a2 = D.second - C.second;
	float b2 = C.first - D.first;
	float c2 = a2 * (C.first) + b2 * (C.second);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		// The lines are parallel. This is simplified 
		// by returning a pair of FLT_MAX 
		return std::make_pair(FLT_MAX, FLT_MAX);
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		float y = (a1 * c2 - a2 * c1) / determinant;
		if (std::fabsf(x - C.first) < 0.0001f || std::fabsf(y - C.second) < 0.0001f
		    || std::fabsf(x - D.first) < 0.0001f || std::fabsf(y - D.second) < 0.0001f) return std::make_pair(x, y);
		if (x <= C.first - 1.0f || y <= C.second - 1.0f) return std::make_pair(FLT_MAX, FLT_MAX);
		if (x >= D.first + 1.0f || y >= D.second + 1.0f) return std::make_pair(FLT_MAX, FLT_MAX);
		return std::make_pair(FLT_MAX, FLT_MAX);
		/*if (x <= C.first - 1.0f || y <= C.second - 1.0f) return std::make_pair(FLT_MAX, FLT_MAX);
		if (x >= D.first + 1.0f || y >= D.second + 1.0f) return std::make_pair(FLT_MAX, FLT_MAX);
		return std::make_pair(x, y);*/
	}

}

std::pdd Game::lineLineIntersection1(std::pdd A, std::pdd B, std::pdd C, std::pdd D)
{

	// Store the values for fast access and easy
	// equations-to-code conversion
	float x1 = A.first, x2 = B.first, x3 = C.first, x4 = D.first;
	float y1 = A.second, y2 = B.second, y3 = C.second, y4 = D.second;

	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return std::make_pair(FLT_MAX, FLT_MAX);

	// Get the x and y
	float pre = (x1 * y2 - y1 * x2), post = (x3 * y4 - y3 * x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

	float f = x - std::floorf(x);
	float c = std::ceilf(x) - x;
	float xx = (f <= c) ? std::floorf(x) : std::ceilf(x);
	if (f < 0.0001f || c < 0.0001f) x = xx;

	f = y - std::floorf(y);
	c = std::ceilf(y) - y;
	float yy = (f <= c) ? std::floorf(y) : std::ceilf(y);
	if (f < 0.0001f || c < 0.0001f) y = yy;

	// Check if the x and y coordinates are within both lines
	if (x < std::min(x1, x2) || x > std::max(x1, x2) ||
		x < std::min(x3, x4) || x > std::max(x3, x4)) return std::make_pair(FLT_MAX, FLT_MAX);
	if (y < std::min(y1, y2) || y > std::max(y1, y2) ||
		y < std::min(y3, y4) || y > std::max(y3, y4)) return std::make_pair(FLT_MAX, FLT_MAX);

	// Return the point of intersection
	return std::make_pair(x, y);

}

bool onLine(std::pdd A, std::pdd B, float x, float y) {   //check whether p is on the line or not
	if (x <= std::max(A.first, B.first) && x <= std::min(A.first, B.first) &&
		(y <= std::max(A.second, B.second) && y <= std::min(A.second, B.second)))
		return true;

	return false;
}

int direction(std::pdd a, std::pdd b, std::pdd c) {
	int val = (b.second - a.second) * (c.first - b.first) - (b.first - a.first) * (c.second - b.second);
	if (val == 0)
		return 0;     //colinear
	else if (val < 0)
		return 2;    //anti-clockwise direction
	return 1;    //clockwise direction
}

bool Game::lineLineIntersection2(std::pdd A, std::pdd B, std::pdd C, std::pdd D) {
	//four direction for two lines and points of other line
	int dir1 = direction(A, B, C);
	int dir2 = direction(A, B, D);
	int dir3 = direction(C, D, A);
	int dir4 = direction(C, D, B);

	if (dir1 != dir2 && dir3 != dir4)
		return true; //they are intersecting

	if (dir1 == 0 && onLine(A, B, C.first, C.second)) //when p2 of line2 are on the line1
		return true;

	if (dir2 == 0 && onLine(A, B, D.first, D.second)) //when p1 of line2 are on the line1
		return true;

	if (dir3 == 0 && onLine(C, D, A.first, A.second)) //when p2 of line1 are on the line2
		return true;

	if (dir4 == 0 && onLine(C, D, B.first, B.second)) //when p1 of line1 are on the line2
		return true;

	return false;
}


float find_min(float a, float b) {
	return (b < a) ? b : a;
}

void find_min_points(std::vector<first_col> c, float ox, float oy, float* i_x, float* i_y) {

	float min = 10000.0f, prev_min = 10000.0f;
	int keep_index = 0;

	if (c.size() == 0) return;

	for (int i = 0; i < c.size(); i++) {
		prev_min = min;
		min = find_min(c.at(i).xy, min);
		if (min < prev_min) keep_index = i;
	}
	//std::cout << c.size() << ", " << keep_index << "\n";
	*i_x = c.at(keep_index).x;
	*i_y = c.at(keep_index).y;

}

float find_max(float a, float b) {
	return (b > a) ? b : a;
}

void find_max_points(std::vector<first_col> c, float ox, float oy, float* i_x, float* i_y) {

	float max = 0.0f, prev_max = 0.0f;
	int keep_index = 0;

	if (c.size() == 0) return;

	for (int i = 0; i < c.size(); i++) {
		prev_max = max;
		max = find_max(c.at(i).xy, max);
		if (max > prev_max) keep_index = i;
	}
	//std::cout << c.size() << ", " << keep_index << "\n";
	*i_x = c.at(keep_index).x;
	*i_y = c.at(keep_index).y;

}

void Game::CalculateVisibilityPolygon(float ox, float oy, float radius)
{
	std::vector<first_col> cols;
	endingEdges.clear();
	/*for (auto& e1 : vecEdges)
	{
		std::cout << e1.sx << ", " << e1.sy << "\n";
		std::cout << e1.ex << ", " << e1.ey << "\n\n";
	}*/

	int counter0 = 0;
	int counter1 = 0;
	float save_rdx = 0.0f, save_rdy = 0.0f;
	std::vector<sEdge> tmp;
	// For each edge in PolyMap
	for (auto& e1 : edges)
	{
		cols.clear();

		float rdx, rdy;
		rdx = e1.x - ox;
		rdy = e1.y - oy;

		float base_ang = atan2f(rdy, rdx);

		rdx = radius * cosf(base_ang);
		rdy = radius * sinf(base_ang);

		rdx += ox;
		rdy += oy;

		float i_x = 0.0f, i_y = 0.0f;

		// Check for ray intersection with all edges
		for (auto& e2 : vecEdges)
		{
			std::pdd A = std::make_pair(ox, oy);
			std::pdd B = std::make_pair(rdx, rdy);
			std::pdd C = std::make_pair(e2.sx, e2.sy);
			std::pdd D = std::make_pair(e2.ex, e2.ey);

			std::pdd intersection = lineLineIntersection1(A, B, C, D);

			if (intersection.first == FLT_MAX &&
				intersection.second == FLT_MAX)
			{
				//std::cout << "The given lines AB and CD are parallel.\n";
			}
			else
			{
				cols.push_back({ intersection.first, intersection.second, sqrtf(std::fabsf(intersection.first * intersection.first - ox * ox) + std::fabsf(intersection.second * intersection.second - oy * oy)) });
			}

			/*float tmp_x, tmp_y;
			if (LineLineIntersect(A, B, C, D, tmp_x, tmp_y)) {
				cols.push_back({ tmp_x, tmp_y, sqrtf(std::fabsf(tmp_x * tmp_x - ox * ox) + std::fabsf(tmp_y * tmp_y - oy * oy)) });
			}*/


		}

		float ii_x = 0.0f, ii_y = 0.0f;
		find_min_points(cols, ox, oy, &ii_x, &ii_y);
		save_rdx = rdx;
		save_rdy = rdy;

		float ang = 0;
		counter0 = 0;
		counter1 = 0;

		tmp.clear();

		float f = ii_x - std::floorf(ii_x);
		float c = std::ceilf(ii_x) - ii_x;
		float x = (f <= c) ? std::floorf(ii_x) : std::ceilf(ii_x);

		f = ii_y - std::floorf(ii_y);
		c = std::ceilf(ii_y) - ii_y;
		float y = (f <= c) ? std::floorf(ii_y) : std::ceilf(ii_y);

		for (auto& e2 : vecEdges)
		{
			if (e2.sx == x && e2.sy == y) tmp.push_back({ e2.sx, e2.sy, e2.ex, e2.ey, e2.theta });
			if (e2.ex == x && e2.ey == y) tmp.push_back({ e2.sx, e2.sy, e2.ex, e2.ey, e2.theta });
		}

		if (tmp.size() == 0) {
			endingEdges.push_back({ ii_x, ii_y, save_rdx, save_rdy, base_ang, false });
			continue;
		}

		/*for (auto& e2 : vecEdges)
		{
			if (e2.sx == e1.x && e2.sy == e1.y) tmp.push_back({ e2.sx, e2.sy, e2.ex, e2.ey, e2.theta });
			if (e2.ex == e1.x && e2.ey == e1.y) tmp.push_back({ e2.sx, e2.sy, e2.ex, e2.ey, e2.theta });
		}*/

		for (int j = 0; j < 2; j++)
		{
			if (j == 0)	ang = base_ang - 0.0001f;
			if (j == 1)	ang = base_ang + 0.0001f;

			rdx = radius * cosf(ang);
			rdy = radius * sinf(ang);

			rdx += ox;
			rdy += oy;

			float i_x = 0.0f, i_y = 0.0f;
			std::vector<first_col> cols;


			for (auto& e2 : tmp)
			{
				std::pdd A = std::make_pair(ox, oy);
				std::pdd B = std::make_pair(rdx, rdy);
				std::pdd C = std::make_pair(e2.sx, e2.sy);
				std::pdd D = std::make_pair(e2.ex, e2.ey);

				std::pdd intersection = lineLineIntersection1(A, B, C, D);

				if (intersection.first != FLT_MAX &&
					intersection.second != FLT_MAX)
				{
					if (j == 0)	counter0++;
					if (j == 1)	counter1++;
					
				}

				/*if (lineLineIntersection2(A, B, C, D))
				{
					if (j == 0)	counter0++;
					if (j == 1)	counter1++;

				}*/
			}


		}
		if(counter0 == 0 || counter1 == 0)
			endingEdges.push_back({ ii_x, ii_y, save_rdx, save_rdy, base_ang, true });
		else
			endingEdges.push_back({ ii_x, ii_y, save_rdx, save_rdy, base_ang, false });

	}
	/*for (auto& e1 : endingEdges)
	{
		std::cout << "Start : " << e1.sx << ", " << e1.sy << "\n";
		std::cout << "End   : " << e1.ex << ", " << e1.ey << "\n";
		std::cout << "Theta : " << e1.theta << "\n\n";
	}*/

	angles.clear();
	for (auto& e1 : endingEdges)
	{
		angles.push_back({ e1.theta });
	}

	/*for (auto& e2 : angles)
		std::cout << e2 << "\n";*/

	std::sort(angles.begin(), angles.end(), std::greater<float>());

	//std::cout << "\n\n";

	/*for (auto& e2 : angles)
		std::cout << e2 << "\n";*/

}

//int find_min_theta(std::vector<sEdge> c, int j) {
//
//	float min = 10000.0f, prev_min = 10000.0f;
//	int keep_index = 0;
//
//	if (c.size() == 0) return;
//
//	for (int i = 0; i < c.size(); i++) {
//		prev_min = min;
//		min = find_min((c.at(i).theta - c.at(j).theta), min);
//		if (min < prev_min) keep_index = i;
//	}
//	return keep_index;
//
//}

int find_edge(std::vector<sEdge> c, float angle) {
	for (int i = 0; i < c.size(); i++) {
		if (c.at(i).theta == angle) return i;
	}
}

bool Point_IsEqual(float e1x, float e1y, float e2x, float e2y) {
	return ((e1x == e2x) && (e1y == e2y));
}

sEdge Game::Find_Next_Edge(sEdge e2) {
	for (auto& e1 : vecEdges)
	{
		if (Point_IsEqual(e1.sx, e1.sy, e2.ex, e2.ey) || Point_IsEqual(e1.ex, e1.ey, e2.ex, e2.ey) && e2.sx != e1.sx && e2.sy != e1.sy) return e1;
	}
	return { FLT_MAX, FLT_MAX, FLT_MAX , FLT_MAX, FLT_MAX };
}

sEdge Game::Find_Starting_Edge(float x, float y) {
	for (auto& e1 : vecEdges)
	{
		if (Point_IsEqual(e1.sx, e1.sy, x, y) || Point_IsEqual(e1.ex, e1.ey, x, y)) return e1;
	}
	return { FLT_MAX, FLT_MAX, FLT_MAX , FLT_MAX, FLT_MAX };
}

bool Edge_Exists_In(float x, float y, std::vector<glm::vec2> v) {

	for (auto& e1 : v)
	{
		if (Point_IsEqual(x, y, e1.x, e1.y)) return true;
	}
	return false;
}

bool Game::is_Edge_Connected(float x1, float y1, float x2, float y2) {

	if (!Edge_Exists_In(x1, y1, edges) || !Edge_Exists_In(x2, y2, edges)) {
		return true;
	}

	sEdge prev_e = { FLT_MAX, FLT_MAX, FLT_MAX , FLT_MAX, FLT_MAX };
	sEdge e = Find_Starting_Edge(x1, y1);
	while (!Point_IsEqual(e.sx, e.sy, prev_e.sx, prev_e.sy) && !Point_IsEqual(e.sx, e.sy, prev_e.ex, prev_e.ey)) {

		if (Point_IsEqual(e.sx, e.sy, x2, y2) || Point_IsEqual(e.ex, e.ey, x2, y2)) return true;

		prev_e = e;
		e = Find_Next_Edge(e);
	}

	return false;
}


void Game::Init_Shadows()
{
	//shadow_quad_list.clear();
	//int index = get_size();
	///* NOTE: put this size in collision detecting function to work properly */
	//size_without_shadows = get_size();

	//int k = 0, l = 0;
	//for (int i = 0; i < angles.size(); i++) {
	//	if (angles.at(i) >= 2.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
	//			shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), k, l));

	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).sx, endingEdges.at(k).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
	//		}
	//	}
	//	else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
	//			shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), k, l));

	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).sx, endingEdges.at(k).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
	//		}
	//	}
	//	else if (angles.at(i) <= -2.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
	//			shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), k, l));

	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).sx, endingEdges.at(k).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
	//		}
	//	}
	//	else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1)%angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
	//			shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), k, l));

	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).sx, endingEdges.at(k).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
	//			buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
	//		}
	//	}
	//}


	//set_size(index);
	////std::cout << "Size : " << get_size() << "\n\n";

	//if (index_buffer != NULL) free(index_buffer);
	//index_buffer = make_indecies(get_size());
	//GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));

}

void Game::Calculate_Shadows0() {

	//int k = 0, l = 0;
	//for (int i = 0; i < angles.size(); i++) {

	//	if (angles.at(i) >= 2.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if(true){
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//		}
	//	}
	//	else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (true) {
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//		}
	//	}
	//	else if (angles.at(i) <= -2.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (true) {
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//		}
	//	}
	//	else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (true) {
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);

	//		}
	//	}
	//}

}

void Game::Calculate_Shadows1() {

	//std::cout << "Size : " << get_size() << "\n\n";
	//is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy);
	//int index = size_without_shadows;
	//int k = 0, l = 0;

	//for (int i = 0; i < angles.size(); i++) {
	//	if (angles.at(i) >= 2.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

	//			buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//			buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//			buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//		}
	//	}
	//	else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

	//			buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//			buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//		}
	//	}
	//	else if (angles.at(i) <= -2.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

	//			buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//			buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//		}
	//	}
	//	else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

	//			buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//			buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//			buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//		}
	//	}
	//}


}

void Game::Init_Shadow_points()
{
	//shadow_quad_list.clear();
	//int index = get_size();
	//size_without_shadows = get_size();

	//for (auto& e : endingEdges) {

	//	buffer = fill_buffer(buffer, &index, new_position(e.sx, e.sy + 2.0f), new_color(1.0f, 0.0f, 0.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
	//	buffer = fill_buffer(buffer, &index, new_position(e.sx + 2.0f, e.sy + 2.0f), new_color(1.0f, 0.0f, 0.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
	//	buffer = fill_buffer(buffer, &index, new_position(e.sx + 2.0f, e.sy), new_color(1.0f, 0.0f, 0.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
	//	buffer = fill_buffer(buffer, &index, new_position(e.sx, e.sy), new_color(1.0f, 0.0f, 0.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));

	//}

	//set_size(index);
	////std::cout << "Size : " << get_size() << "\n\n";

	//if (index_buffer != NULL) free(index_buffer);
	//index_buffer = make_indecies(get_size());
	//GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));

}

void Game::Calculate_Shadow_points()
{
	//int index = size_without_shadows;

	//for (auto& e : endingEdges) {

	//	buffer[index++].position = new_position(e.sx, e.sy + 2.0f);
	//	buffer[index++].position = new_position(e.sx + 2.0f, e.sy + 2.0f);
	//	buffer[index++].position = new_position(e.sx + 2.0f, e.sy);
	//	buffer[index++].position = new_position(e.sx, e.sy);

	//}

}
