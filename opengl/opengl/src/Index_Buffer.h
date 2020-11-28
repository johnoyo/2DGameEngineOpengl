#pragma once
#include <stdlib.h>

class Index_Buffer {
public:
	void Make_Indecies(unsigned int size);
	void Clean();
	unsigned int* Get_Index_Buffer();

private:
	unsigned int* index_buffer;
	unsigned int index = 0;
};