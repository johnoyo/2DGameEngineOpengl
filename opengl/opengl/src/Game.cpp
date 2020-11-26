#include "Game.h"

Game::Game(std::string& level_path, GLFWwindow *win, float width, float height, float character_scale, float refresh_rate) 
	: window(win), refresh_rate(refresh_rate), tile_size(character_scale), m_Camera(0.0f, width, 0.0f, height)

{
	/* TODO: make this better */
	total_buffer_size = (100 * 100) * 4;
	buffer = (struct Vertex_Array*)malloc(total_buffer_size * sizeof(struct Vertex_Array));
	//Load_Menu(width, height, 2.0f);
	index_buffer = make_indecies(get_size());

	//load_level(buffer, level_path, width, height, character_scale);

	handle_opengl();
	
	Init_Transparent_Texture();
	
	texture_slot[1] = LoadTexture("res/textures/factory_tile.png");
	texture_slot[2] = LoadTexture("res/textures/main_menu.png");
	texture_slot[3] = LoadTexture("res/textures/collectible.png");
	texture_slot[4] = LoadTexture("res/textures/enemy.png");
	texture_slot[5] = LoadTexture("res/textures/robot.png");
	texture_slot[6] = LoadTexture("res/textures/factory_bg_6.png");
	texture_slot[7] = LoadTexture("res/textures/robot_reversed.png");
	texture_slot[8] = LoadTexture("res/textures/next_level_button.png");
	texture_slot[9] = LoadTexture("res/textures/win_congrats_screen.png");

	std::cout << texture_slot[0] << ", " << texture_slot[1] << ", " << texture_slot[2] << "\n";

	//p1 = Player(texture_slot[0] - 1, buffer[get_size() - 1].position, character_scale - 1);
	//p1.set_buffer_index(get_size() - 4, get_size() - 3, get_size() - 2, get_size() - 1);
	//std::cout << p1.get_position().x << ", " << p1.get_position().y << "\n";
}

void Game::Init_Transparent_Texture() {

	/* 1x1 whitetexture */
	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &white_texture_id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	uint32_t color = 0xffffffff;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

	texture_slot[0] = white_texture_id;

}

Vertex_Array * Game::fill_buffer(Vertex_Array *vertex, int *index, glm::vec2 new_position, glm::vec4 new_color, glm::vec2 new_tex_coord, float new_tex_id) {
	vertex[*index].position = new_position;
	vertex[*index].color = new_color;
	vertex[*index].tex_coord = new_tex_coord;
	vertex[*index].tex_id = new_tex_id;
	*index = *index + 1;
	return vertex;
}

void Game::Load_Menu(float width, float height, float text_id) {

	int index = 4;

	/* Background data(position, color, texture) gets added first to the vertex buffer */

	buffer = fill_buffer(buffer, &index, new_position(0.0, height), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(text_id));
	buffer = fill_buffer(buffer, &index, new_position(width, height), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(text_id));
	buffer = fill_buffer(buffer, &index, new_position(width, 0.0), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(text_id));
	buffer = fill_buffer(buffer, &index, new_position(0.0, 0.0), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(text_id));

	set_size(index);

	if (index_buffer != NULL) free(index_buffer);
	index_buffer = make_indecies(get_size());
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));

}

void Game::Game_Over(float text_id) {
	custom_sprite_list.clear();
	current_level = 0;
	Load_Menu(945.0f, 540.0f, text_id);

	if (index_buffer != NULL) free(index_buffer);
	index_buffer = make_indecies(get_size());
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));
}



void Game::Load_Next_Level(std::string& level_path, float width, float height, float character_scale) {

	collectible_list.clear();
	enemies_list.clear();

	load_level(buffer, level_path, width, height, character_scale);
	std::cout << "size: " << get_size() << ", total size: " << total_buffer_size << "\n";

	if (index_buffer != NULL) free(index_buffer);
	index_buffer = make_indecies(get_size());

	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));

	p1 = Player(texture_slot[0] - 1, buffer[get_size() - 1].position, character_scale - 1);
	p1.set_buffer_index(get_size() - 4, get_size() - 3, get_size() - 2, get_size() - 1);

	current_level++;

}

Vertex_Array* Game::Make_Custom_Sprite(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, float tex_id) {

	int index = get_size();
	int size = get_size();

	buffer = fill_buffer(buffer, &index, tl, new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(tex_id));
	buffer = fill_buffer(buffer, &index, tr, new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(tex_id));
	buffer = fill_buffer(buffer, &index, br, new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(tex_id));
	buffer = fill_buffer(buffer, &index, bl, new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(tex_id));

	return buffer;

}

void Game::Change_Sprite_Scale(Player sp, float x)
{
	buffer[sp.get_buffer_index()[1]].position.x += x;
	buffer[sp.get_buffer_index()[2]].position.x += x;
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
	char acc[2];
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
			p.push_back({ i,j,0.0f });
		}
		else if (c == 'G') {
			p.push_back({ i,j,1.0f });
		}
		else if (c == 'C')
		{
			p.push_back({ i,j,3.0f });
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
	index = 0;

	/* Background data(position, color, texture) gets added first to the vertex buffer */
	vertex = fill_buffer(vertex, &index, new_position(0.0f, height), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(6.0f));
	vertex = fill_buffer(vertex, &index, new_position(width, height), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(6.0f));
	vertex = fill_buffer(vertex, &index, new_position(width, 0.0f), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(6.0f));
	vertex = fill_buffer(vertex, &index, new_position(0.0f, 0.0f), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(6.0f));
	
	for (int k = 0; k < p.size(); k++) {

		if (k == s) continue;
		/*std::cout << p.at(k).i << "," << p.at(k).j << "\n"; */
		if (p.at(k).k == 3.0f) {

			collectible_list.push_back(Player(3, index, new_position(p.at(k).j* character_scale, p.at(k).i* character_scale)));
		}
		else if (p.at(k).k == 4.0f) {

			enemies_list.push_back(Player(4, index, new_position(p.at(k).j* character_scale, p.at(k).i* character_scale)));
		}
		else if (p.at(k).k == 8.0f) {

			Next_Level = Player(8, index, new_position(p.at(k).j * character_scale, p.at(k).i * character_scale));
		}

		vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, (p.at(k).i + 1) * character_scale), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(p.at(k).k));
		vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, (p.at(k).i + 1) * character_scale), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(p.at(k).k));
		vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, p.at(k).i * character_scale), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(p.at(k).k));
		vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, p.at(k).i * character_scale), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(p.at(k).k));
			
		world[p.at(k).i * w + p.at(k).j].exist = true;
	}

	set_size(index);

	for (int i = 0; i < custom_sprite_list.size(); i++) {
		vertex = Make_Custom_Sprite(custom_sprite_list.at(i).get_custom_position_0(), custom_sprite_list.at(i).get_custom_position_1(),
									custom_sprite_list.at(i).get_custom_position_2(), custom_sprite_list.at(i).get_custom_position_3(), custom_sprite_list.at(i).get_texture_id());

		custom_sprite_list.at(i).set_buffer_index(index, index + 1, index + 2, index + 3);

		index += 4;
	}

	/* Player data (position, color, texture) gets added last to the vertex buffer */

	vertex = fill_buffer(vertex, &index, new_position(p.at(s).j * character_scale - 1, (p.at(s).i + 1) * character_scale - 1), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(5.0f));
	vertex = fill_buffer(vertex, &index, new_position((p.at(s).j + 1) * character_scale - 1, (p.at(s).i + 1) * character_scale - 1), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(5.0f));
	vertex = fill_buffer(vertex, &index, new_position((p.at(s).j + 1) * character_scale - 1, p.at(s).i * character_scale - 1), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(5.0f));
	vertex = fill_buffer(vertex, &index, new_position(p.at(s).j * character_scale - 1, p.at(s).i * character_scale - 1), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(5.0f));


	//for (int k = 0; k < ((p.size() + 1)*4); k++) {
	//	std::cout << vertex[k].position.x << ", " << vertex[k].position.y << "\n";
	//}

	fclose(f);
	set_size(index);
}

unsigned int *Game::make_indecies(int size)
{
	unsigned int* indecies = (unsigned int*)malloc((size / 4) * 6 * sizeof(unsigned int));
	int index = 0, w = 0;
	for (int k = 0; k < (size / 4) * 6; k += 6) {
		indecies[index++] = 0 + w;
		indecies[index++] = 3 + w;
		indecies[index++] = 2 + w;
		indecies[index++] = 2 + w;
		indecies[index++] = 1 + w;
		indecies[index++] = 0 + w;
		w += 4;
	}
	/*
	for (int k = 0; k < (size / 24) * 6; k++) {
		std::cout << indecies[k] << "\n";
	}
	*/
	return indecies;
}

void Game::set_window(GLFWwindow * win)
{
	window = win;
}

GLFWwindow * Game::get_window()
{
	return window;
}

void Game::set_size(unsigned int size) 
{
	buffer_size = size;
}


unsigned int Game::get_size()
{
	return buffer_size;
}

struct Vertex_Array * Game::get_buffer()
{
	return buffer;
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
		buffer[p1.get_buffer_index()[0]].position.x = p1.get_position().x;
		buffer[p1.get_buffer_index()[0]].position.y = p1.get_position().y + p1.get_scale();
			   										  
		buffer[p1.get_buffer_index()[1]].position.x = p1.get_position().x + p1.get_scale();
		buffer[p1.get_buffer_index()[1]].position.y = p1.get_position().y + p1.get_scale();
			   										  
		buffer[p1.get_buffer_index()[2]].position.x = p1.get_position().x + p1.get_scale();
		buffer[p1.get_buffer_index()[2]].position.y = p1.get_position().y;
			   										  
		buffer[p1.get_buffer_index()[3]].position.x = p1.get_position().x;
		buffer[p1.get_buffer_index()[3]].position.y = p1.get_position().y;

		p1.set_teleport(false);
	}
	
	buffer[p1.get_buffer_index()[0]].tex_id = p1.get_texture_id();
	buffer[p1.get_buffer_index()[1]].tex_id = p1.get_texture_id();
	buffer[p1.get_buffer_index()[2]].tex_id = p1.get_texture_id();
	buffer[p1.get_buffer_index()[3]].tex_id = p1.get_texture_id();

	if (Next_Level.get_texture_id() != -11) {
		buffer[Next_Level.get_buffer_index()[0]].position.x = Next_Level.get_position().x;
		buffer[Next_Level.get_buffer_index()[0]].position.y = Next_Level.get_position().y + tile_size;

		buffer[Next_Level.get_buffer_index()[1]].position.x = Next_Level.get_position().x + tile_size;
		buffer[Next_Level.get_buffer_index()[1]].position.y = Next_Level.get_position().y + tile_size;
		   									  
		buffer[Next_Level.get_buffer_index()[2]].position.x = Next_Level.get_position().x + tile_size;
		buffer[Next_Level.get_buffer_index()[2]].position.y = Next_Level.get_position().y;
		   									  
		buffer[Next_Level.get_buffer_index()[3]].position.x = Next_Level.get_position().x;
		buffer[Next_Level.get_buffer_index()[3]].position.y = Next_Level.get_position().y;
	}

	for (int i = 0; i < enemies_list.size(); i++) {
		buffer[enemies_list.at(i).get_buffer_index()[0]].position.x = enemies_list.at(i).get_position().x;
		buffer[enemies_list.at(i).get_buffer_index()[0]].position.y = enemies_list.at(i).get_position().y + tile_size;

		buffer[enemies_list.at(i).get_buffer_index()[1]].position.x = enemies_list.at(i).get_position().x + tile_size;
		buffer[enemies_list.at(i).get_buffer_index()[1]].position.y = enemies_list.at(i).get_position().y + tile_size;

		buffer[enemies_list.at(i).get_buffer_index()[2]].position.x = enemies_list.at(i).get_position().x + tile_size;
		buffer[enemies_list.at(i).get_buffer_index()[2]].position.y = enemies_list.at(i).get_position().y;

		buffer[enemies_list.at(i).get_buffer_index()[3]].position.x = enemies_list.at(i).get_position().x;
		buffer[enemies_list.at(i).get_buffer_index()[3]].position.y = enemies_list.at(i).get_position().y;
	}

	for (int i = 0; i < collectible_list.size(); i++) {
		buffer[collectible_list.at(i).get_buffer_index()[0]].position.x = collectible_list.at(i).get_position().x;
		buffer[collectible_list.at(i).get_buffer_index()[0]].position.y = collectible_list.at(i).get_position().y + tile_size;

		buffer[collectible_list.at(i).get_buffer_index()[1]].position.x = collectible_list.at(i).get_position().x + tile_size;
		buffer[collectible_list.at(i).get_buffer_index()[1]].position.y = collectible_list.at(i).get_position().y + tile_size;

		buffer[collectible_list.at(i).get_buffer_index()[2]].position.x = collectible_list.at(i).get_position().x + tile_size;
		buffer[collectible_list.at(i).get_buffer_index()[2]].position.y = collectible_list.at(i).get_position().y;

		buffer[collectible_list.at(i).get_buffer_index()[3]].position.x = collectible_list.at(i).get_position().x;
		buffer[collectible_list.at(i).get_buffer_index()[3]].position.y = collectible_list.at(i).get_position().y;
	}
	
	/*for (int i = 0; i < custom_sprite_list.size(); i++) {
		
		buffer[custom_sprite_list.at(i).get_buffer_index()[0]].position.x = custom_sprite_list.at(i).get_position().x;
		buffer[custom_sprite_list.at(i).get_buffer_index()[0]].position.y = custom_sprite_list.at(i).get_position().y + tile_size;

		buffer[custom_sprite_list.at(i).get_buffer_index()[1]].position.x = custom_sprite_list.at(i).get_position().x + tile_size;
		buffer[custom_sprite_list.at(i).get_buffer_index()[1]].position.y = custom_sprite_list.at(i).get_position().y + tile_size;

		buffer[custom_sprite_list.at(i).get_buffer_index()[2]].position.x = custom_sprite_list.at(i).get_position().x + tile_size;
		buffer[custom_sprite_list.at(i).get_buffer_index()[2]].position.y = custom_sprite_list.at(i).get_position().y;

		buffer[custom_sprite_list.at(i).get_buffer_index()[3]].position.x = custom_sprite_list.at(i).get_position().x;
		buffer[custom_sprite_list.at(i).get_buffer_index()[3]].position.y = custom_sprite_list.at(i).get_position().y;
	}*/
	
}

void Game::Update_Camera_Uniform() {
	glm::mat4 vp = m_Camera.Get_View_Projection_Matrix();
	GLCall(int location1 = glGetUniformLocation(shader, "u_VP"));
	if (location1 == -1) {
		std::cout << "Uniform not found!!!\n";
	}
	GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(vp)));
}

void Game::render()
{
	//update_buffer();
	Update_Camera_Uniform();

	/* set dynamic vertex buffer */
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, get_size() * sizeof(Vertex_Array), buffer));

	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT);

	GLCall(glBindTextureUnit(0, texture_slot[0]));
	GLCall(glBindTextureUnit(1, texture_slot[1]));
	GLCall(glBindTextureUnit(2, texture_slot[2]));
	GLCall(glBindTextureUnit(3, texture_slot[3]));
	GLCall(glBindTextureUnit(4, texture_slot[4]));
	GLCall(glBindTextureUnit(5, texture_slot[5]));
	GLCall(glBindTextureUnit(6, texture_slot[6]));
	GLCall(glBindTextureUnit(7, texture_slot[7]));
	GLCall(glBindTextureUnit(8, texture_slot[8]));
	GLCall(glBindTextureUnit(9, texture_slot[9]));

	GLCall(glDrawElements(GL_TRIANGLES, (get_size() / 4) * 6, GL_UNSIGNED_INT, NULL));
}

void Game::clean()
{
	GLCall(glDeleteBuffers(1, &vb));
	GLCall(glDeleteBuffers(1, &ib));
	GLCall(glDeleteProgram(shader));
	GLCall(glDeleteVertexArrays(1, &vao));

	GLCall(glDeleteTextures(10, texture_slot));
}


void Game::handle_opengl()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	/* vertex array object */
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	/* vertex buffer */
	GLCall(glGenBuffers(1, &vb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, total_buffer_size * sizeof(struct Vertex_Array), nullptr, GL_DYNAMIC_DRAW));


	/* vertex attrib positions*/
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, position)));

	/* vertex attrib colors*/
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, color)));

	/* vertex attrib texture coordinates*/
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, tex_coord)));

	/* vertex attrib texture id*/
	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, tex_id)));

	/* index buffer */
	GLCall(glGenBuffers(1, &ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));

	/* shaders */
	ShaderProgramSource shaderSource = ParseShader("res/shaders/Basic.shader");
	shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(auto loc = glGetUniformLocation(shader, "u_textures"));
	int samplers[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	GLCall(glUniform1iv(loc, 10, samplers));

	//glm::vec3 translationA(0, 0, 0);
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	//glm::mat4 projection = glm::ortho(0.0f, 945.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	//glm::mat4 mvp = projection * view * model;

	glm::mat4 vp = m_Camera.Get_View_Projection_Matrix();

	GLCall(int location1 = glGetUniformLocation(shader, "u_VP"));
	if (location1 == -1) {
		std::cout << "Uniform not found!!!\n";
	}
	//GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, &mvp[0][0]));
	GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(vp)));
}

void Game::update_player_position(float amount_x, float amount_y)
{
	//p1.change_position(glm::vec2(amount_x, amount_y));

	buffer[p1.get_buffer_index()[0]].position.x += amount_x;
	buffer[p1.get_buffer_index()[0]].position.y += amount_y;
	buffer[p1.get_buffer_index()[1]].position.x += amount_x;
	buffer[p1.get_buffer_index()[1]].position.y += amount_y;
	buffer[p1.get_buffer_index()[2]].position.x += amount_x;
	buffer[p1.get_buffer_index()[2]].position.y += amount_y;
	buffer[p1.get_buffer_index()[3]].position.x += amount_x;
	buffer[p1.get_buffer_index()[3]].position.y += amount_y;

}


void Game::update_player_position_x()
{

	buffer[p1.get_buffer_index()[0]].position.x = p1.get_position().x;
	buffer[p1.get_buffer_index()[1]].position.x = p1.get_position().x + p1.get_scale();
	buffer[p1.get_buffer_index()[2]].position.x = p1.get_position().x + p1.get_scale();
	buffer[p1.get_buffer_index()[3]].position.x = p1.get_position().x;

}

void Game::update_player_position_y()
{

	buffer[p1.get_buffer_index()[0]].position.y = p1.get_position().y + p1.get_scale();
	buffer[p1.get_buffer_index()[1]].position.y = p1.get_position().y + p1.get_scale();
	buffer[p1.get_buffer_index()[2]].position.y = p1.get_position().y;
	buffer[p1.get_buffer_index()[3]].position.y = p1.get_position().y;

}


void Game::handle_collision(float scale_h, float scale_v, float amount_x, float amount_y, unsigned int axis)
{
	/* change the position of the player in the x-axis (i.e last quad in vertex buffer) according to input */
	update_player_position(amount_x, 0.0f);

	/* check if there is collision on x-axis */
	buffer = check_for_collitions(buffer, &p1, size_without_shadows, scale_h, &Is_Grounded_x, &Collides_x, X_AXIS);

	/* change the position of the player in the y-axis (i.e last quad in vertex buffer) according to input */
	update_player_position(0.0f, amount_y);

	/* check if there is collision on y-axis */
	buffer = check_for_collitions(buffer, &p1, size_without_shadows, scale_v, &Is_Grounded_y, &Collides_y, Y_AXIS);

	p1.set_teleport(false);
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
	
	// A.first A.second B.first B.second C.first C.second D.first D.second
	//   x1		y1        x2      y2       x3      y3      x4       y4
	/*float den = ((A.first - B.first) * (C.second - D.second) - (A.second - B.second) * (C.first - D.first));

	if(den == 0) return std::make_pair(FLT_MAX, FLT_MAX);

	float t = ((A.first - C.first) * (C.second - D.second) - (A.second - C.second) * (C.first - D.first)) / den;
	float u = -((A.first - B.first) * (A.second - C.second) - (A.second - B.second) * (A.first - C.first)) / den;

	if(t > 0.0f && t < 1.0f && u >= 0.0f) return std::make_pair(A.first + t * (A.first - B.first), A.second + t * (A.second - B.second));
	else return std::make_pair(FLT_MAX, FLT_MAX);*/

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
	shadow_quad_list.clear();
	int index = get_size();
	size_without_shadows = get_size();
	

	/*int save_index = 0;

	for (int i = 0; i < endingEdges.size(); i++) {
		float min = 10000.0f, prev_min = 10000.0f;
		bool flag1 = false, flag2 = false;
		for (int j = 0; j < endingEdges.size(); j++) {
			if (j != i) {
				if(endingEdges.at(j).sx >= endingEdges.at(i).sx){
					prev_min = min;
					min = find_min(std::fabsf(endingEdges.at(i).theta - endingEdges.at(j).theta), min);
					if (min < prev_min) save_index = j;
				}
			}
		}
		if (endingEdges.at(i).theta > endingEdges.at(save_index).theta) {
			for (int k = 0; k < duo.size(); k++) {
				if ((duo.at(k).x == i && duo.at(k).y == save_index) || i == save_index) flag1 = true;
			}
			if(!flag1) duo.push_back({ (float)i, (float)save_index, -1.0f });
			flag1 = false;
		}
		else {
			for (int k = 0; k < duo.size(); k++) {
				if ((duo.at(k).y == i && duo.at(k).x == save_index) || i == save_index) flag2 = true;
			}
			if (!flag2) duo.push_back({ (float)save_index, (float)i, -1.0f });
			flag2 = false;
		}
	}

	for (int i = 0; i < duo.size(); i++) {

		std::cout << "i,j: " << duo.at(i).x << " ,,, " << duo.at(i).y << "\n";

	}*/

	//for (int i = 0; i < duo.size(); i++) {

	//	//if (p1.get_position().x > endingEdges.at(0).sx) {

	//		shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(duo.at(i).x).ex, endingEdges.at(duo.at(i).x).ey), duo.at(i).x, duo.at(i).y));

	//		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(duo.at(i).y).ex, endingEdges.at(duo.at(i).y).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
	//		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(duo.at(i).y).sx, endingEdges.at(duo.at(i).y).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
	//		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(duo.at(i).x).sx, endingEdges.at(duo.at(i).x).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
	//		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(duo.at(i).x).ex, endingEdges.at(duo.at(i).x).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));

	//	/*}
	//	else {
	//		shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(duo.at(i).x).sx, endingEdges.at(duo.at(i).x).sy), duo.at(i).x, duo.at(i).y));

	//		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(duo.at(i).y).sx, endingEdges.at(duo.at(i).y).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
	//		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(duo.at(i).y).ex, endingEdges.at(duo.at(i).y).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
	//		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(duo.at(i).x).ex, endingEdges.at(duo.at(i).x).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
	//		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(duo.at(i).x).sx, endingEdges.at(duo.at(i).x).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));

	//	}*/

	//}

		/*shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(3).sx, endingEdges.at(3).sy)));

		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(1).sx, endingEdges.at(1).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(1).ex, endingEdges.at(1).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(3).ex, endingEdges.at(3).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(3).sx, endingEdges.at(3).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
		
		shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(0).sx, endingEdges.at(0).sy)));

		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(3).sx, endingEdges.at(3).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(3).ex, endingEdges.at(3).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(0).ex, endingEdges.at(0).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(0).sx, endingEdges.at(0).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));

		shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(2).sx, endingEdges.at(2).sy)));

		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(0).sx, endingEdges.at(0).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(0).ex, endingEdges.at(0).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(2).ex, endingEdges.at(2).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(2).sx, endingEdges.at(2).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
		*/

	int k = 0, l = 0;
	for (int i = 0; i < angles.size(); i++) {
		if (angles.at(i) >= 2.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
				shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), k, l));

				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).sx, endingEdges.at(k).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
			}
		}
		else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
				shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), k, l));

				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).sx, endingEdges.at(k).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
			}
		}
		else if (angles.at(i) <= -2.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
				shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), k, l));

				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).sx, endingEdges.at(k).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
			}
		}
		else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1)%angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
				shadow_quad_list.push_back(Player(0, index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), k, l));

				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).sx, endingEdges.at(l).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).sx, endingEdges.at(k).sy), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(k).ex, endingEdges.at(k).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
				buffer = fill_buffer(buffer, &index, new_position(endingEdges.at(l).ex, endingEdges.at(l).ey), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));
			}
		}
	}


	set_size(index);
	//std::cout << "Size : " << get_size() << "\n\n";

	if (index_buffer != NULL) free(index_buffer);
	index_buffer = make_indecies(get_size());
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));

}

void Game::Calculate_Shadows0() {

	int k = 0, l = 0;
	for (int i = 0; i < angles.size(); i++) {

		if (angles.at(i) >= 2.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if(true){
				buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
			}
		}
		else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (true) {
				buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
			}
		}
		else if (angles.at(i) <= -2.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (true) {
				buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
			}
		}
		else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (true) {
				buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);

			}
		}
	}

}

void Game::Calculate_Shadows1() {

	//std::cout << "Size : " << get_size() << "\n\n";
	//is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy);
	int index = size_without_shadows;
	int k = 0, l = 0;
	//for (int i = 0; i < angles.size(); i++) {
	//	if (angles.at(i) >= 2.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
	//			
	//			buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//			buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//			buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//		}
	//	}
	//	else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
	//			
	//			buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//			buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//		}
	//	}
	//	else if (angles.at(i) <= -2.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
	//			
	//			buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//			buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//		}
	//	}
	//	else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

	//		k = find_edge(endingEdges, angles.at(i));
	//		l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
	//		//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
	//		if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
	//			
	//			buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
	//			buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
	//			buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
	//			buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
	//		}
	//	}
	//}

	for (int i = 0; i < angles.size(); i++) {
		if (angles.at(i) >= 2.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

				buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
			}
		}
		else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

				buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
			}
		}
		else if (angles.at(i) <= -2.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

				buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
			}
		}
		else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

			k = find_edge(endingEdges, angles.at(i));
			l = find_edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

				buffer[index++].position = new_position(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer[index++].position = new_position(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer[index++].position = new_position(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer[index++].position = new_position(endingEdges.at(l).ex, endingEdges.at(l).ey);
			}
		}
	}
	
		/*if (angles.at(0) > 2.0f) {

			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_k_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_k_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_k_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_k_pos()).sy);
			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_l_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_l_pos()).sy);
			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_l_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_l_pos()).ey);

		}
		else if (angles.at(0) < 2.0f && angles.at(0) > 0.0f) {

			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_l_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_l_pos()).sy);
			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_l_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_l_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_k_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_k_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_k_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_k_pos()).sy);

		}
		else if (angles.at(0) < -2.0f) {

			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_l_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_l_pos()).sy);
			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_l_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_l_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_k_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_k_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_k_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_k_pos()).sy);

		}
		else if (angles.at(0) > -2.0f && angles.at(0) < 0.0f) {

			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_l_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_l_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_k_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_k_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_k_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_k_pos()).sy);
			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_l_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_l_pos()).sy);

		}*/
	//}



	//for (int i = 0; i < shadow_quad_list.size(); i++) {

		//if (p1.get_position().x > endingEdges.at(0).sx) {

			/*buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_min_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_min_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_min_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_min_pos()).sy);
			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_max_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_max_pos()).sy);
			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_max_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_max_pos()).ey);*/

		/*}
		else {

			buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_min_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_min_pos()).sy);
			buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_min_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_min_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_max_pos()).ex, endingEdges.at(shadow_quad_list.at(i).get_max_pos()).ey);
			buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = new_position(endingEdges.at(shadow_quad_list.at(i).get_max_pos()).sx, endingEdges.at(shadow_quad_list.at(i).get_max_pos()).sy);

		}*/
	//}

	/*buffer[shadow_quad_list.at(0).get_buffer_index()[0]].position = new_position(endingEdges.at(3).sx, endingEdges.at(3).sy);
	buffer[shadow_quad_list.at(0).get_buffer_index()[1]].position = new_position(endingEdges.at(3).ex, endingEdges.at(3).ey);
	buffer[shadow_quad_list.at(0).get_buffer_index()[2]].position = new_position(endingEdges.at(1).ex, endingEdges.at(1).ey);
	buffer[shadow_quad_list.at(0).get_buffer_index()[3]].position = new_position(endingEdges.at(1).sx, endingEdges.at(1).sy);

	buffer[shadow_quad_list.at(1).get_buffer_index()[0]].position = new_position(endingEdges.at(3).sx, endingEdges.at(3).sy);
	buffer[shadow_quad_list.at(1).get_buffer_index()[1]].position = new_position(endingEdges.at(3).ex, endingEdges.at(3).ey);
	buffer[shadow_quad_list.at(1).get_buffer_index()[2]].position = new_position(endingEdges.at(0).ex, endingEdges.at(0).ey);
	buffer[shadow_quad_list.at(1).get_buffer_index()[3]].position = new_position(endingEdges.at(0).sx, endingEdges.at(0).sy);

	buffer[shadow_quad_list.at(2).get_buffer_index()[0]].position = new_position(endingEdges.at(0).sx, endingEdges.at(0).sy);
	buffer[shadow_quad_list.at(2).get_buffer_index()[1]].position = new_position(endingEdges.at(0).ex, endingEdges.at(0).ey);
	buffer[shadow_quad_list.at(2).get_buffer_index()[2]].position = new_position(endingEdges.at(2).ex, endingEdges.at(2).ey);
	buffer[shadow_quad_list.at(2).get_buffer_index()[3]].position = new_position(endingEdges.at(2).sx, endingEdges.at(2).sy);*/

}

void Game::Init_Shadow_points()
{
	shadow_quad_list.clear();
	int index = get_size();
	size_without_shadows = get_size();

	for (auto& e : endingEdges) {

		buffer = fill_buffer(buffer, &index, new_position(e.sx, e.sy + 2.0f), new_color(1.0f, 0.0f, 0.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(e.sx + 2.0f, e.sy + 2.0f), new_color(1.0f, 0.0f, 0.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(e.sx + 2.0f, e.sy), new_color(1.0f, 0.0f, 0.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
		buffer = fill_buffer(buffer, &index, new_position(e.sx, e.sy), new_color(1.0f, 0.0f, 0.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));

	}

	set_size(index);
	//std::cout << "Size : " << get_size() << "\n\n";

	if (index_buffer != NULL) free(index_buffer);
	index_buffer = make_indecies(get_size());
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));

}

void Game::Calculate_Shadow_points()
{
	int index = size_without_shadows;

	for (auto& e : endingEdges) {

		buffer[index++].position = new_position(e.sx, e.sy + 2.0f);
		buffer[index++].position = new_position(e.sx + 2.0f, e.sy + 2.0f);
		buffer[index++].position = new_position(e.sx + 2.0f, e.sy);
		buffer[index++].position = new_position(e.sx, e.sy);

	}

}

glm::vec2 Game::new_position(float width, float height)
{
	glm::vec2 pos;
	pos.x = width;
	pos.y = height;
	return pos;
}

glm::vec4 Game::new_color(float r, float g, float b, float a)
{
	glm::vec4 col;
	col.r = r;
	col.g = g;
	col.b = b;
	col.a = a;
	return col;
}

glm::vec2 Game::new_tex_coord(float x, float y)
{
	glm::vec2 tex_coord;
	tex_coord.x = x;
	tex_coord.y = y;
	return tex_coord;
}

float Game::new_tex_id(float id)
{
	return id;
}
