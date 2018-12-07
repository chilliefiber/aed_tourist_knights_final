#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "pointers.h"
#include "search.h"
#include "structs.h"
#include "graph.h"

int main(int argc, char **argv){
  FILE *input = fopen(argv[1], "r");
  if (input == NULL)
    exit (0);
  char *output_name = outputName(argv[1]);
  if (output_name == NULL)
    exit(0);
  FILE* output = fopen(output_name, "w");
  free(output_name);
  if (output == NULL){
    //printf("Erro a criar ficheiro de output\n");
    fclose(input);
    exit (0);
  }
   int width, height, num_tur_points, **tur_points = NULL, **map = NULL;
  char not_over;
  char valid_challenge;
   int aux1, aux2, num_points;
  int cost, cost_acum;
  char objective;
  do {
    if(fscanf(input, "%d", &height)!=1)
      exit(0);
    if(fscanf(input, "%d", &width)!=1)
      exit(0);
    if(fscanf(input, " %c", &objective)!=1)
      exit(0);
    if(fscanf(input, "%d", &num_tur_points)!=1)
      exit(0);
    fprintf(output, "%d ", height);
    fprintf(output, "%d ", width);
    fprintf(output, "%c ", objective);
    fprintf(output, "%d ", num_tur_points);
    valid_challenge = 1;
    if (objective != 'A' && objective != 'B' && objective != 'C')
      valid_challenge = 0;
    if (valid_challenge)
      tur_points = safeMalloc(sizeof( int*) * num_tur_points);
    for (int point_ix = 0; point_ix < num_tur_points; point_ix++){
      if (valid_challenge){
        tur_points[point_ix] = safeMalloc(sizeof( int) * 2);
        if(fscanf(input, "%d %d", tur_points[point_ix], tur_points[point_ix] + 1)!=2)
          exit(0);
      }
      else{
        if(fscanf(input, "%d %d", &aux1, &aux2)!=2)
          exit(0);
      }
    }
    map = createMap(input, width, height, valid_challenge);
    if (objective == 'A'){
      cost = 0;
      num_points = 0;
      int dest_eq_src = 0;
      Path *path = NULL;
      
      if (num_tur_points==2){
        dest_eq_src = tur_points[0][0] == tur_points[1][0] &&  tur_points[0][1] == tur_points[1][1];
        // se o ponto inicial é valido e o destino é diferente do inicio
        if (map[tur_points[0][0]][tur_points[0][1]] &&  !dest_eq_src)
          path = search(map, height, width, tur_points[0][0], tur_points[0][1],
                            tur_points[1][0], tur_points[1][1], &cost, &num_points);
      }    
	  if(!cost){
	    cost=-1;
	    num_points=0;
	  }
	  if (dest_eq_src)
          cost = 0;
      fprintf(output, "%d %d\n", cost, num_points);
      Path *p_aux;
      while (path != NULL){
        fprintf(output, "%d %d %d\n", path->coords.row, path->coords.column, map[path->coords.row][path->coords.column]);
        p_aux = path;
        path = path->next;
        free(p_aux);
      }
    }
    else if(objective == 'B'){
      Path *path;
      Path *whole_path = NULL;
      cost_acum=0;
      num_points=0;
      int no_null_points_in_path=1;
      
      if(num_tur_points!=1){

        for(int i=0; i<num_tur_points; i=i+1){
          if(!isValidPoint(tur_points[i][0], tur_points[i][1], height, width, map)) {
            no_null_points_in_path=0;
            cost_acum=-1;
            break;
          }
        }  
	    
        if(no_null_points_in_path){
          for(int i=0; i<num_tur_points-1; i=i+1){
            if(tur_points[i][0] == tur_points[i+1][0] &&  tur_points[i][1] == tur_points[i+1][1])
              continue;
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
        }
	  }
   
      else
        cost_acum=-1;
        
      fprintf(output, "%d %d\n", cost_acum, num_points);
      Path *p_aux;
      while (whole_path != NULL){
        if(num_points)
          fprintf(output, "%d %d %d\n", whole_path->coords.row, whole_path->coords.column, map[whole_path->coords.row][whole_path->coords.column]);
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
