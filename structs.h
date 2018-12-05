#ifndef STRUCTS_H
#define STRUCTS_H
typedef struct Point{
  unsigned int row;
  unsigned int column;
} Point;

typedef struct Node{
  struct Node *parent;
  Point coords;
  unsigned int cost;
} Node;

typedef struct PQueue{
  unsigned int size;
  Node **heap;
} PQueue;

typedef struct Path{
  Point coords;
  struct Path *next;
} Path;

Node *createNode(Node *parent, unsigned int _row, unsigned int _column,
                 unsigned int **map);
PQueue *createQueue(unsigned int _height, unsigned int _width);
void freeQueue(PQueue *q);
void insert(PQueue *q, Node *p);
void popRoot(PQueue *q);
void heapifyDown(PQueue *q, unsigned int ix);
unsigned int smallerChildIndex(unsigned int ix, PQueue *q);
unsigned int swapChildIndex(unsigned int ix, PQueue *q);
unsigned int leftChildIndex(unsigned int ix);
unsigned int rightChildIndex(unsigned int ix);
char hasLeftChild(unsigned int ix, PQueue *q);
char hasRightChild(unsigned int ix, PQueue *q);
void heapifyUp(PQueue *q, unsigned int ix);
void swapParent(unsigned int ix, Node **heap);
void swap(unsigned int ix0, unsigned int ix1, Node **heap);
unsigned int pointCost(unsigned int ix, Node **heap);
unsigned int parentCost(unsigned int ix, Node **heap);
unsigned int parentIndex(unsigned int ix);
char empty(PQueue *q);
unsigned int currentCost(Node *p);
unsigned int newCost(unsigned int **map, Node *src, Point *dest);
unsigned int heapNodeCost(unsigned int ix, Node **heap);
Path *createPath(Node *cur, Path *previous, unsigned int _origin_row, unsigned int _origin_column, unsigned int *num_points);
void joinPaths(Path **whole_path, Path *path);

#endif
