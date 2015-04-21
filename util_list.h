#include "kr_common.h"
#include "kr_util.h"


/*!
* \file list.h
* \brief File defining the list structure as studied in TP 4
*/

#ifndef __UTIL_LIST_H__
#define __UTIL_LIST_H__


/*! \struct nodeList
* \brief Data element of a list, containing a tree
*/
typedef struct nodeList{
	 void * e;					/*!< A pointer to an element*/
	 struct nodeList * next;    /*!< A pointer to the next element in the list*/
}nodeList;



/*! \struct list
* \brief list of tree nodes
*/
typedef struct{
	 nodeList *first; /*!< A pointer to the first list element*/
	 nodeList *current; /*!< A pointer to the current list element*/
	 nodeList *last; /*!< A pointer to the last list element*/
}list;







nodeList * newNodeList(void *e, void *n);
void deleteNodeList(nodeList * n);

void initList(list *l);
void deleteList(list *l);

int emptylist(list * l);
int first(list * l);
int last(list * l);
int outOfList(list * l);

void setOnFirst(list * l);
void setOnLast(list * l);
void next (list * l);
void * getCurrentTree(list * l);


/* Fonctions ,écessaire au insertSort */
int insertFirst(list *l, void *e);
int insertLast(list *l, void *e);
int insertAfterCurrent(list *l, void *e);
int insertBeforeCurrent(list *l, void *e);

nodeList * deleteFirst(list *l);
int  deleteLast(list *l);
int  deleteCurrent(list *l);



#endif
