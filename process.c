/*
Ficheiro com as funções associadas ao processamento dos ficheiros de input
*/

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "pointers.h"
#include <string.h>


/**
 * carrega para uma tabela os custos dos pontos do mapa
 * @param  fp              ficheiro de input
 * @param  _height         altura do mapa
 * @param  _width          largura do mapa
 * @param  valid_challenge boolean que faz deprezar alocação de mem. se a var.
 * for desconhecida
 * @return                 ponteiro para a tabela criada
 */
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

/**
 * gera string com o nome do ficheiro de output
 * @param  input_name nome do ficheiro de input
 * @return            pointeiro para o nome do ficheiro de output
 */
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

/**
 * funcao que elimina redundancia de pontos a visitar
 * @param  tur_points     tabela com coords. dos pontos a visitar
 * @param  num_tur_points num. de pontos a visitar
 * @return                vetor com nenhum ponto repetido
 */
int **removeDuplicates(int **tur_points, int *num_tur_points){
  int **no_dups = safeMalloc(*num_tur_points * sizeof(int*));
  int num_no_dups = 0;
  char dup;
  for (int i = 0; i < *num_tur_points; i = i + 1){
    dup = 0;
    for (int j = 0; j < num_no_dups; j++){
      if (tur_points[i][0] == no_dups[j][0] && tur_points[i][1] == no_dups[j][1]){
        dup = 1;
        break;
      }
    }
    if (!dup){
      no_dups[num_no_dups] = safeMalloc(2 * sizeof(int));
      no_dups[num_no_dups][0] = tur_points[i][0];
      no_dups[num_no_dups][1] = tur_points[i][1];
      num_no_dups = num_no_dups + 1;
    }
    free(tur_points[i]);
  }
  free(tur_points);
  *num_tur_points = num_no_dups;
  return no_dups;
}
