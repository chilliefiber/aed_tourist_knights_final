#ifndef SEARCH_H
#define SEARCH_H
#include "structs.h"

Node ***createNodeMap(unsigned int _height, unsigned int _width);
void freeNodeMap(Node ***map, unsigned int _height, unsigned int _width);
Path *search(unsigned int **map, unsigned int _height, unsigned int _width, 
            unsigned int _origin_row, unsigned int _origin_column,
            unsigned int _dest_row, unsigned int _dest_column,
            int *cost, unsigned int *num_points);
char explore(Node *p, Point *dest, unsigned int **map, unsigned int _height, unsigned int _width, Node ***nodes, PQueue *q);
Point *accessibleNodes(Point *p, unsigned int **map, unsigned int _height, unsigned int _width, char *num_accessible_nodes);
void validatePoint(unsigned int **map, unsigned int _height, unsigned int _width, 
                   char *num_points_to_explore, Point *points,
                   unsigned int _row, unsigned int _column);
#endif
