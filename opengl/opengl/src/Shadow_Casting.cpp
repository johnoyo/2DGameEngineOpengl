#include "Shadow_Casting.h"

void Shadow_Casting::convert_quads_to_polygons(int sx, int sy, int w, int h, float fBlockWidth, int pitch)
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

std::pdd Shadow_Casting::lineLineIntersection0(std::pdd A, std::pdd B, std::pdd C, std::pdd D)
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

std::pdd Shadow_Casting::lineLineIntersection1(std::pdd A, std::pdd B, std::pdd C, std::pdd D)
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

bool Shadow_Casting::lineLineIntersection2(std::pdd A, std::pdd B, std::pdd C, std::pdd D)
{
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

inline float Det(float a, float b, float c, float d)
{
	return a * d - b * c;
}

bool Shadow_Casting::LineLineIntersect(std::pdd A, std::pdd B, std::pdd C, std::pdd D, float & ixOut, float & iyOut)
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

float Shadow_Casting::Find_Min(float a, float b) {
	return (b < a) ? b : a;
}

void Shadow_Casting::Find_Min_Points(std::vector<first_col> c, float ox, float oy, float* i_x, float* i_y) {

	float min = 10000.0f, prev_min = 10000.0f;
	int keep_index = 0;

	if (c.size() == 0) return;

	for (int i = 0; i < c.size(); i++) {
		prev_min = min;
		min = Find_Min(c.at(i).xy, min);
		if (min < prev_min) keep_index = i;
	}
	//std::cout << c.size() << ", " << keep_index << "\n";
	*i_x = c.at(keep_index).x;
	*i_y = c.at(keep_index).y;

}

void Shadow_Casting::CalculateVisibilityPolygon(float ox, float oy, float radius)
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
		Find_Min_Points(cols, ox, oy, &ii_x, &ii_y);
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
		if (counter0 == 0 || counter1 == 0)
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

int Shadow_Casting::Find_Edge(std::vector<sEdge> c, float angle) {
	for (int i = 0; i < c.size(); i++) {
		if (c.at(i).theta == angle) return i;
	}
}

void Shadow_Casting::Init_Shadows(Vertex_Buffer& buffer, Index_Buffer& index_buffer, Renderer2D& renderer, std::vector<Player>& shadow_quad_list)
{
	shadow_quad_list.clear();
	int index = buffer.Get_Size();
	/* NOTE: put this size in collision detecting function to work properly */
	size_without_shadows = buffer.Get_Size();

	int k = 0, l = 0;
	for (int i = 0; i < angles.size(); i++) {
		if (angles.at(i) >= 2.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
				shadow_quad_list.push_back(Player(0, index, glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey), k, l));

				buffer.Fill_Buffer(glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0.0f);
			}
		}
		else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
				shadow_quad_list.push_back(Player(0, index, glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy), k, l));

				buffer.Fill_Buffer(glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0.0f);
			}
		}
		else if (angles.at(i) <= -2.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
				shadow_quad_list.push_back(Player(0, index, glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy), k, l));

				buffer.Fill_Buffer(glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0.0f);
			}
		}
		else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1)%angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {
				shadow_quad_list.push_back(Player(0, index, glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey), k, l));

				buffer.Fill_Buffer(glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0.0f);
				buffer.Fill_Buffer(glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0.0f);
			}
		}
	}

	index_buffer.Clean();
	index_buffer.Make_Indecies(buffer.Get_Size());
	renderer.Upadte_Index_Buffer(buffer.Get_Size(), index_buffer);

}


void Shadow_Casting::Calculate_Shadows0(Vertex_Buffer& buffer, std::vector<Player>& shadow_quad_list) {

	int k = 0, l = 0;
	for (int i = 0; i < angles.size(); i++) {

		if (angles.at(i) >= 2.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if(true){
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey);
			}
		}
		else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (true) {
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy);
				
			}
		}
		else if (angles.at(i) <= -2.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (true) {
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy);
			}
		}
		else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (true) {
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[0]].position = glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[1]].position = glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[2]].position = glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer.buffer[shadow_quad_list.at(i).get_buffer_index()[3]].position = glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy);

			}
		}
	}

}

void Shadow_Casting::Calculate_Shadows1(Vertex_Buffer& buffer) {

	//std::cout << "Size : " << get_size() << "\n\n";
	//is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy);
	int index = size_without_shadows;
	int k = 0, l = 0;

	for (int i = 0; i < angles.size(); i++) {
		if (angles.at(i) >= 2.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

				buffer.buffer[index++].position = glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey);
			}
		}
		else if (angles.at(i) <= 2.0f && angles.at(i) >= 0.0f) {
			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

				buffer.buffer[index++].position = glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy);
			}
		}
		else if (angles.at(i) <= -2.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

				buffer.buffer[index++].position = glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy);
			}
		}
		else if (angles.at(i) >= -2.0f && angles.at(i) <= 0.0f) {

			k = Find_Edge(endingEdges, angles.at(i));
			l = Find_Edge(endingEdges, angles.at((i + 1) % angles.size()));
			//if (is_Edge_Connected(endingEdges.at(k).sx, endingEdges.at(k).sy, endingEdges.at(l).sx, endingEdges.at(l).sy)) {
			if (endingEdges.at(k).is_external == false || endingEdges.at(l).is_external == false) {

				buffer.buffer[index++].position = glm::vec2(endingEdges.at(l).sx, endingEdges.at(l).sy);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(k).sx, endingEdges.at(k).sy);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(k).ex, endingEdges.at(k).ey);
				buffer.buffer[index++].position = glm::vec2(endingEdges.at(l).ex, endingEdges.at(l).ey);
			}
		}
	}


}
