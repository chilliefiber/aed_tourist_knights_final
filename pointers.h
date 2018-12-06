#ifndef POINTERS_H
#define POINTERS_H
#include <stddef.h>

void *safeMalloc(size_t _size);
void freeAll( int _num_tur_points,  int ***tur_points,
              int  _width,  int ***map);
#endif
