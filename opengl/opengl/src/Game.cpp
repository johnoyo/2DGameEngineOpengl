#include "Game.h"

Game::Game(std::string& level_path, GLFWwindow *win, float width, float height, float character_scale, float refresh_rate) : window(win), refresh_rate(refresh_rate), tile_size(character_scale)
{

	total_buffer_size = (width / character_scale * height / character_scale);
	buffer = (struct Vertex_Array*)malloc(total_buffer_size * sizeof(struct Vertex_Array));
	buffer = load_level(buffer, level_path, width, height, character_scale);
	index_buffer = make_indecies(get_size());

	handle_opengl();

	texture_slot[0] = LoadTexture("res/textures/Pixel-Art.png");
	texture_slot[1] = LoadTexture("res/textures/BrickPreview.png");
	texture_slot[2] = LoadTexture("res/textures/DT_LeaSeydoux.png");
	texture_slot[3] = LoadTexture("res/textures/collectible-nobg.png");
	texture_slot[4] = LoadTexture("res/textures/enemy.png");

	std::cout << texture_slot[0] << ", " << texture_slot[1] << ", " << texture_slot[2] << "\n";

	p1 = Player(texture_slot[0] - 1, buffer[get_size() - 1].position, character_scale - 1);
	p1.set_buffer_index(get_size() - 4, get_size() - 3, get_size() - 2, get_size() - 1);
	std::cout << p1.get_position().x << ", " << p1.get_position().y << "\n";
}

void Game::Load_Next_Level(std::string& level_path, float width, float height, float character_scale) {

	collectible_list.clear();
	enemies_list.clear();

	buffer = load_level(buffer, level_path, width, height, character_scale);
	index_buffer = make_indecies(get_size());

	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (get_size() / 4) * 6 * sizeof(unsigned int), index_buffer, GL_STATIC_DRAW));

	p1 = Player(texture_slot[0] - 1, buffer[get_size() - 1].position, character_scale - 1);
	p1.set_buffer_index(get_size() - 4, get_size() - 3, get_size() - 2, get_size() - 1);

}

Game::~Game()
{
}

Vertex_Array * Game::fill_buffer(Vertex_Array *vertex, int *index, glm::vec2 new_position, glm::vec4 new_color, glm::vec2 new_tex_coord, float new_tex_id) {
	vertex[*index].position = new_position;
	vertex[*index].color = new_color;
	vertex[*index].tex_coord = new_tex_coord;
	vertex[*index].tex_id = new_tex_id;
	*index = *index + 1;
	return vertex;
}

Vertex_Array * Game::load_level(Vertex_Array* vertex, std::string & level_path, float width, float height, float character_scale)
{
	FILE* f;
	fopen_s(&f, level_path.c_str(), "r");

	if (!f) return {};
	char c;
	int i = (height / character_scale) - 1, j = 0, s = 0;

	struct pos {
		int i;
		int j;
		float k;
	};

	std::vector<pos> p;
	while ((c = fgetc(f)) != EOF) {
		if (c == 'B') {
			p.push_back({ i,j,1.0 });
		}
		else if (c == 'G') {
			p.push_back({ i,j,2.0 });
		}
		else if (c == 'C')
		{
			p.push_back({ i,j,3.0 });
		}
		else if (c == 'E')
		{
			p.push_back({ i,j,4.0 });
		}
		else if (c == 'P')
		{
			p.push_back({ i,j,0.0 });
			s = p.size() - 1;
		}
		if (j == int(width / character_scale)) {

			j = 0;
			i--;
			continue;
		}
		j++;
	}

	//struct Vertex_Array *vertex = (struct Vertex_Array *)malloc(((p.size() + 1) * 4) * sizeof(struct Vertex_Array));
	int index = 0;

	/* Background data(position, color, texture) gets added first to the vertex buffer */
	vertex = fill_buffer(vertex, &index, new_position(0.0, height), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(2.0f));
	vertex = fill_buffer(vertex, &index, new_position(width, height), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(2.0f));
	vertex = fill_buffer(vertex, &index, new_position(width, 0.0), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(2.0f));
	vertex = fill_buffer(vertex, &index, new_position(0.0, 0.0), new_color(1.0f, 1.0f, 1.0f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(2.0f));
	
	for (int k = 0; k < p.size(); k++) {
		if (k == s) continue;
		/*std::cout << p.at(k).i << "," << p.at(k).j << "\n"; */

		if (p.at(k).k == 1.0) {

			vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, (p.at(k).i + 1) * character_scale), new_color(0.18f, 0.6f, 0.96f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(1.0f));
			vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, (p.at(k).i + 1) * character_scale), new_color(0.18f, 0.6f, 0.96f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(1.0f));
			vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, p.at(k).i * character_scale), new_color(0.18f, 0.6f, 0.96f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(1.0f));
			vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, p.at(k).i * character_scale), new_color(0.18f, 0.6f, 0.96f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(1.0f));
			
		}
		else if (p.at(k).k == 2.0f) {

			vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, (p.at(k).i + 1) * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(1.0f));
			vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, (p.at(k).i + 1) * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(1.0f));
			vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, p.at(k).i * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(1.0f));
			vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, p.at(k).i * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(1.0f));

		}
		else if (p.at(k).k == 3.0f) {

			collectible_list.push_back(Player(3, index, new_position(p.at(k).j* character_scale, p.at(k).i* character_scale)));
			vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, (p.at(k).i + 1) * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(3.0f));
			vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, (p.at(k).i + 1) * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(3.0f));
			vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, p.at(k).i * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(3.0f));
			vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, p.at(k).i * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(3.0f));

		}
		else if (p.at(k).k == 4.0f) {

			enemies_list.push_back(Player(4, index, new_position(p.at(k).j* character_scale, p.at(k).i* character_scale)));
			vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, (p.at(k).i + 1) * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(4.0f));
			vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, (p.at(k).i + 1) * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(4.0f));
			vertex = fill_buffer(vertex, &index, new_position((p.at(k).j + 1) * character_scale, p.at(k).i * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(4.0f));
			vertex = fill_buffer(vertex, &index, new_position(p.at(k).j * character_scale, p.at(k).i * character_scale), new_color(1.0f, 0.93f, 0.24f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(4.0f));

		}
	}

	/* Player data (position, color, texture) gets added last to the vertex buffer */

	vertex = fill_buffer(vertex, &index, new_position(p.at(s).j * character_scale - 1, (p.at(s).i + 1) * character_scale - 1), new_color(1.0f, 0.3f, 0.8f, 1.0f), new_tex_coord(0.0f, 1.0f), new_tex_id(0.0f));
	vertex = fill_buffer(vertex, &index, new_position((p.at(s).j + 1) * character_scale - 1, (p.at(s).i + 1) * character_scale - 1), new_color(1.0f, 0.3f, 0.8f, 1.0f), new_tex_coord(1.0f, 1.0f), new_tex_id(0.0f));
	vertex = fill_buffer(vertex, &index, new_position((p.at(s).j + 1) * character_scale - 1, p.at(s).i * character_scale - 1), new_color(1.0f, 0.3f, 0.8f, 1.0f), new_tex_coord(1.0f, 0.0f), new_tex_id(0.0f));
	vertex = fill_buffer(vertex, &index, new_position(p.at(s).j * character_scale - 1, p.at(s).i * character_scale - 1), new_color(1.0f, 0.3f, 0.8f, 1.0f), new_tex_coord(0.0f, 0.0f), new_tex_id(0.0f));

	//for (int k = 0; k < ((p.size() + 1)*4); k++) {
	//	std::cout << vertex[k].position.x << ", " << vertex[k].position.y << "\n";
	//}

	fclose(f);
	set_size((p.size() + 1) * 4);
	return vertex;
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



void Game::render()
{
	update_buffer();

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

	GLCall(glDrawElements(GL_TRIANGLES, (get_size() / 4) * 6, GL_UNSIGNED_INT, NULL));
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
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void *)offsetof(Vertex_Array, color)));

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
	int samplers[5] = { 0, 1, 2, 3, 4 };
	GLCall(glUniform1iv(loc, 5, samplers));

	glm::vec3 translationA(0, 0, 0);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 projection = glm::ortho(0.0f, 945.0f, 0.0f, 540.0f, -1.0f, 1.0f);

	glm::mat4 mvp = projection * view * model;

	GLCall(int location1 = glGetUniformLocation(shader, "u_MVP"));
	if (location1 == -1) {
		std::cout << "Uniform not found!!!\n";
	}
	GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, &mvp[0][0]));
}

void Game::handle_collision(float scale_h, float scale_v, float amount_x, float amount_y, unsigned int axis)
{
	/* change the position of the player in the x-axis (i.e last quad in vertex buffer) according to input */
	update_player_position(amount_x, 0.0f);
	//update_player_position_x();

	/* check if there is collision on x-axis */
	buffer = check_for_collitions(buffer, p1, get_size(), scale_h, &Is_Grounded_x, &Collides_x, X_AXIS);

	/* change the position of the player in the y-axis (i.e last quad in vertex buffer) according to input */
	update_player_position(0.0f, amount_y);
	//update_player_position_y();

	/* check if there is collision on y-axis */
	buffer = check_for_collitions(buffer, p1, get_size(), scale_v, &Is_Grounded_y, &Collides_y, Y_AXIS);
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
