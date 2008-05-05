// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <stdio.h>
#include <stdlib.h>
#include "arrivalslist.h"

// Implementacion dinamica de las listas de instantes de llegadas

// Inicializa la lista de llegadas transformandola en una lista vacia
//
void initAL (ARRIVALSLIST *l) {

	l->first = NULL;
	l->last = NULL;	
}

// Comprueba si la lista de llegadas esta vacia
//
bool emptyAL (ARRIVALSLIST *l) {

	if (l->first == NULL)
		return true;
	return false;
}

// Añade un nuevo tiempo de llegadas a la lista al final de la misma
//
void addArrivalAL (ARRIVALSLIST *l, TIME t) {

	// lista vacia
	if (l->first == NULL) {
		l->first = (NODEAL *)malloc(sizeof(NODEAL));
		(l->first)->t = t;
		(l->first)->next = NULL;
		l->last = l->first;
	}
	// ya tenia elementos
	else {
		(l->last)->next = (NODEAL *)malloc(sizeof(NODEAL));
		((l->last)->next)->t = t;
		((l->last)->next)->next = NULL;
		l->last = (l->last)->next;
	}
}

// Elimina el primer elemento de la lista si la lista no esta vacia
//
void delArrivalAL (ARRIVALSLIST *l) {

	if (l->first == NULL) {
		printf("arrivalslist.delArrivalAL(): ERROR, the list is empty\n");
		exit(EXIT_FAILURE);
	}
	
	// la lista solo tiene un elemento
	if (l->first == l->last) {
		free(l->first);
		l->first = NULL;
		l->last = NULL;
	}
	// hay mas de un elemento
	else {
		NODEAL *n = l->first;
		l->first = (l->first)->next;
		free(n);
	}
}

// Obtiene el tiempo de llegada del cliente que ocupa la primera posicion de la cola
//
TIME getArrivalAL (ARRIVALSLIST *l) {

	if (l->first == NULL) {
		printf("arrivalslist.getArrivalAL(): ERROR, the list is empty\n");
		exit(EXIT_FAILURE);
	}
	
	return (l->first)->t;
}

// Muestra el contenido de la lista de llegadas
//
void showAL (ARRIVALSLIST *l) {
	
	if (!emptyAL(l)) {
		NODEAL *p = l->first;
		while (p != NULL) {
			printf("arrivalslist.showAL(): <%f>\n", p->t);
			p = p->next;	
		}
	}
}

