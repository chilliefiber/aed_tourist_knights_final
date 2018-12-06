#include "structs.h"
#include "pointers.h"
#include "search.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
Node ***createNodeMap( int _height,  int _width){
  Node ***map = safeMalloc(sizeof(Node**) * _height);
  for (int row = 0; row < _height; row = row + 1){
    map[row] = safeMalloc(sizeof(Node*) * _width);
    for (int column = 0; column < _width; column = column + 1)
      map[row][column] = NULL;
  }
  return map;
}

void freeNodeMap(Node ***map,  int _height,  int _width){
  for (int row = 0; row < _height; row = row + 1){
    for (int column = 0; column < _width; column = column + 1)
      free(map[row][column]);
    free(map[row]);
  }
  free(map);
}

Path *search( int **map,  int _height,  int _width,
             int _origin_row,  int _origin_column,
             int _dest_row,  int _dest_column,
            int *cost,  int *num_points){
  Node ***nodes = createNodeMap(_height, _width), *cur;
  nodes[_origin_row][_origin_column] = createNode(NULL, _origin_row, _origin_column, map);
  PQueue *q = createQueue(_height, _width);
  insert(q, nodes[_origin_row][_origin_column]);
  Point dest;
  dest.row = _dest_row;
  dest.column = _dest_column;
  char finished = 0;
  Path *shortest_path = NULL;
  while(!empty(q)){ // enquanto houver pontos para explorar
    cur = q->heap[0]; // ponto a explorar
    popRoot(q);
    if ((finished = explore(cur, &dest, map, _height, _width, nodes, q))){
      shortest_path = createPath(nodes[dest.row][dest.column], NULL, _origin_row, _origin_column, num_points);
      *cost += nodes[dest.row][dest.column]->cost;
      break;
    }
  }
  freeNodeMap(nodes, _height, _width);
  freeQueue(q);
  return shortest_path;
}

char explore(Node *p, Point *dest,  int **map,  int _height,  int _width, Node ***nodes, PQueue *q){
  char num_accessible_nodes;
  Point *points = accessibleNodes(&(p->coords), map, _height, _width, &num_accessible_nodes);
  Node *cur;
  for (int i = 0; i < num_accessible_nodes; i = i + 1){
    // se chegámos ao destino
    if (points[i].row == dest->row && points[i].column == dest->column){
      nodes[dest->row][dest->column] = createNode(p, dest->row, dest->column, map);
      free(points);
      return 1;
    }
    cur = nodes[points[i].row][points[i].column];
    // se for a primeira vez que visitamos este ponto, criamos um nó para o mapa de nós
    // e inserimos o nó na heap
    if (cur == NULL){
      nodes[points[i].row][points[i].column] = createNode(p, points[i].row, points[i].column, map);
      insert(q, nodes[points[i].row][points[i].column]);
      continue;
    }
   // caso contrário, ver se o custo de chegar ao ponto é inferior do que o do caminho guardado. Nesse caso,
   // atualizar a heap com o novo valor
    if (cur->cost > newCost(map, p, points + i)){
      cur->parent = p; // o pai passa a ser o nó que estamos a explorar
      cur->cost = newCost(map, p, points + i); // o custo do nó é atualizado
      // descobrimos o indice deste ponto na heap, e garantimos que a condição de acervo é mantida com o novo custo
      heapifyUp(q, cur->ix);
    }
  }
  free(points);
  return 0;
}

Point *accessibleNodes(Point *p,  int **map,  int _height,  int _width, char *num_accessible_nodes){
  *num_accessible_nodes = 0;
  Point points[8];
  validatePoint(map, _height, _width, num_accessible_nodes, points, p->row + 2, p->column + 1);
  validatePoint(map, _height, _width, num_accessible_nodes, points, p->row + 2, p->column - 1);
  validatePoint(map, _height, _width, num_accessible_nodes, points, p->row + 1, p->column + 2);
  validatePoint(map, _height, _width, num_accessible_nodes, points, p->row + 1, p->column - 2);
  validatePoint(map, _height, _width, num_accessible_nodes, points, p->row - 2, p->column + 1);
  validatePoint(map, _height, _width, num_accessible_nodes, points, p->row - 2, p->column - 1);
  validatePoint(map, _height, _width, num_accessible_nodes, points, p->row - 1, p->column - 2);
  validatePoint(map, _height, _width, num_accessible_nodes, points, p->row - 1, p->column + 2);
  Point *to_access = safeMalloc(sizeof(Point) * (*num_accessible_nodes));
  for (int i = 0; i < *num_accessible_nodes; i = i + 1)
    to_access[i] = points[i];
  return to_access;
}

void validatePoint( int **map,  int _height,  int _width,
                   char *num_accessible_nodes, Point *points,
                    int _row,  int _column){
  if (isValidPoint(_row, _column, _height, _width, map)){
    Point aux;
    aux.row = _row;
    aux.column = _column;
    points[(int) *num_accessible_nodes] = aux;
    *num_accessible_nodes = (*num_accessible_nodes) + 1;
  }
}
