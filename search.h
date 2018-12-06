#ifndef SEARCH_H
#define SEARCH_H
#include "structs.h"

Node ***createNodeMap( int _height,  int _width);
void freeNodeMap(Node ***map,  int _height,  int _width);
Path *search( int **map,  int _height,  int _width, 
             int _origin_row,  int _origin_column,
             int _dest_row,  int _dest_column,
            int *cost,  int *num_points);
char explore(Node *p, Point *dest,  int **map,  int _height,  int _width, Node ***nodes, PQueue *q);
Point *accessibleNodes(Point *p,  int **map,  int _height,  int _width, char *num_accessible_nodes);
void validatePoint( int **map,  int _height,  int _width, 
                   char *num_points_to_explore, Point *points,
                    int _row,  int _column);
#endif
