#include "structs.h"
#include "pointers.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * alloca memoria para um "Node" e preenche
 * os seus campos com os dados respetivos (argumentos
 * @return      ponteiro para o nodo criado
 */
Node *createNode(Node *parent,  int _row,  int _column,
                  int **map){
  Node *new = safeMalloc(sizeof(Node));
  if (parent != NULL)
    new->cost = parent->cost  + map[_row][_column];
  else
    new->cost = 0;
  new->parent = parent;
  new->coords.row = _row;
  new->coords.column = _column;
  return new;
}

/**
 * alloca memoria para a estrutura basilar da fila
 * @return         ponteiro para a fila
 */
PQueue *createQueue( int _height,  int _width){
  PQueue *q = safeMalloc(sizeof(PQueue));
  q->size = 0;
  q->heap = safeMalloc(sizeof(Node*) * _height * _width);
  return q;
}

/**
 * AQUI
 * @param q [description]
 */
void freeQueue(PQueue *q){
  free(q->heap);
  free(q);
}

Path *createPath(Node *cur, Path *previous,  int _origin_row,  int _origin_column,  int *num_points){
  Path *p = initPath(cur->coords.row, cur->coords.column, previous);
  *num_points = *(num_points) + 1;
  // se chegámos ao primeiro ponto do caminho
  if (cur->parent->coords.row == _origin_row && cur->parent->coords.column == _origin_column)
    return p;
  return createPath(cur->parent, p, _origin_row, _origin_column, num_points);
}


Path *reversePath(Path *previous_path, int **tur_points, int new_dest_ix, int new_src_ix){
  Path *p = initPath(tur_points[new_dest_ix][0], tur_points[new_dest_ix][1], NULL);
  Path *reverse = traversePath(p, tur_points, new_src_ix, previous_path);
  return reverse;
}

Path *initPath(int row, int column, Path *next){
  Path *p = safeMalloc(sizeof(Path));
  p->coords.row = row;
  p->coords.column = column;
  p->next = next;
  return p;
}

Path *traversePath(Path *previous, int **tur_points, int new_src_ix, Path *old){
  Path *new = initPath(old->coords.row, old->coords.column, previous);
  // se o próximo nó é o novo início/antigo destino, chegámos ao fim
  // se o próximo nó é NULL, quer dizer que só havia 1 passo no caminho e também chegámos ao fim
  if (old->next == NULL || (old->next->coords.row == tur_points[new_src_ix][0] && old->next->coords.column == tur_points[new_src_ix][1]))
    return new;
  return traversePath(new, tur_points, new_src_ix, old->next);
}

void insert(PQueue *q, Node *n){
  q->heap[q->size] = n;
  n->ix = q->size; // atualizar o indice do no
  q->size = q->size + 1;
  heapifyUp(q, q->size - 1);
}

void popRoot(PQueue *q){
  q->heap[0] = q->heap[q->size -1];
  q->heap[0]->ix = 0; // atualizar o indice do no
  q->size = q->size -1;
  heapifyDown(q, 0);
}

void heapifyDown(PQueue *q ,  int ix){
   int childIx;
  while ((childIx = swapChildIndex(ix, q))){
    swap(ix, childIx, q->heap);
    ix = childIx;
  }
}

 int smallerChildIndex( int ix, PQueue *q){
  if (!hasLeftChild(ix, q))
    return 0;
   int smallerChildIx = leftChildIndex(ix);
  if (hasRightChild(ix, q) && heapNodeCost(rightChildIndex(ix), q->heap) < heapNodeCost(smallerChildIx, q->heap))
    return rightChildIndex(ix);
  return smallerChildIx;
}

 int swapChildIndex( int ix, PQueue *q){
   int childIx = smallerChildIndex(ix, q);
  if (!childIx)
    return 0;
  return heapNodeCost(childIx, q->heap) < heapNodeCost(ix, q->heap) ? childIx : 0;
}

 int leftChildIndex( int ix){
  return ix * 2 + 1;
}

 int rightChildIndex( int ix){
  return ix * 2 + 2;
}

char hasLeftChild( int ix, PQueue *q){
  return leftChildIndex(ix) < q->size;
}

char hasRightChild( int ix, PQueue *q){
  return rightChildIndex(ix) < q->size;
}

void heapifyUp(PQueue *q,  int ix){
  while (ix != 0 && parentCost(ix, q->heap) > heapNodeCost(ix, q->heap)){
    swapParent(ix, q->heap);
    ix = parentIndex(ix);
  }
}

void swapParent( int ix, Node **heap){
  swap(ix, parentIndex(ix), heap);
}

void swap( int ix0,  int ix1, Node **heap){
  Node *aux = heap[ix0];
  heap[ix0] = heap[ix1];
  heap[ix1] = aux;
  // atualizar os indices nos nós
  heap[ix1]->ix = ix1;
  heap[ix0]->ix = ix0;
}

 int heapNodeCost( int ix, Node **heap){
  return heap[ix]->cost;
}

 int parentCost( int ix, Node **heap){
  return heapNodeCost(parentIndex(ix), heap);
}

 int parentIndex( int ix){
  return ( int) ((ix-1)/2);
}

char empty(PQueue *q){
  return q->size == 0;
}

 int newCost( int **map, Node *src, Point *dest){
  return src->cost + map[dest->row][dest->column];
}

/**
 * AQUI
 * @param whole_path [description]
 * @param path       [description]
 */
void joinPaths(Path **whole_path, Path *path){
  if(*whole_path==NULL){
    *whole_path=path;
    return;
  }

  Path *curr = *whole_path;

  while(curr->next!=NULL)
    curr=curr->next;

  curr->next=path;
}

void initHyperNode(int ix, HyperNode *graph, int **map, int num_tur_points, int **tur_points){
  HyperNode *cur = graph + ix;
  cur->edges = safeMalloc(sizeof(Edge*) * num_tur_points);
  cur->edges[ix] = NULL; // não é preciso uma estrutura para o proprio ponto
  // iterar pelos nós correspondentes aos pontos aos quais ainda não aplicámos Dijkstra
  for (int i = ix + 1; i < num_tur_points; i++)
    cur->edges[i] = NULL;
  // iterar pelos nós correspondentes aos pontos aos quais já aplicámos Dijkstra
  for (int i = ix - 1; i > -1; i--)
    // o custo será igual ao custo do caminho contrário menos o custo de chegar a cur
    // mais o custo de chegar ao outro ponto
    cur->edges[i] = createEdge(reversePath(graph[i].edges[ix]->path, tur_points, i, ix),
                              graph[i].edges[ix]->cost - map[tur_points[ix][0]][tur_points[ix][1]] +
                         map[tur_points[i][0]][tur_points[i][1]],
                         graph[i].edges[ix]->num_points);
}

Edge *createEdge(Path *p, int cost, int num_points){
  Edge *new = safeMalloc(sizeof(Edge));
  new->path = p;
  new->cost = cost;
  new->num_points = num_points;
  return new;
}
