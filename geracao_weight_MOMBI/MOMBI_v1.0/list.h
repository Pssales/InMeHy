
typedef struct Node {
  int index;
  int elem;
  struct Node *ant;
  struct Node *sig;
  int inblock;
} Node;

typedef struct List {
  int length;
  int max_length;
  Node *ini;
  Node *fin;
  Node **memblock;
  int available;
} List;

void initList(List *l, int mlength);
Node *createNode(int elem);
Node *getNode(List *l, int elem); 
void freeNode(List *l, Node *n); 
void queue(List *l, int elem);
void add(List *l, int elem, int pos);
void dequeue(List *l, int *elem);
void eliminate(List *l, int pos);
Node *find(List l, int pos);
void element(List l, int pos, int *elem);
void print(List l);
void clear(List *l);
void freeList(List *l);
