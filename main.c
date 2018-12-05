#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "pointers.h"
#include "search.h"
#include "structs.h"

int main(int argc, char **argv){
  char *ending = strstr(argv[1], ".cities");
  if (ending == NULL || strlen(ending) != strlen(".cities")){
    printf("O ficheiro não tem a extensão correta.\n");
    exit (0);
  }
  FILE *input = fopen(argv[1], "r");
  if (input == NULL){
    printf("Erro a abrir ficheiro\n");
    exit (0);
  }
  char *name = strtok(argv[1], ".");
  char *output_name = safeMalloc(sizeof(char) * (strlen(name) + strlen(".walks") + 1));
  strcpy(output_name, name);
  strcat(output_name, ".walks");
  FILE* output = fopen(output_name, "w");
  free(output_name);
  if (output == NULL){
    printf("Erro a criar ficheiro de output\n");
    fclose(input);
    exit (0);
  }
  unsigned int width, height, num_tur_points, **tur_points = NULL, **map = NULL;
  char not_over;
  char valid_challenge;
  unsigned int aux1, aux2, num_points, cost_acum;
  int cost;
  char objective;
  do {
    if(fscanf(input, "%u", &height)!=1)
      exit(0);
    if(fscanf(input, "%u", &width)!=1)
      exit(0);
    if(fscanf(input, " %c", &objective)!=1)
      exit(0);
    if(fscanf(input, "%u", &num_tur_points)!=1)
      exit(0);
    fprintf(output, "%u ", height);
    fprintf(output, "%u ", width);
    fprintf(output, "%c ", objective);
    fprintf(output, "%u ", num_tur_points);
    valid_challenge = 1;
    if (objective != 'A' && objective != 'B' && objective != 'C')
      valid_challenge = 0;
    if (valid_challenge)
      tur_points = safeMalloc(sizeof(unsigned int*) * num_tur_points);
    for (int point_ix = 0; point_ix < num_tur_points; point_ix++){
      if (valid_challenge){
        tur_points[point_ix] = safeMalloc(sizeof(unsigned int) * 2);
        if(fscanf(input, "%u %u", tur_points[point_ix], tur_points[point_ix] + 1)!=2)
          exit(0);
      }
      else{
        if(fscanf(input, "%u %u", &aux1, &aux2)!=2)
          exit(0);
      }
    }
    map = createMap(input, width, height, valid_challenge);
    if (objective == 'A'){
      cost = -1;
      num_points = 0;
      Path *path = search(map, height, width, tur_points[0][0], tur_points[0][1],
                          tur_points[1][0], tur_points[1][1], &cost, &num_points);
      fprintf(output, "%d %u\n", cost, num_points);
      Path *p_aux;
      while (path != NULL){
        fprintf(output, "%u %u %u\n", path->coords.row, path->coords.column, map[path->coords.row][path->coords.column]);
        p_aux = path;
        path = path->next;
        free(p_aux);
      }
    }
    else if(objective == 'B'){
      Path *path;
      Path *whole_path = NULL;
      cost_acum=0;
      num_points = 0;
      for(int i=0; i<num_tur_points-1; i=i+1){
        cost = 0;
        path = search(map, height, width, tur_points[i][0], tur_points[i][1],
                            tur_points[i+1][0], tur_points[i+1][1], &cost, &num_points);

        if(!cost){
          cost_acum=-1;
          num_points=0;
          break;
        }
        joinPaths(&whole_path, path);

        cost_acum+=cost;
    }
      fprintf(output, "%d %u\n", cost_acum, num_points);
      Path *p_aux;
      while (whole_path != NULL){
        if(num_points)
          fprintf(output, "%u %u %u\n", whole_path->coords.row, whole_path->coords.column, map[whole_path->coords.row][whole_path->coords.column]);
        p_aux = whole_path;
        whole_path = whole_path->next;
        free(p_aux);
      }
    }
    else
      fprintf(output, "-1 0");
    fprintf(output, "\n");
    if (valid_challenge)
      freeAll(num_tur_points, &tur_points, height, &map);
    not_over = 0;
    while((objective = fgetc(input)) != EOF){
      if (objective != ' '  && objective != '\n' && objective != '\r'){
        fseek(input, -1, SEEK_CUR);
        not_over = 1;
        break;
      }
    }
  } while (not_over);
  fclose(output);
  fclose(input);
  exit (0);
}
