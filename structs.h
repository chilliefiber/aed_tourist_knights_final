#ifndef STRUCTS_H
#define STRUCTS_H
/**
 * guarda coordenadas de um ponto no mapa
 **/
typedef struct Point{
   int row;
   int column;
} Point;

/**
 * guarda as coordenadas de um ponto, o custo para o atingir
 * partindo da origem e um ponteiro para o Node do ponto que o
 * precede. aquando a sua inserção no acervo é lhe atribuido um
 * indice que facilita acessos futuros
 */
typedef struct Node{
  struct Node *parent;
  Point coords;
   int cost;
   int ix;
} Node;

/**
 *  tipo usado para gurdar a lista prioritaria.
 *  mantem a um int com a sua dimensao e a lista
 *  esta implementada como um acervo assente num
 *  vetor de ponteiros para Node's, acima descritos
 */
typedef struct PQueue{
   int size;
  Node **heap;
} PQueue;

/**
 * no para uma lista simplesmente ligada destinada
 * a registar caminhos
 */
typedef struct Path{
  Point coords;
  struct Path *next;
} Path;

/**
 *
 */
typedef struct Edge{
  int cost;
  int num_points;
  Path *path;
} Edge;

typedef struct HyperNode{
  Edge **edges;
} HyperNode;

Node *createNode(Node *parent,  int _row,  int _column,
                  int **map);
PQueue *createQueue( int _height,  int _width);
void freeQueue(PQueue *q);
void insert(PQueue *q, Node *p);
void popRoot(PQueue *q);
void heapifyDown(PQueue *q,  int ix);
 int smallerChildIndex( int ix, PQueue *q);
 int swapChildIndex( int ix, PQueue *q);
 int leftChildIndex( int ix);
 int rightChildIndex( int ix);
char hasLeftChild( int ix, PQueue *q);
char hasRightChild( int ix, PQueue *q);
void heapifyUp(PQueue *q,  int ix);
void swapParent( int ix, Node **heap);
void swap( int ix0,  int ix1, Node **heap);
 int pointCost( int ix, Node **heap);
 int parentCost( int ix, Node **heap);
 int parentIndex( int ix);
char empty(PQueue *q);
 int currentCost(Node *p);
 int newCost( int **map, Node *src, Point *dest);
 int heapNodeCost( int ix, Node **heap);
Path *createPath(Node *cur, Path *previous,  int _origin_row,  int _origin_column,  int *num_points);
Path *reversePath(Path *previous_path, int **tur_points, int new_dest_ix, int new_src_ix);
Path *initPath(int row, int column, Path *next);
Path *traversePath(Path *previous, int **tur_points, int new_src_ix, Path *old);
Edge *createEdge(Path *p, int cost, int num_points);
void initHyperNode(int ix, HyperNode *graph, int **map, int num_tur_points, int **tur_points);
#endif
