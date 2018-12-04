#ifndef POINTERS_H
#define POINTERS_H
#include <stddef.h>

void *safeMalloc(size_t _size);
void freeAll(unsigned int _num_tur_points, unsigned int ***tur_points,
             unsigned int  _width, unsigned int ***map);
#endif
