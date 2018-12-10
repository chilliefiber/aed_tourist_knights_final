#ifndef PROCESS_H
#define PROCESS_H
 int **createMap(FILE *fp,  int _width, int _height, char valid_challenge);
char *outputName(char *input_name);
int **removeDuplicates(int **tur_points, int *num_tur_points);
#endif
