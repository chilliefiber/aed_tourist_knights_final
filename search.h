#ifndef SEARCH_H
#define SEARCH_H
#include "structs.h"

Node ***createNodeMap( int _height,  int _width);
void freeNodeMap(Node ***map,  int _height,  int _width);
Path *search( int **map,  int _height,  int _width, 
             int _origin_row,  int _origin_column,
             int _dest_row,  int _dest_column,
            int *cost,  int *num_points, Path **previous_tail);
void fillNode(int ix, HyperNode *graph, int **map, int _height, int _width, int **tur_points, int num_tur_points);
void explore(Node *p,  int **map,  int _height,  int _width, Node ***nodes, PQueue *q);
Point *accessibleNodes(Point *p,  int **map,  int _height,  int _width, char *num_accessible_nodes);
void validatePoint( int **map,  int _height,  int _width, 
                   char *num_points_to_explore, Point *points,
                    int _row,  int _column);
void checkPermutations(HyperNode *graph, int *best_cost, int *num_points, int size, int *answer, int *visited, int *permutation,
                       int depth, int cost_acum);
int pathCost(HyperNode *graph, int *permutation, int size);
#endif
