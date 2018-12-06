#include "structs.h"
#include "pointers.h"
#include <stdio.h>
#include <stdlib.h>

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

PQueue *createQueue( int _height,  int _width){
  PQueue *q = safeMalloc(sizeof(PQueue));
  q->size = 0;
  q->heap = safeMalloc(sizeof(Node*) * _height * _width);
  return q;
}

void freeQueue(PQueue *q){
  free(q->heap);
  free(q);
}

Path *createPath(Node *cur, Path *previous,  int _origin_row,  int _origin_column,  int *num_points){
  Path *p = safeMalloc(sizeof(Path));
  p->coords.row = cur->coords.row;
  p->coords.column = cur->coords.column;
  p->next = previous;
  *num_points = *(num_points) + 1;
  // se chegámos ao primeiro ponto do caminho
  if (cur->parent->coords.row == _origin_row && cur->parent->coords.column == _origin_column)
    return p;
  return createPath(cur->parent, p, _origin_row, _origin_column, num_points);
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
