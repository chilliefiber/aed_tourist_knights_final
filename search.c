#include "structs.h"
#include "pointers.h"
#include "search.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * inicializa tabela (das dims do mapa) de ponteiros para Node's
 * @param  _height altura do mapa
 * @param  _width  largura do mapa
 * @return         ponteiro para a tabela
 */
Node ***createNodeMap( int _height,  int _width){
  Node ***map = safeMalloc(sizeof(Node**) * _height);
  for (int row = 0; row < _height; row = row + 1){
    map[row] = safeMalloc(sizeof(Node*) * _width);
    for (int column = 0; column < _width; column = column + 1)
      map[row][column] = NULL;
  }
  return map;
}

/**
 * libeta memoria alocada para a tabela de Node's
 * @param map     ponteiro para
 * @param _height altura do mapa
 * @param _width  largura do mapa
 */
void freeNodeMap(Node ***map,  int _height,  int _width){
  for (int row = 0; row < _height; row = row + 1){
    for (int column = 0; column < _width; column = column + 1)
      free(map[row][column]);
    free(map[row]);
  }
  free(map);
}

/**
 * detemina caminho mais curto entre dois pontos
 * implementa em conjunto com a func. explore()
 * o algoritmo de dijkstra com uma lista prioritaria
 * na forma de acervo/heap
 * @param  map            tabela dos custos
 * @param  _height        altura do mapa
 * @param  _width         largura do mapa
 * @param  _origin_row    coordenada y do ponto inicial
 * @param  _origin_column coordenada x do ponto inicial
 * @param  _dest_row      coordenada y do ponto inicial
 * @param  _dest_column   coordenada x do ponto inicial
 * @param  cost           custo do caminho já percorrido (Var. B)
 * @param  num_points     numero de pontos do caminho já percorrido (Var. B)
 * @param  previous_tail  cauda da lista que guarda o caminho já percorrido (Var. B)
 * @return                caminho mais curto determinado
 */
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

/**
 * [fillNode description]
 * @param ix             indice do ponto de origem
 * @param graph          grafo que tem os pontos turisticos e as arestas que
 * os ligam. ponderado e direcionado
 * @param map            mapa dos custos
 * @param _height
 * @param _width
 * @param tur_points
 * @param num_tur_points
 */
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

/**
 * explora os pontos à volta de "p" e atualiza a lista de prioridades com os
 * melhores custos
 * @param p       ponto a partir do qual se faz a busca
 * @param map     tebela dos custos
 * @param _height altura do mapa
 * @param _width  largura do mapa
 * @param nodes   mapa de nós
 * @param q       lista prioritaria - heap
 */
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
      //garantimos que a condição de acervo é mantida com o novo custo
      heapifyUp(q, cur->ix);
    }
  }
  free(points);
}

/**
 * gera um vetor com os pontos acessiveis a partir de "p"
 * @param  p                    ponto a partir do qual se faz a busca
 * @param  map                  tabela dos custos
 * @param  _height              altura do mapa
 * @param  _width               largura do mapa
 * @param  num_accessible_nodes numero de pontos acessiveis partindo de p
 * @return                      vetor de Node's de Point's(struct das coordenadas) acessiveis
 */
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

/**
 * guarda o ponto no vetor acima descrito se este for valido
 * (dentro do mapa e com custo !=0)
 * @param  map                 tabela dos custos
 * @param  _height             altura do mapa
 * @param  _width              largura do mapa
 * @param num_accessible_nodes numero de pontos asseciveis part de p
 * @param points               vetor auxiliar onde se guarda o ponto se for acessivel
 * @param _row                 coord. ponto a validar
 * @param _column              coord. ponto a validar
 */
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

void checkPermutations(HyperNode *graph, int *best_cost, int *num_points, int size, int *answer, int *visited, int *permutation,
                       int depth, int cost_acum){
  int temp_cost = 0;
  int aux_cost;
  // se temos uma permutação completa cujo custo é inferior ao custo mínimo encontrado até agora
  if (depth == size && ((aux_cost = pathCost(graph, permutation, size)) < *best_cost || *best_cost == -1 )){
    *best_cost = aux_cost; // atualizar o custo
    *num_points = 0;
    int previous_ix = 0;
    // atualizar a melhor permutação e o número de pontos no caminho
    for (int i = 0; i < size; i++){
      answer[i] = permutation[i];
      *num_points = *num_points + graph[previous_ix].edges[permutation[i]]->num_points;
      previous_ix = permutation[i];
    }
    return;
  }
  for (int i = 0; i < size; i++){
    if (!visited[i] && depth == 0){
      temp_cost = graph[0].edges[i+1]->cost;
      if (temp_cost + cost_acum < *best_cost || *best_cost == -1){
        visited[i] = 1;
        permutation[depth] = i+1;
        checkPermutations(graph, best_cost, num_points, size, answer, visited, permutation,depth + 1, temp_cost + cost_acum);
        visited[i] = 0;
      }
    }
    if (!visited[i] && depth){
      temp_cost = graph[permutation[depth-1]].edges[i+1]->cost;
      if (temp_cost + cost_acum < *best_cost || *best_cost == -1){
        visited[i] = 1;
        permutation[depth] = i + 1;
        checkPermutations(graph, best_cost, num_points, size, answer, visited, permutation,depth + 1, temp_cost + cost_acum);
        visited[i] = 0;
      }
    }
  }
}
/**
 * calcula o custo do caminho de uma permutacao
 */
int pathCost(HyperNode *graph, int *permutation, int size){
  int previous_ix = 0;
  int cost = 0;
  for (int i = 0; i < size; i = i + 1){
    cost = cost + graph[previous_ix].edges[permutation[i]]->cost;
    previous_ix = permutation[i];
  }
  return cost;
}
