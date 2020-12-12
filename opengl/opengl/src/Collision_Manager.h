#pragma once
#include "Renderer.h"
#include "Player.h"


class Collision_Manager {
public:
	void Check_For_Collisions(struct Vertex_Array** vertex, Player* p, int size, float tile_size, bool* Is_Grounded, bool* Collides, int axis);
	bool Check_If_Obj_Collides_With_Obj(Player obj1, Player obj2, Vertex_Array* vertex);

private:
	bool check_corner_br_tl(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
	bool check_corner_tr_bl(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
	bool check_corner_tl_br(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);
	bool check_corner_bl_tr(struct Vertex_Array** vertex, Player* p, float p_x, float p_y, float l_x, float l_y, float l_x1, float l_y1, int axis);

	bool check_side_l_r(struct Vertex_Array** vertex, Player* p, float p2_x, float p2_y, float p1_x, float p1_y, float l3_x, float l3_y, float l0_x, float l0_y, float l1_x, float l1_y, int axis);
	bool check_side_r_l(struct Vertex_Array** vertex, Player* p, float p0_x, float p0_y, float p3_x, float p3_y, float l1_x, float l1_y, float l2_x, float l2_y, float l0_x, float l0_y, int axis);
	bool check_side_b_t(struct Vertex_Array** vertex, Player* p, float p0_x, float p0_y, float p1_x, float p1_y, float l3_x, float l3_y, float l2_x, float l2_y, float l0_x, float l0_y, int axis);
	bool check_side_t_b(struct Vertex_Array** vertex, Player* p, float p2_x, float p2_y, float p3_x, float p3_y, float l1_x, float l1_y, float l0_x, float l0_y, float l3_x, float l3_y, int axis);

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