/**************************************************************
 * list.c       Implementation of doubly linked list.         *
 *                                                            *
 * September 2014                                             *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/* Create a new node */
Node *createNode(int elem) {
  Node *n;

  if((n = (Node *) malloc(sizeof(Node))) == NULL) {
    printf("Error, not enough memory in list.c:createNode.\n");
    exit(1);
  }
  n->index = -1;
  n->elem = elem;
  n->ant = n->sig = NULL;
  n->inblock = 0;
  return n;
}

/* Initialize the structure of a list and reserves
   the memory from which nodes will be allocated.
 */
void EMO_initList(EMO_List *l, int mlength){
  Node *n;
  int i;

  l->ini = l->fin = NULL;
  l->length = 0;
  l->max_length = mlength;

  if(mlength == 0) 
    l->available = -1;
  else {
    l->memblock = (Node **) malloc(sizeof(Node *) * mlength);

    if(l->memblock == NULL) {
      printf("Error, not enough memory in list.c:initList.\n");
      exit(1);
    }
 
    for(i = 0; i < mlength; i++) {
      n = createNode(0);
      n->inblock = 1;
      l->memblock[i] = n;
    }

    l->available = 0;
  }
}


/* Get a node from the block of memory reserved.
   If there is not available memory, it creates a new one
 */
Node *getNode(EMO_List *l, int elem) {
  Node *n;
  int i;

  if(l->available == -1)
    return createNode(elem);

  i = l->available;
  n = l->memblock[i];
  n->elem = elem;
 
  while(1) {
    i = (i + 1) % l->max_length;

    if(i == l->available) {
      l->available = -1;
      break;
    }

    if(l->memblock[i]->index == -1) {
      l->available = i;
      break;
    }
  }

  return n;
}

/* Free a node */
void freeNode(EMO_List *l, Node *n) {

  if(n->inblock) {
    n->index = -1;
    n->elem = 0;
    n->ant = n->sig = NULL;
  }
  else {
    free(n);
  }
}

/* Append a node at the end of the list */
void EMO_queue(EMO_List *l, int elem) {
  Node *n = getNode(l, elem);

  if (l->length == 0) {
    l->ini = l->fin = n;
  }
  else {
    n->ant = l->fin;
    l->fin->sig = n;
    l->fin = n;
  }
  n->index = l->length++; 
}

/* Insert a node at the given position */
void EMO_add(EMO_List *l, int elem, int pos) {
  Node *n, *ap;

  if(pos == l->length){
    EMO_queue(l, elem); 
    return;
  }
 
  n = getNode(l, elem);
 
  if (l->length == 0 && pos == 0) {
    l->ini = l->fin = n;
    n->index = l->length;
  }
  else { 
    ap = findAt(*l, pos);
 
    n->ant = ap->ant;
    n->sig = ap;
 
    if(ap->ant != NULL)
      ap->ant->sig = n;
 
    ap->ant = n;

    n->index = ap->index;

    while(ap != NULL) {
      ap->index++;
      ap = ap->sig;
    }

    if(pos == 0)
      l->ini = n;

    if(pos == l->length)
      l->fin = n;
  }
  l->length++;
}

/* Delete the first node from the list */
int EMO_dequeue(EMO_List *l) {
  int elem;
  Node *tmp;

  if(l->length == 0) 
    return 0;

  if(l->ini->sig != NULL)
    l->ini->sig->ant = NULL;

  tmp = l->ini;
  elem = tmp->elem;
  l->ini = l->ini->sig; 
  l->length--; 
  freeNode(l, tmp);

  return elem;
}

/* Delete the node from the given position */
int EMO_eliminateAt(EMO_List *l, int *elem, int pos) {
  Node *ap, *tmp;

  if(l->length == 0)
    return 0;

  ap = findAt(*l, pos);

  if(ap == NULL)
    return 0;

  if(elem != NULL)
    *elem = ap->elem;

  if(ap->ant != NULL)
    ap->ant->sig = ap->sig;

  if(ap->sig != NULL)
    ap->sig->ant = ap->ant;

  if(ap == l->ini)
    l->ini = ap->sig;

  if(ap == l->fin)
    l->fin = ap->ant;

  tmp = ap;
  ap = ap->sig;

  while(ap != NULL) {
    ap->index--;
    ap = ap->sig;
  }

  l->length--;
  freeNode(l, tmp);
  return 1;
}

/* Delete the node of a given element */
int EMO_eliminate(EMO_List *l, int elem) {
  Node *ap, *tmp;

  if(l->length == 0)
    return 0;

  ap = find(*l, elem);

  if(ap == NULL)
    return 0;

  if(ap->ant != NULL)
    ap->ant->sig = ap->sig;

  if(ap->sig != NULL)
    ap->sig->ant = ap->ant;

  if(ap == l->ini)
    l->ini = ap->sig;

  if(ap == l->fin)
    l->fin = ap->ant;

  tmp = ap;
  ap = ap->sig;

  while(ap != NULL) {
    ap->index--;
    ap = ap->sig;
  }

  l->length--;
  freeNode(l, tmp);
  return 1;
}

/* Look for the node at the given position */ 
Node *findAt(EMO_List l, int pos) {
  Node *n;

  if(pos < 0 || pos >= l.length)
    return NULL;

  if(l.length - pos - 1 >= pos) {
    n = l.ini;
    
    while(n != NULL) {
      if(n->index == pos)
        break;
      n = n->sig;
    }
  }
  else {
    n = l.fin;
  
    while(n != NULL) {
      if(n->index == pos)
        break;
      n = n->ant;
    }
  }
  
  return n;
}

/* Look for the node which contains a given element */ 
Node *find(EMO_List l, int elem) {
  Node *n;

  n = l.ini;
    
  while(n != NULL) {
    if(n->elem == elem)
      break;
    n = n->sig;
  }
  
  return n;
}

/* Look for the node which contains a given element */
int EMO_List_find(EMO_List *l, int elem) {
  Node *n;

  n = l->ini;

  while(n != NULL) {
    if(n->elem == elem)
      return 1;
    n = n->sig;
  }

  return 0;
}

/* Return the element at the given position */
int EMO_element(EMO_List l, int pos) {
  Node *n = findAt(l, pos);
  return n->elem; 
}

/* Print the list */
void EMO_lprint(EMO_List l) {
  Node *n;

  n = l.ini;

  while(n != NULL) {
    printf("[%d]=%d ", n->index, n->elem);
    n = n->sig;
  }
  printf(". length=%d\n", l.length);
}

/* Clean the list */
void EMO_clear(EMO_List *l) {
  int i, n;

  n = l->length;

  for(i = 0; i < n; i++)  
    EMO_dequeue(l);

  l->ini = l->fin = NULL;
}

/* Free the list */
void EMO_freeList(EMO_List *l) {
  int i;

  EMO_clear(l);

  if(l->max_length > 0) {
    for(i = 0; i < l->max_length; i++) {
      if(l->memblock[i]->index == -1) {
        free(l->memblock[i]);
      }
    }

    free(l->memblock);
  }
}

