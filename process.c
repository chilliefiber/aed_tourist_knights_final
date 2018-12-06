/*
Ficheiro com as funções associadas ao processamento dos ficheiros de input
*/

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "pointers.h"
#include <string.h>

 int **createMap(FILE *fp,  int _width,  int _height, char valid_challenge){
   int aux;
   int **map = NULL;
  if (valid_challenge){
    // alocar memória para o mapa
    map = safeMalloc(sizeof( int*) * _height);
    for (int row = 0; row < _height; row++)
       map[row] = safeMalloc(sizeof( int) * _width);
  }
  // preencher mapa
  for (int row = 0; row < _height; row++){
    for (int column = 0; column < _width; column++){
      if (valid_challenge){
        if (fscanf(fp, "%d", map[row] + column) != 1)
          exit (0);
      }
      else{
        if (fscanf(fp, "%d", &aux) != 1)
          exit (0);
      }
    }
  }
  return map;
}

char *outputName(char *input_name){
  int input_length;
  if ((input_length = strlen(input_name)) < strlen(".cities") + 1)
    return NULL;
  char s[] = ".cities";
  for (int i = 0; i < strlen(".cities"); i++){
    if (input_name[input_length - strlen(".cities") + i] != s[i])
      return NULL;
  }
  char c = input_name[input_length - strlen(".cities") - 1]; // caractere imediatamente antes de .cities
  input_name[input_length - strlen(".cities")] = '\0';
  char *output_name = safeMalloc(sizeof(char) * (strlen(input_name) + strlen(".walks") + 1));
  strcpy(output_name, input_name);
  strcat(output_name, ".walks");        
  input_name[input_length - strlen(".cities")] = c;
  return output_name;
}
