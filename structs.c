#include "structs.h"
#include "pointers.h"
#include <stdio.h>
#include <stdlib.h>

Node *createNode(Node *parent, unsigned int _row, unsigned int _column,
                 unsigned int **map){
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

PQueue *createQueue(unsigned int _height, unsigned int _width){
  PQueue *q = safeMalloc(sizeof(PQueue));
  q->size = 0;
  q->heap = safeMalloc(sizeof(Node*) * _height * _width);
  return q;
}

void freeQueue(PQueue *q){
  free(q->heap);
  free(q);
}

Path *createPath(Node *cur, Path *previous, unsigned int _origin_row, unsigned int _origin_column, unsigned int *num_points){
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
  q->size = q->size + 1;
  heapifyUp(q, q->size - 1);
}

void popRoot(PQueue *q){
  q->heap[0] = q->heap[q->size -1];
  q->size = q->size -1;
  heapifyDown(q, 0);
}

void heapifyDown(PQueue *q , unsigned int ix){
  unsigned int childIx;
  while ((childIx = swapChildIndex(ix, q))){
    swap(ix, childIx, q->heap);
    ix = childIx;
  }
}

unsigned int smallerChildIndex(unsigned int ix, PQueue *q){
  if (!hasLeftChild(ix, q))
    return 0;
  unsigned int smallerChildIx = leftChildIndex(ix);
  if (hasRightChild(ix, q) && heapNodeCost(rightChildIndex(ix), q->heap) < heapNodeCost(smallerChildIx, q->heap))
    return rightChildIndex(ix);
  return smallerChildIx;
}

unsigned int swapChildIndex(unsigned int ix, PQueue *q){
  unsigned int childIx = smallerChildIndex(ix, q);
  if (!childIx)
    return 0;
  return heapNodeCost(childIx, q->heap) < heapNodeCost(ix, q->heap) ? childIx : 0;
}

unsigned int leftChildIndex(unsigned int ix){
  return ix * 2 + 1;
}

unsigned int rightChildIndex(unsigned int ix){
  return ix * 2 + 2;
}

char hasLeftChild(unsigned int ix, PQueue *q){
  return leftChildIndex(ix) < q->size;
}

char hasRightChild(unsigned int ix, PQueue *q){
  return rightChildIndex(ix) < q->size;
}

void heapifyUp(PQueue *q, unsigned int ix){
  while (ix != 0 && parentCost(ix, q->heap) > heapNodeCost(ix, q->heap)){
    swapParent(ix, q->heap);
    ix = parentIndex(ix);
  }
}

void swapParent(unsigned int ix, Node **heap){
  swap(ix, parentIndex(ix), heap);
}

void swap(unsigned int ix0, unsigned int ix1, Node **heap){
  Node *aux = heap[ix0];
  heap[ix0] = heap[ix1];
  heap[ix1] = aux;
}

unsigned int heapNodeCost(unsigned int ix, Node **heap){
  return heap[ix]->cost;
}

unsigned int parentCost(unsigned int ix, Node **heap){
  return heapNodeCost(parentIndex(ix), heap);
}

unsigned int parentIndex(unsigned int ix){
  return (unsigned int) ((ix-1)/2);
}

char empty(PQueue *q){
  return q->size == 0;
}

unsigned int newCost(unsigned int **map, Node *src, Point *dest){
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
