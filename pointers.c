/*
Funções relacionadas com alocação de memória
*/

#include <stdio.h>
#include <stdlib.h>

void *safeMalloc(size_t _size){
  void *p = malloc(_size);
  if (p == NULL){
    exit (0);
  }
  return p;
}

void freeAll( int _num_tur_points,  int ***tur_points,
               int  _height,  int ***map){
  for (int point_ix = 0; point_ix < _num_tur_points; point_ix++)
    free((*tur_points)[point_ix]);
  free(*tur_points);
  *tur_points = NULL;
  for (int row = 0; row < _height; row++)
    free((*map)[row]);
  free(*map);
  *map = NULL;
}
