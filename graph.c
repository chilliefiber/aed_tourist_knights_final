#include "graph.h"
#include "pointers.h"
#include <stdlib.h>

/**
 * verifica se um dado ponto esta dentro do mapa
 * e se o seu custo não é nulo
 */
int isValidPoint( int _row,  int _column,
                  int _height,  int _width,
                  int **map){
  if (_row < 0 || _row >= _height || _column < 0 || _column >= _width
       || ! map[_row][_column])
    return 0;
  return 1;
}
