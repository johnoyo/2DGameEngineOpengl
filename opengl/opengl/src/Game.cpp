#include "Game.h"

Game::Game(Window& window, float character_scale) 
	: tile_size(character_scale), window(window), m_Camera(0.0f, window.Get_Width(), 0.0f, window.Get_Height())
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
	Load_Menu(window.Get_Width(), window.Get_Height(), text_id);

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

	if (shadows.world != NULL) free(shadows.world);
	shadows.world = (sCell*)malloc(h * w * sizeof(sCell));
	for (int i = 0; i < h * w; i++)
		shadows.world[i].exist = false;

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
			
		shadows.world[p.at(k).i * w + p.at(k).j].exist = true;
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
		buffer.Get_Buffer()[Background.get_buffer_index()[0]].position.y = Background.get_position().y + window.Get_Height();

		buffer.Get_Buffer()[Background.get_buffer_index()[1]].position.x = Background.get_position().x + window.Get_Width();
		buffer.Get_Buffer()[Background.get_buffer_index()[1]].position.y = Background.get_position().y + window.Get_Height();

		buffer.Get_Buffer()[Background.get_buffer_index()[2]].position.x = Background.get_position().x + window.Get_Width();
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
	if (shadows.world != NULL) free(shadows.world);
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
