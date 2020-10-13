/**************************************************************
 * list.h       Implementation of doublly linked list         *
 *                                                            *
 * September 2013                                             *
 *************************************************************/

#ifndef _LIST_
#define _LIST_

typedef struct Node {
  int index;
  int elem;
  struct Node *ant;
  struct Node *sig;
  int inblock;  /* specifies whether the node is taken from memblock */
} Node;

typedef struct EMO_List {
  int length;
  int max_length;
  Node *ini;
  Node *fin;
  Node **memblock;
  int available;
} EMO_List;

void EMO_initList(EMO_List *l, int mlength);
Node *createNode(int elem);
Node *getNode(EMO_List *l, int elem); 
void freeNode(EMO_List *l, Node *n); 
void EMO_queue(EMO_List *l, int elem);
void EMO_add(EMO_List *l, int elem, int pos);

int EMO_dequeue(EMO_List *l);
int EMO_eliminateAt(EMO_List *l, int *elem, int pos);
int EMO_eliminate(EMO_List *l, int pos);
Node *findAt(EMO_List l, int pos);
Node *find(EMO_List l, int pos);
int EMO_List_find(EMO_List *l, int elem);
int EMO_element(EMO_List l, int pos);
void EMO_lprint(EMO_List l);
void EMO_clear(EMO_List *l);
void EMO_freeList(EMO_List *l);

#endif

