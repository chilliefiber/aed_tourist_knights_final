/*
Funções relacionadas com alocação de memória
*/

#include <stdio.h>
#include <stdlib.h>
#include "pointers.h"

void *safeMalloc(size_t _size){
  void *p = malloc(_size);
  if (p == NULL){
    exit (0);
  }
  return p;
}

void freeAll( int _num_tur_points,  int ***tur_points,
               int  _height,  int ***map){
  freeTurPoints(_num_tur_points, tur_points);
  freeCostMap(map, _height);
}

void freeCostMap(int ***map, int _height){
 for (int row = 0; row < _height; row++)
    free((*map)[row]);
  free(*map);
  *map = NULL;
}
void freeTurPoints(int _num_tur_points, int ***tur_points){
  for (int point_ix = 0; point_ix < _num_tur_points; point_ix++)
    free((*tur_points)[point_ix]);
  free(*tur_points);
  *tur_points = NULL;
}

void freeHyperNode(HyperNode n, int num_tur_points){
  for (int i = 0; i < num_tur_points; i++){
    if (n.edges[i] != NULL)
      freePath(n.edges[i]->path);
    free(n.edges[i]);
  }
  free(n.edges);
}

void freePath(Path *p){
  if (p == NULL)
    return;
  Path *next = p->next;
  free(p);
  freePath(next);
}

void freeGraph(HyperNode *graph, int _num_tur_points){
  for (int i = 0; i < _num_tur_points; i = i + 1)
    freeHyperNode(graph[i], _num_tur_points);
  free(graph);
}
