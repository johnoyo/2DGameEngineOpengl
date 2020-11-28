#include "Index_Buffer.h"

void Index_Buffer::Make_Indecies(unsigned int size)
{
	index_buffer = (unsigned int*)malloc((size / 4) * 6 * sizeof(unsigned int));
	int w = 0;
	for (int k = 0; k < (size / 4) * 6; k += 6) {
		index_buffer[index++] = 0 + w;
		index_buffer[index++] = 3 + w;
		index_buffer[index++] = 2 + w;
		index_buffer[index++] = 2 + w;
		index_buffer[index++] = 1 + w;
		index_buffer[index++] = 0 + w;
		w += 4;
	}
	
}

void Index_Buffer::Clean()
{
	if (index_buffer != NULL) free(index_buffer);
	index = 0;
}

unsigned int* Index_Buffer::Get_Index_Buffer()
{
	return index_buffer;
}
