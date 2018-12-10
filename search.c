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
            int *cost,  int *num_points, Path **previous_tail){
  Node ***nodes = createNodeMap(_height, _width), *cur;
  nodes[_origin_row][_origin_column] = createNode(NULL, _origin_row, _origin_column, map);
  PQueue *q = createQueue(_height, _width);
  insert(q, nodes[_origin_row][_origin_column]);
  Path *shortest_path = NULL;
  while(!empty(q)){ // enquanto houver pontos para explorar
    cur = q->heap[0]; // ponto a explorar
    popRoot(q);
    explore(cur, map, _height, _width, nodes, q);
    // se chegámos ao fim
    if (nodes[_dest_row][_dest_column] != NULL){
      shortest_path = createPath(nodes[_dest_row][_dest_column], NULL, _origin_row, _origin_column, num_points);
      // ligar os caminhos
      if (*previous_tail != NULL){
        (*previous_tail)->next = shortest_path;
      }
      Path *aux = shortest_path;
      while(aux->next != NULL)
        aux = aux->next;
      // atualizar a cauda do caminho
      *previous_tail = aux;
      *cost += nodes[_dest_row][_dest_column]->cost;
      break;
    }
  }
  freeNodeMap(nodes, _height, _width);
  freeQueue(q);
  return shortest_path;
}

void fillNode(int ix, HyperNode *graph, int **map, int _height, int _width, int **tur_points, int num_tur_points){
  initHyperNode(ix, graph, map, num_tur_points, tur_points);
  Node ***nodes = createNodeMap(_height, _width), *cur;
  nodes[tur_points[ix][0]][tur_points[ix][1]] = createNode(NULL, tur_points[ix][0], tur_points[ix][1], map); 
  PQueue *q = createQueue(_height, _width);
  insert(q, nodes[tur_points[ix][0]][tur_points[ix][1]]);
  int num_found_points = 0, num_points_to_find = num_tur_points - ix - 1, num_points_in_path;
  while(!empty(q) && num_found_points != num_points_to_find){
    cur = q->heap[0];
    popRoot(q);
    explore(cur, map, _height, _width, nodes, q);
    for (int i = ix + 1; i < num_tur_points; i = i + 1){
      // se chegámos a um novo ponto de destino
      if(nodes[tur_points[i][0]][tur_points[i][1]] != NULL && graph[ix].edges[i] == NULL){
        num_points_in_path = 0;
        num_found_points = num_found_points + 1;
        Path *p = createPath(nodes[tur_points[i][0]][tur_points[i][1]], NULL, tur_points[ix][0], tur_points[ix][1], &num_points_in_path);
        graph[ix].edges[i] = createEdge(p, nodes[tur_points[i][0]][tur_points[i][1]]->cost, num_points_in_path);
      }
    }
  }
  freeNodeMap(nodes, _height, _width);
  freeQueue(q);
}

void explore(Node *p,  int **map,  int _height,  int _width, Node ***nodes, PQueue *q){
  char num_accessible_nodes;
  Point *points = accessibleNodes(&(p->coords), map, _height, _width, &num_accessible_nodes);
  Node *cur;
  for (int i = 0; i < num_accessible_nodes; i = i + 1){
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

void checkPermutations(HyperNode *graph, int *cost, int *num_points, int *permutation, int size, int n, int *answer){
  // Calcular o custo desta permutação
  if (size == 1){
    int cur_cost = graph[0].edges[permutation[0]]->cost;
    int cur_num_points = graph[0].edges[permutation[0]]->num_points;
    // iterar pelos nós todos
    for (int i = 1; i < n; i = i + 1){
      cur_cost = cur_cost + graph[permutation[i-1]].edges[permutation[i]]->cost;
      cur_num_points = cur_num_points + graph[permutation[i-1]].edges[permutation[i]]->num_points;
    }
    // esta permutação tem um custo inferior ao mínimo custo encontrado até agora
    if (*cost == -1 || cur_cost < *cost){
      *cost = cur_cost;
      *num_points = cur_num_points;
      for (int i = 0; i < n; i++)
        answer[i] = permutation[i];
    }
  }
  for (int i = 0; i < size; i++){
    checkPermutations(graph, cost, num_points, permutation, size - 1, n, answer);
    if (size % 2 == 1){
      int aux = permutation[0];
      permutation[0] = permutation[size - 1];
      permutation[size - 1] = aux;
    }
    else{
      int aux = permutation[i];
      permutation[i] = permutation[size - 1];
      permutation[size - 1] = aux;
    }
  }
}
 
