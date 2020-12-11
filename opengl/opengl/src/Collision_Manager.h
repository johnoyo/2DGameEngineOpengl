#pragma once
#include "Renderer.h"
#include "Player.h"


class Collision_Manager {
public:
	void Check_For_Collisions(struct Vertex_Array** vertex, Player* p, int size, bool* Is_Grounded, bool* Collides, int axis);
	bool Check_If_Obj_Collides_With_Obj(Player obj1, Player obj2, Vertex_Array* vertex);

private:
	bool check_corner_br_tl(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
	bool check_corner_tr_bl(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
	bool check_corner_tl_br(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
	bool check_corner_bl_tr(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
	void change_position_x(Player p, struct Vertex_Array** vertex);
	void change_position_y(Player p, struct Vertex_Array** vertex);
	inline bool check_corner_br_tl_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1);
	inline bool check_corner_tr_bl_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1);
	inline bool check_corner_tl_br_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1);
	inline bool check_corner_bl_tr_bool(float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1);

};








// By copy (old)
//struct Vertex_Array* Check_For_Collisions(struct Vertex_Array* vertex, Player *p, int size, bool* Is_Grounded, bool* Collides, int axis);
/*struct Vertex_Array* check_corner_br_tl(struct Vertex_Array* vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
struct Vertex_Array* check_corner_tr_bl(struct Vertex_Array* vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
struct Vertex_Array* check_corner_tl_br(struct Vertex_Array* vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
struct Vertex_Array* check_corner_bl_tr(struct Vertex_Array* vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
struct Vertex_Array* change_position_x(Player p, struct Vertex_Array* vertex);
struct Vertex_Array* change_position_y(Player p, struct Vertex_Array* vertex);*/