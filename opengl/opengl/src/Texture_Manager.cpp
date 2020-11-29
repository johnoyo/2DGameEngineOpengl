#include "Texture_Manager.h"

void Texture_Manager::Load_Texture(Renderer2D &renderer, const std::string& path)
{
	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
	unsigned int tex_id;

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &tex_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, tex_id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

	stbi_image_free(pixels);

	map.push_back(path);
	renderer.Get_Texture_Slot()[current_index++] = tex_id;
	if (current_index == 31) std::cout << "NOTE: You have reached the max amount of textures(32)\n";
	assert(current_index < 32);
}

float Texture_Manager::Find(const std::string &path)
{
	for (int i = 0; i < map.size(); i++) {
		if (path == map.at(i)) return (float)i;
	}
	std::cerr << "Error! Could not find the file specified." << "\n";
	return (float)0;
}

void Texture_Manager::Init_Transparent_Texture(Renderer2D& renderer)
{
	unsigned int white_texture_id;

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &white_texture_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, white_texture_id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	uint32_t color = 0xffffffff;
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

	std::string str = "Transparent_Texture";
	map.push_back(str);
	renderer.Get_Texture_Slot()[current_index++] = white_texture_id;
}
