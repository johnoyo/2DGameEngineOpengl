#pragma once
#include "Renderer.h"
#include "Player.h"
#include "Vertex_Buffer.h"

class Line {
public:
	Line() {}
	Line(float x1, float y1, float x2, float y2);
	bool Intersects(Line b);
	bool Intersects(Player p, Vertex_Buffer buffer);
	std::pdd P1;
	std::pdd P2;
private:
	bool LineLineIntersect(std::pdd A, std::pdd B, std::pdd C, std::pdd D, float& ixOut, float& iyOut);
	std::pdd lineLineIntersection1(std::pdd A, std::pdd B, std::pdd C, std::pdd D);
	inline float Det(float a, float b, float c, float d);

};


