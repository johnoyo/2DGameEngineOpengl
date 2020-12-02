#include "Line.h"

Line::Line(float x1, float y1, float x2, float y2) : P1(std::make_pair(x1, y1)), P2(std::make_pair(x2,y2))
{
}

inline float Line::Det(float a, float b, float c, float d) { return a * d - b * c; }

bool Line::LineLineIntersect(std::pdd A, std::pdd B, std::pdd C, std::pdd D, float& ixOut, float& iyOut) //Output 
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

std::pdd Line::lineLineIntersection1(std::pdd A, std::pdd B, std::pdd C, std::pdd D)
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

bool Line::Intersects(Line b)
{
	float x, y;
	std::pdd res = lineLineIntersection1(P1, P2, b.P1, b.P2);
	if (res.first != FLT_MAX && res.second != FLT_MAX) return true;
	else return false;
}

bool Line::Intersects(Player p, Vertex_Buffer buffer)
{
	bool res0 = false, res1 = false, res2 = false, res3 = false;
	Line tmp0 = Line(buffer.Get_Buffer()[p.get_buffer_index()[0]].position.x, buffer.Get_Buffer()[p.get_buffer_index()[0]].position.y, buffer.Get_Buffer()[p.get_buffer_index()[1]].position.x, buffer.Get_Buffer()[p.get_buffer_index()[1]].position.y);
	Line tmp1 = Line(buffer.Get_Buffer()[p.get_buffer_index()[1]].position.x, buffer.Get_Buffer()[p.get_buffer_index()[1]].position.y, buffer.Get_Buffer()[p.get_buffer_index()[2]].position.x, buffer.Get_Buffer()[p.get_buffer_index()[2]].position.y);
	Line tmp2 = Line(buffer.Get_Buffer()[p.get_buffer_index()[2]].position.x, buffer.Get_Buffer()[p.get_buffer_index()[2]].position.y, buffer.Get_Buffer()[p.get_buffer_index()[3]].position.x, buffer.Get_Buffer()[p.get_buffer_index()[3]].position.y);
	Line tmp3 = Line(buffer.Get_Buffer()[p.get_buffer_index()[3]].position.x, buffer.Get_Buffer()[p.get_buffer_index()[3]].position.y, buffer.Get_Buffer()[p.get_buffer_index()[0]].position.x, buffer.Get_Buffer()[p.get_buffer_index()[0]].position.y);

	res0 = Intersects(tmp0);
	res1 = Intersects(tmp1);
	res2 = Intersects(tmp2);
	res3 = Intersects(tmp3);

	return (res0 || res1 || res2 || res3);
}
