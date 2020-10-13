#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void initList(List *l, int mlength){
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
      printf("Error al reservar memoria en memblock.\n");
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

Node *createNode(int elem) {
  Node *n = (Node *) malloc(sizeof(Node));

  if(n == NULL) {
    printf("Error al reservar memoria en createNode.\n");
    exit(1);
  }

  n->index = -1;
  n->elem = elem;
  n->ant = n->sig = NULL;
  n->inblock = 0;
  return n;
}

Node *getNode(List *l, int elem) {
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

void freeNode(List *l, Node *n) {

  if(n->inblock) {
    n->index = -1;
    n->elem = 0;
    n->ant = n->sig = NULL;
  }
  else {
    free(n);
  }
}


//Agrega al final
void queue(List *l, int elem) {
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

// Agrega un nodo en la posicion especificada
// y recorre los demas nodos
void add(List *l, int elem, int pos) {
  Node *n, *ap;

  if(pos == l->length){
    queue(l, elem); 
    return;
  }
 
  n = getNode(l, elem);
 
  if (l->length == 0 && pos == 0) {
    l->ini = l->fin = n;
    n->index = l->length;
  }
  else { 
    ap = find(*l, pos);
 
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

//Elimina el primer elemento de la lista
void dequeue(List *l, int *elem) {
  Node *tmp;

  if(l->length == 0) {
    printf("Error, lista vacia.\n");
    exit(1);
  }

  if(l->ini->sig != NULL)
    l->ini->sig->ant = NULL;

  tmp = l->ini;
  *elem = tmp->elem;
  l->ini = l->ini->sig; 
  l->length--; 
  freeNode(l, tmp);
}

void eliminate(List *l, int pos) {
  Node *ap, *tmp;

  if(l->length == 0) {
    printf("Error, lista vacia.\n");
    exit(1);
  }

  ap = find(*l, pos);
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
}

Node *find(List l, int pos) {
  Node *n;

  if(pos < 0 || pos >= l.length) {
    printf("Error, posicion invalida %d.\n", pos);
    exit(1);
  }

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
  
  if(n == NULL) {
    print(l);
    printf("Error, nodo en posicion %d no encontrado.\n", pos);
    exit(1);
  }

  return n;
}

void element(List l, int pos, int *elem) {

  Node *n = find(l, pos);
  *elem = n->elem; 
}

void print(List l) {
  Node *n;

  n = l.ini;

  while(n != NULL) {
    printf("[%d]=%d ", n->index, n->elem);
    n = n->sig;
  }
  printf(". length=%d\n", l.length);
}

void clear(List *l) {
  int i, n, e;

  n = l->length;

  for(i = 0; i < n; i++)  
    dequeue(l, &e);

  l->ini = l->fin = NULL;
}

void freeList(List *l) {
  int i;

  clear(l);

  if(l->max_length > 0) {
    for(i = 0; i < l->max_length; i++) {
      if(l->memblock[i]->index == -1) {
        free(l->memblock[i]);
        //printf("free..\n");
      }
    }

    free(l->memblock);
  }
}
