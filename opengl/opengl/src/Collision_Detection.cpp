#include "Collision_Detection.h"

struct Vertex_Array* change_position_x(Player *p, struct Vertex_Array* vertex, int size, float amount_x, float player_scale) {
	/* std::cout << "---" << amount_x << " " << player_scale << "---\n"; */
	std::cout << "Changing position X\n";
	if (player_scale < 0) {
		vertex[p->get_buffer_index()[0]].position.x = amount_x + player_scale;
		vertex[p->get_buffer_index()[1]].position.x = amount_x;
		vertex[p->get_buffer_index()[2]].position.x = amount_x;
		vertex[p->get_buffer_index()[3]].position.x = amount_x + player_scale;
	}
	else if (player_scale > 0) {
		vertex[p->get_buffer_index()[0]].position.x = amount_x;
		vertex[p->get_buffer_index()[1]].position.x = amount_x + player_scale;
		vertex[p->get_buffer_index()[2]].position.x = amount_x + player_scale;
		vertex[p->get_buffer_index()[3]].position.x = amount_x;
	}
	p->fix_position({ vertex[p->get_buffer_index()[3]].position.x, vertex[p->get_buffer_index()[3]].position.y });
	return vertex;
}

struct Vertex_Array* change_position_y(Player *p, struct Vertex_Array* vertex, int size, float amount_y, float player_scale) {
	std::cout << "Changing position Y\n";
	if (player_scale < 0) {
		vertex[p->get_buffer_index()[0]].position.y = amount_y;
		vertex[p->get_buffer_index()[1]].position.y = amount_y;
		vertex[p->get_buffer_index()[2]].position.y = amount_y + player_scale;
		vertex[p->get_buffer_index()[3]].position.y = amount_y + player_scale;
	}
	else if (player_scale > 0) {
		vertex[p->get_buffer_index()[0]].position.y = amount_y + player_scale;
		vertex[p->get_buffer_index()[1]].position.y = amount_y + player_scale;
		vertex[p->get_buffer_index()[2]].position.y = amount_y;
		vertex[p->get_buffer_index()[3]].position.y = amount_y;
	}
	p->fix_position({ vertex[p->get_buffer_index()[3]].position.x, vertex[p->get_buffer_index()[3]].position.y });
	return vertex;
}

struct Vertex_Array* change_positions(Player *p, struct Vertex_Array* vertex, int size, float amount_x, float amount_y) {
	vertex[p->get_buffer_index()[0]].position.x += amount_x;
	vertex[p->get_buffer_index()[0]].position.y += amount_y;
	vertex[p->get_buffer_index()[1]].position.x += amount_x;
	vertex[p->get_buffer_index()[1]].position.y += amount_y;
	vertex[p->get_buffer_index()[2]].position.x += amount_x;
	vertex[p->get_buffer_index()[2]].position.y += amount_y;
	vertex[p->get_buffer_index()[3]].position.x += amount_x;
	vertex[p->get_buffer_index()[3]].position.y += amount_y;
	//p->fix_position({ vertex[p->get_buffer_index()[3]].position.x, vertex[p->get_buffer_index()[3]].position.y });
	return vertex;
}

struct Vertex_Array* check_corner_br_tl(Player *p, struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis) {
	if (p_y < l_y && p_x > l_x && p_y > l_y1 && p_x < l_x1) {
		//std::cout << "bottom right\n";
		if (axis == X_AXIS) vertex = change_position_x(p, vertex, size, l_x, player_scale);
		else if (axis == Y_AXIS) vertex = change_position_y(p, vertex, size, l_y, player_scale);
		return vertex;
	}
	return NULL;
}

struct Vertex_Array* check_corner_tr_bl(Player *p, struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis) {
	if (p_y > l_y && p_x > l_x && p_y < l_y1 && p_x < l_x1) {
		//std::cout << "top right\n";
		if (axis == X_AXIS) vertex = change_position_x(p, vertex, size, l_x, player_scale);
		else if (axis == Y_AXIS) vertex = change_position_y(p, vertex, size, l_y, player_scale);
		return vertex;
	}
	return NULL;
}

struct Vertex_Array* check_corner_tl_br(Player *p, struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis) {
	if (p_y > l_y && p_x < l_x && p_y < l_y1 && p_x > l_x1) {
		//std::cout << "top left\n";
		if (axis == X_AXIS) vertex = change_position_x(p, vertex, size, l_x, player_scale);
		else if (axis == Y_AXIS) vertex = change_position_y(p, vertex, size, l_y, player_scale);
		return vertex;
	}
	return NULL;
}

struct Vertex_Array* check_corner_bl_tr(Player *p, struct Vertex_Array* vertex, int size, float player_scale, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis) {
	if (p_y < l_y && p_x < l_x && p_y > l_y1 && p_x > l_x1) {
		//std::cout << "bottom left\n";
		if (axis == X_AXIS) vertex = change_position_x(p, vertex, size, l_x, player_scale);
		else if (axis == Y_AXIS) vertex = change_position_y(p, vertex, size, l_y, player_scale);
		return vertex;
	}
	return NULL;
}


struct Vertex_Array* check_for_collitions(struct Vertex_Array* vertex, Player *p, int size, float player_scale, bool* is_grounded, bool* collides, int axis) {
	struct Vertex_Array* tmp;
	*is_grounded = false;
	*collides = true;
	for (int i = 4; i < size - 4; i += 4) {

		tmp = check_corner_br_tl(p, vertex, size, player_scale, 
			vertex[p->get_buffer_index()[2]].position.x, 
			vertex[p->get_buffer_index()[2]].position.y, 
			vertex[i + 0].position.x, 
			vertex[i + 0].position.y, 
			vertex[i + 2].position.x, 
			vertex[i + 2].position.y, axis);
		if (tmp != NULL) { *is_grounded = true; return tmp; }

		tmp = check_corner_tr_bl(p, vertex, size, player_scale,
			vertex[p->get_buffer_index()[1]].position.x, 
			vertex[p->get_buffer_index()[1]].position.y, 
			vertex[i + 3].position.x, 
			vertex[i + 3].position.y, 
			vertex[i + 1].position.x, 
			vertex[i + 1].position.y, axis);
		if (tmp != NULL) { return tmp; }

		tmp = check_corner_tl_br(p, vertex, size, player_scale, 
			vertex[p->get_buffer_index()[0]].position.x, 
			vertex[p->get_buffer_index()[0]].position.y,
			vertex[i + 2].position.x, 
			vertex[i + 2].position.y, 
			vertex[i + 0].position.x, 
			vertex[i + 0].position.y, axis);
		if (tmp != NULL) { return tmp; }

		tmp = check_corner_bl_tr(p, vertex, size, player_scale, 
			vertex[p->get_buffer_index()[3]].position.x, 
			vertex[p->get_buffer_index()[3]].position.y, 
			vertex[i + 1].position.x, 
			vertex[i + 1].position.y, 
			vertex[i + 3].position.x, 
			vertex[i + 3].position.y, axis);
		if (tmp != NULL) { *is_grounded = true; return tmp; }
	}
	*is_grounded = false;
	*collides = false;
	//p->fix_position({ vertex[p->get_buffer_index()[3]].position.x, vertex[p->get_buffer_index()[3]].position.y });
	return vertex;
}

bool check_corner_br_tl_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1) {
	if (p_y <= l_y && p_x >= l_x && p_y >= l_y1 && p_x <= l_x1) return true;
	return false;
}

bool check_corner_tr_bl_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1) {
	if (p_y >= l_y && p_x >= l_x && p_y <= l_y1 && p_x <= l_x1) {
		//std::cout << "--top right--\n";
		return true;
	}
	return false;
}

bool check_corner_tl_br_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1) {
	if (p_y >= l_y && p_x <= l_x && p_y <= l_y1 && p_x >= l_x1) return true;
	return false;
}

bool check_corner_bl_tr_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1) {
	if (p_y <= l_y && p_x <= l_x && p_y >= l_y1 && p_x >= l_x1) return true;
	return false;
}

bool check_if_obj_collides_with_obj(Player obj1, Player obj2, Vertex_Array* vertex, int size)
{
	bool tmp = false;

	tmp = check_corner_br_tl_bool(vertex[obj1.get_buffer_index()[2]].position.x, vertex[obj1.get_buffer_index()[2]].position.y, vertex[obj2.get_buffer_index()[0]].position.x, vertex[obj2.get_buffer_index()[0]].position.y, vertex[obj2.get_buffer_index()[2]].position.x, vertex[obj2.get_buffer_index()[2]].position.y);
	if (tmp) return true;

	tmp = check_corner_tr_bl_bool(vertex[obj1.get_buffer_index()[1]].position.x, vertex[obj1.get_buffer_index()[1]].position.y, vertex[obj2.get_buffer_index()[3]].position.x, vertex[obj2.get_buffer_index()[3]].position.y, vertex[obj2.get_buffer_index()[1]].position.x, vertex[obj2.get_buffer_index()[1]].position.y);
	if (tmp) return true;

	tmp = check_corner_tl_br_bool(vertex[obj1.get_buffer_index()[0]].position.x, vertex[obj1.get_buffer_index()[0]].position.y, vertex[obj2.get_buffer_index()[2]].position.x, vertex[obj2.get_buffer_index()[2]].position.y, vertex[obj2.get_buffer_index()[0]].position.x, vertex[obj2.get_buffer_index()[0]].position.y);
	if (tmp) return true;

	tmp = check_corner_bl_tr_bool(vertex[obj1.get_buffer_index()[3]].position.x, vertex[obj1.get_buffer_index()[3]].position.y, vertex[obj2.get_buffer_index()[1]].position.x, vertex[obj2.get_buffer_index()[1]].position.y, vertex[obj2.get_buffer_index()[3]].position.x, vertex[obj2.get_buffer_index()[3]].position.y);
	if (tmp) return true;

	return false;
}

