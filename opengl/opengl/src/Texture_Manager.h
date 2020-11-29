#pragma once
#include <vector>
#include <string>
#include "Renderer2D.h"
#include "stb_image\stb_image.h"

class Texture_Manager {
public:
	unsigned int current_index = 0;
	std::vector<std::string> map;
	void Init_Transparent_Texture(Renderer2D& renderer);
	void Load_Texture(Renderer2D &renderer, const std::string &path);
	float Find(const std::string& path);

};