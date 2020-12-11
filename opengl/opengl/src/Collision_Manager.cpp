#include "Collision_Manager.h"

bool Collision_Manager::check_corner_br_tl(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis)
{
	if (p_y < l_y && p_x > l_x && p_y > l_y1 && p_x < l_x1) {
		//std::cout << "bottom right\n";
		if (axis == X_AXIS) {
			(*p).change_position_x(l_x - p_x);
			change_position_x(*p, vertex);
		}
		else if (axis == Y_AXIS) {
			(*p).change_position_y(l_y - p_y);
			change_position_y(*p, vertex);
		}
		return true;
	}
	return false;
}

bool Collision_Manager::check_corner_tr_bl(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis)
{
	if (p_y > l_y && p_x > l_x && p_y < l_y1 && p_x < l_x1) {
		//std::cout << "top right\n";
		if (axis == X_AXIS) {
			(*p).change_position_x(l_x - p_x);
			change_position_x(*p, vertex);
		}
		else if (axis == Y_AXIS) {
			(*p).change_position_y(l_y - 0.5f - p_y);
			change_position_y(*p, vertex);
		}
		return true;
	}
	return false;
}

bool Collision_Manager::check_corner_tl_br(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis)
{
	if (p_y > l_y && p_x < l_x && p_y < l_y1 && p_x > l_x1) {
		//std::cout << "top left\n";
		if (axis == X_AXIS) {
			(*p).change_position_x(l_x - p_x);
			change_position_x(*p, vertex);
		}
		else if (axis == Y_AXIS) {
			(*p).change_position_y(l_y - 0.5f - p_y);
			change_position_y(*p, vertex);
		}
		return true;
	}
	return false;
}

bool Collision_Manager::check_corner_bl_tr(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis)
{
	if (p_y < l_y && p_x < l_x && p_y > l_y1 && p_x > l_x1) {
		//std::cout << "bottom left\n";
		if (axis == X_AXIS) {
			(*p).change_position_x(l_x - p_x);
			change_position_x(*p, vertex);
		}
		else if (axis == Y_AXIS) {
			(*p).change_position_y(l_y - p_y);
			change_position_y(*p, vertex);
		}
		return true;
	}
	return false;
}


void Collision_Manager::Check_For_Collisions(struct Vertex_Array** vertex, Player* p, int size, bool* Is_Grounded, bool* Collides, int axis) {

	bool tmp;
	*Is_Grounded = false;
	*Collides = true;
	for (int i = 4; i < size - 4; i += 4) {

		tmp = check_corner_br_tl(vertex, p,
			(*vertex)[p->get_buffer_index()[2]].position.x,
			(*vertex)[p->get_buffer_index()[2]].position.y,
			(*vertex)[i + 0].position.x,
			(*vertex)[i + 0].position.y,
			(*vertex)[i + 2].position.x,
			(*vertex)[i + 2].position.y, axis);
		if (tmp != false) {
			*Is_Grounded = true;
			return;
		}

		tmp = check_corner_tr_bl(vertex, p,
			(*vertex)[p->get_buffer_index()[1]].position.x,
			(*vertex)[p->get_buffer_index()[1]].position.y,
			(*vertex)[i + 3].position.x,
			(*vertex)[i + 3].position.y,
			(*vertex)[i + 1].position.x,
			(*vertex)[i + 1].position.y, axis);
		if (tmp != false) {
			return;
		}

		tmp = check_corner_tl_br(vertex, p,
			(*vertex)[p->get_buffer_index()[0]].position.x,
			(*vertex)[p->get_buffer_index()[0]].position.y,
			(*vertex)[i + 2].position.x,
			(*vertex)[i + 2].position.y,
			(*vertex)[i + 0].position.x,
			(*vertex)[i + 0].position.y, axis);
		if (tmp != false) {
			return;
		}

		tmp = check_corner_bl_tr(vertex, p,
			(*vertex)[p->get_buffer_index()[3]].position.x,
			(*vertex)[p->get_buffer_index()[3]].position.y,
			(*vertex)[i + 1].position.x,
			(*vertex)[i + 1].position.y,
			(*vertex)[i + 3].position.x,
			(*vertex)[i + 3].position.y, axis);
		if (tmp != false) {
			*Is_Grounded = true;
			return;
		}
	}
	*Is_Grounded = false;
	*Collides = false;
	return;
}

void Collision_Manager::change_position_x(Player p, struct Vertex_Array** vertex) {

	(*vertex)[p.get_buffer_index()[0]].position.x = p.get_position().x;
	(*vertex)[p.get_buffer_index()[1]].position.x = p.get_position().x + p.get_scale();
	(*vertex)[p.get_buffer_index()[2]].position.x = p.get_position().x + p.get_scale();
	(*vertex)[p.get_buffer_index()[3]].position.x = p.get_position().x;

}

void Collision_Manager::change_position_y(Player p, struct Vertex_Array** vertex) {

	(*vertex)[p.get_buffer_index()[0]].position.y = p.get_position().y + p.get_scale();
	(*vertex)[p.get_buffer_index()[1]].position.y = p.get_position().y + p.get_scale();
	(*vertex)[p.get_buffer_index()[2]].position.y = p.get_position().y;
	(*vertex)[p.get_buffer_index()[3]].position.y = p.get_position().y;

}


inline bool Collision_Manager::check_corner_br_tl_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1) {
	if (p_y <= l_y && p_x >= l_x && p_y >= l_y1 && p_x <= l_x1) return true;
	return false;
}

inline bool Collision_Manager::check_corner_tr_bl_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1) {
	if (p_y >= l_y-0.5f && p_x >= l_x && p_y <= l_y1 && p_x <= l_x1) {
		//std::cout << "--top right--\n";
		return true;
	}
	return false;
}

inline bool Collision_Manager::check_corner_tl_br_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1) {
	if (p_y >= l_y-0.5f && p_x <= l_x && p_y <= l_y1 && p_x >= l_x1) return true;
	return false;
}

inline bool Collision_Manager::check_corner_bl_tr_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1) {
	if (p_y <= l_y && p_x <= l_x && p_y >= l_y1 && p_x >= l_x1) return true;
	return false;
}

bool Collision_Manager::Check_If_Obj_Collides_With_Obj(Player obj1, Player obj2, Vertex_Array* vertex)
{
	bool tmp = false;

	tmp = check_corner_br_tl_bool(vertex[obj1.get_buffer_index()[2]].position.x, 
									vertex[obj1.get_buffer_index()[2]].position.y, 
									vertex[obj2.get_buffer_index()[0]].position.x,
									vertex[obj2.get_buffer_index()[0]].position.y, 
									vertex[obj2.get_buffer_index()[2]].position.x, 
									vertex[obj2.get_buffer_index()[2]].position.y);
	if (tmp) return true;

	tmp = check_corner_tr_bl_bool(vertex[obj1.get_buffer_index()[1]].position.x, 
									vertex[obj1.get_buffer_index()[1]].position.y, 
									vertex[obj2.get_buffer_index()[3]].position.x, 
									vertex[obj2.get_buffer_index()[3]].position.y, 
									vertex[obj2.get_buffer_index()[1]].position.x, 
									vertex[obj2.get_buffer_index()[1]].position.y);
	if (tmp) return true;

	tmp = check_corner_tl_br_bool(vertex[obj1.get_buffer_index()[0]].position.x,
									vertex[obj1.get_buffer_index()[0]].position.y, 
									vertex[obj2.get_buffer_index()[2]].position.x, 
									vertex[obj2.get_buffer_index()[2]].position.y,
									vertex[obj2.get_buffer_index()[0]].position.x, 
									vertex[obj2.get_buffer_index()[0]].position.y);
	if (tmp) return true;

	tmp = check_corner_bl_tr_bool(vertex[obj1.get_buffer_index()[3]].position.x, 
									vertex[obj1.get_buffer_index()[3]].position.y,
									vertex[obj2.get_buffer_index()[1]].position.x,
									vertex[obj2.get_buffer_index()[1]].position.y, 
									vertex[obj2.get_buffer_index()[3]].position.x, 
									vertex[obj2.get_buffer_index()[3]].position.y);
	if (tmp) return true;

	return false;
}





// By copy (old)
//struct Vertex_Array* Collision_Manager::change_position_x(Player p, struct Vertex_Array* vertex) {
//	//std::cout << "Changing position X\n";
//	vertex[p.get_buffer_index()[0]].position.x = p.get_position().x;
//	vertex[p.get_buffer_index()[1]].position.x = p.get_position().x + p.get_scale();
//	vertex[p.get_buffer_index()[2]].position.x = p.get_position().x + p.get_scale();
//	vertex[p.get_buffer_index()[3]].position.x = p.get_position().x;
//	
//	return vertex;
//}
//
//
//struct Vertex_Array* Collision_Manager::change_position_y(Player p, struct Vertex_Array* vertex) {
//	//std::cout << "Changing position Y\n";
//	vertex[p.get_buffer_index()[0]].position.y = p.get_position().y + p.get_scale();
//	vertex[p.get_buffer_index()[1]].position.y = p.get_position().y + p.get_scale();
//	vertex[p.get_buffer_index()[2]].position.y = p.get_position().y;
//	vertex[p.get_buffer_index()[3]].position.y = p.get_position().y;
//	
//	return vertex;
//}
//
//struct Vertex_Array* Collision_Manager::check_corner_br_tl(struct Vertex_Array* vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis)
//{
//	if (p_y < l_y && p_x > l_x && p_y > l_y1 && p_x < l_x1) {
//		//std::cout << "bottom right\n";
//		if (axis == X_AXIS) {
//			(*p).change_position_x(l_x - p_x);
//			return change_position_x(*p, vertex);
//		} else if (axis == Y_AXIS) {
//			(*p).change_position_y(l_y - p_y);
//			return change_position_y(*p, vertex);
//		}
//		assert(0);
//	}
//	return NULL;
//}
//
//struct Vertex_Array* Collision_Manager::check_corner_tr_bl(struct Vertex_Array* vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis)
//{
//	if (p_y > l_y && p_x > l_x && p_y < l_y1 && p_x < l_x1) {
//		//std::cout << "top right\n";
//		if (axis == X_AXIS) {
//			(*p).change_position_x(l_x - p_x);
//			return change_position_x(*p, vertex);
//		} else if (axis == Y_AXIS) {
//			(*p).change_position_y(l_y - 0.5f - p_y);
//			return change_position_y(*p, vertex);
//		}
//		assert(0);
//	}
//	return NULL;
//}
//
//struct Vertex_Array* Collision_Manager::check_corner_tl_br(struct Vertex_Array* vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis)
//{
//	if (p_y > l_y && p_x < l_x && p_y < l_y1 && p_x > l_x1) {
//		//std::cout << "top left\n";
//		if (axis == X_AXIS) {
//			(*p).change_position_x(l_x - p_x);
//			return change_position_x(*p, vertex);
//		} else if (axis == Y_AXIS) {
//			(*p).change_position_y(l_y - 0.5f -p_y);
//			return change_position_y(*p, vertex);
//		}
//		assert(0);
//	}
//	return NULL;
//}
//
//struct Vertex_Array* Collision_Manager::check_corner_bl_tr(struct Vertex_Array* vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis)
//{
//	if (p_y < l_y && p_x < l_x && p_y > l_y1 && p_x > l_x1) {
//		//std::cout << "bottom left\n";
//		if (axis == X_AXIS) {
//			(*p).change_position_x(l_x - p_x);
//			return change_position_x(*p, vertex);
//		} else if (axis == Y_AXIS) {
//			(*p).change_position_y(l_y - p_y);
//			return change_position_y(*p, vertex);
//		}
//		assert(0);
//	}
//	return NULL;
//}
//
//struct Vertex_Array* Collision_Manager::Check_For_Collisions(struct Vertex_Array *vertex, Player *p, int size, bool* Is_Grounded, bool* Collides, int axis) {
//
//	struct Vertex_Array* tmp = NULL;
//	*Is_Grounded = false;
//	*Collides = true;
//	for (int i = 4; i < size - 4; i += 4) {
//
//		tmp = check_corner_br_tl(vertex, p, 
//									vertex[p->get_buffer_index()[2]].position.x, 
//									vertex[p->get_buffer_index()[2]].position.y, 
//									vertex[i + 0].position.x, 
//									vertex[i + 0].position.y, 
//									vertex[i + 2].position.x, 
//									vertex[i + 2].position.y, axis);
//		if (tmp != NULL) { 
//			*Is_Grounded = true;
//			return tmp; 
//		}
//
//		tmp = check_corner_tr_bl(vertex, p, 
//									vertex[p->get_buffer_index()[1]].position.x, 
//									vertex[p->get_buffer_index()[1]].position.y, 
//									vertex[i + 3].position.x, 
//									vertex[i + 3].position.y, 
//									vertex[i + 1].position.x, 
//									vertex[i + 1].position.y, axis);
//		if (tmp != NULL) {
//			return tmp; 
//		}
//
//		tmp = check_corner_tl_br(vertex, p, 
//									vertex[p->get_buffer_index()[0]].position.x, 
//									vertex[p->get_buffer_index()[0]].position.y,
//									vertex[i + 2].position.x, 
//									vertex[i + 2].position.y,
//									vertex[i + 0].position.x, 
//									vertex[i + 0].position.y, axis);
//		if (tmp != NULL) {
//			return tmp; 
//		}
//
//		tmp = check_corner_bl_tr(vertex, p, 
//									vertex[p->get_buffer_index()[3]].position.x, 
//									vertex[p->get_buffer_index()[3]].position.y, 
//									vertex[i + 1].position.x, 
//									vertex[i + 1].position.y, 
//									vertex[i + 3].position.x, 
//									vertex[i + 3].position.y, axis);
//		if (tmp != NULL) {
//			*Is_Grounded = true;
//			return tmp;
//		}
//	}
//	*Is_Grounded = false;
//	*Collides = false;
//	return vertex;
//}