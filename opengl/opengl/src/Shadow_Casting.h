#pragma once
#include "Renderer.h"
#include "Vertex_Buffer.h"
#include "Index_Buffer.h"
#include "Renderer2D.h"
#include <vector>
#include "Player.h"

class Shadow_Casting {
public:
	sCell *world = NULL;

	std::vector<sEdge> vecEdges;
	std::vector<sEdge> endingEdges;

	std::vector<glm::vec2> edges;
	std::vector<float> angles;
	std::vector<first_col> duo;

	int size_without_shadows;

	/*
		Converts the individual quads to polygons
		@sx : starting x
		@sy : starting y
		@w  : how many tiles exist in the width of the world
		@h  : how many tiles exist int the height of the world
		@fBlockWidth: the width of an individual tile
		@pitch : the width of the world in tile size(same as @w)
	*/
	void convert_quads_to_polygons(int sx, int sy, int w, int h, float fBlockWidth, int pitch);

	std::pdd lineLineIntersection0(std::pdd A, std::pdd B, std::pdd C, std::pdd D);
	std::pdd lineLineIntersection1(std::pdd A, std::pdd B, std::pdd C, std::pdd D);
	bool lineLineIntersection2(std::pdd A, std::pdd B, std::pdd C, std::pdd D);
	bool LineLineIntersect(std::pdd A, std::pdd B, std::pdd C, std::pdd D, float& ixOut, float& iyOut);

	void CalculateVisibilityPolygon(float ox, float oy, float radius);
	void Find_Min_Points(std::vector<first_col> c, float ox, float oy, float* i_x, float* i_y);
	float Find_Min(float a, float b);
	int Find_Edge(std::vector<sEdge> c, float angle);

	void Init_Shadows(Vertex_Buffer& buffer, Index_Buffer& index_buffer, Renderer2D& renderer, std::vector<Player>& shadow_quad_list);
	void Calculate_Shadows0(Vertex_Buffer& buffer, std::vector<Player>& shadow_quad_list);
	void Calculate_Shadows1(Vertex_Buffer& buffer);

private:

};
