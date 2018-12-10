#ifndef POINTERS_H
#define POINTERS_H
#include <stddef.h>
#include "structs.h"
void *safeMalloc(size_t _size);
void freeAll( int _num_tur_points,  int ***tur_points,
              int  _width,  int ***map);
void freeTurPoints(int _num_tur_points, int ***tur_points);
void freeHyperNode(HyperNode n, int num_tur_points);
void freePath(Path *p);
void freeGraph(HyperNode *graph, int _num_tur_points);
void freeCostMap(int ***map, int _height);
#endif
