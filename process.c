/*
Ficheiro com as funções associadas ao processamento dos ficheiros de input
*/

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "pointers.h"

unsigned int **createMap(FILE *fp, unsigned int _width, unsigned int _height, char valid_challenge){
  unsigned int aux;
  unsigned int **map = NULL;
  if (valid_challenge){
    // alocar memória para o mapa
    map = safeMalloc(sizeof(unsigned int*) * _height);
    for (int row = 0; row < _height; row++)
       map[row] = safeMalloc(sizeof(unsigned int) * _width);
  }
  // preencher mapa
  for (int row = 0; row < _height; row++){
    for (int column = 0; column < _width; column++){
      if (valid_challenge){
        if (fscanf(fp, "%u", map[row] + column) != 1)
          exit (0);
      }
      else{
        if (fscanf(fp, "%u", &aux) != 1)
          exit (0);
      }
    }
  }
  return map;
}
