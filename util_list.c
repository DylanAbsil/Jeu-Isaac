/*! Un module liste qui peut etre adapt� selon vos besoin */



#include "util_list.h"


nodeList * newNodeList(void *e, void *n)
{
		nodeList* new = malloc(sizeof(nodeList));
		if (new == NULL)
		{
			return NULL;
		}
		new->e = e;
		new->next = n;
		return new;
	
}


void deleteNodeList(nodeList * n)
{
	/* On supprime (d�sallouer) l'arbre associ� au noeud � supprimer -> switch suivant le type de n->e */
	/*switch ()
	{
	default:
		break;
	}*/
	/* On libere la m�moire prise par le noeud qu'on veut delete */
	UTIL_Free(n);
}




void initList(list * l)
{
	l->first = l->last = l->current = NULL;
}

/* Suppression de ou l'espace m�moire utilis�e par la liste. On fait donc un pacours de la liste pour delete cacun de ses noeuds */
void deleteList(list *l)
{
	setOnFirst(l);
	while (l->current != NULL)
	{
		/* On se place sur le suivant */
		next(l);
		/* On supprime le premier noeud de la liste */
		deleteNodeList(l->first);
		/* le deuxieme devient le premier*/
		l->first = l->current;
		/* Et on recommence */
	}
	/* A la fin de la boucle on a suppr tous les noeuds */
	
}



int emptyList(list* l)
{
	return (l->first == NULL);
}


int first(list *l)
{
	return (l->current->next == l->first->next);
}


int last(list *l)
{
	return (l->current->next == l->last->next);
}


int outOfList(list *l)
{
	return ((l->current == NULL) && !emptyList(l));
}


void setOnFirst(list *l)
{
	l->current = l->first;
}


void setOnLast(list *l)
{
	l->current = l->last;
}


void next(list *l)
{
	l->current = l->current->next;
}


void * getCurrentElement(list * l)
{
	l->current->e;
}



int insertFirst(list *l, void* e)
{
	nodeList* new = newNodeList(e, l->first);
	if (new == NULL)
	{
		return 0;
	}

	if (emptyList(l))
	{
		l->last = l->current = new;
	}

	l->first = new;

	return 1;

}



int insertLast(list *l, void *e)
{
	nodeList* new = newNodeList(e, NULL);
	if (new == NULL)
	{
		return 0;
	}

	if (emptyList(l))
	{
		l->first = l->current = new;
	}

	l->last->next = new;
	l->last = new;
	new->next = NULL;

	return 1;
}



int insertAfterCurrent(list *l, void *e){
	if (emptyList(l))
	{
		insertFirst(l, e);
		l->current = l->last = l->first;
	}
	else
	{
		if (l->current == l->last)
			insertLast(l, e);
		else
		{
			nodeList* new = newNodeList(e, l->current->next);
			if (new == NULL)
				return 0;
			if (emptyList(l))
				l->first = l->current = l->last = new;
			l->current->next = new;
			return 1;
		}
	}
}




int  insertBeforeCurrent(list *l, void *e)
{
	/* On cr� un nouveau noeud dont le next sera le current avant lequel on insere ce noiuveau noeud */
	nodeList* new = newNodeList(e, l->current);
	if (new == NULL)
		return 0;

	if (emptyList(l))
		l->first = l->current = l->last = new;

	/* Si il n'y a qu'un seul �l�ment, ou que le current est d�j� le premier �l�ment, alors on peut faire un insertFirst() */
	if (l->first == l->last || l->current==l->first)
	{
		insertFirst(l, e);
		return 1;
	}

	/* Pour changer l'adresse de l'�l�ment avant le current, on se place au d�but de la liste,
	que l'on parcours ensuite jusqu'� tomb� sur le noeud pr�c�dant le current (en regardant son next).
	On change alors l'adresse de cet �l�ment.
	Puis on repositionne le curretn comme il �tait avant grace
	*/
	nodeList* nodeTmp = l->current;
	setOnFirst(l);
	while (l->current->next != nodeTmp)
	{
		next(l);
	}
	l->current->next = new;
	next(l);
	next(l);

	return 1;
}




/*
Si le current est le last actuel, on place le current sur l'avant dernier.
Dans tous les cas on met le pointeur next de l'avant dernier a NULL.

*/

nodeList * deleteFirst(list * l)
{
	nodeList* nodeTmp = l->first;
	l->first = l->first->next;
	deleteNodeList(l->first);
	return nodeTmp;
}


int  deleteLast(list *l)
{
	nodeList* nodeTmp = l->current;

	if (l->current = l->last)
	{
		nodeList* nodeTmp = l->current;
		setOnFirst(l);
		while (l->current->next != nodeTmp)
		{
			next(l);
		}
		l->current->next = NULL;
		l->last = l->current;
	}
	else
	{
		while (l->current->next != nodeTmp)
		{
			next(l);
		}
		l->current->next = NULL;
	}
	deleteNodeList(nodeTmp);
}


int  deleteCurrent(list *l)
{

	nodeList* nodeTmp = l->current;

	if (l->current == l->first)
		deleteFirst(l);

	if (l->current == l->last)
		deleteLast(l);

	else
	{
		setOnFirst(l);
		while (l->current->next != nodeTmp)
		{
			next(l);
		}
		l->current->next = nodeTmp->next;
		deleteNodeList(nodeTmp);
	}


}






