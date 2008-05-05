// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

// Contiene la definición de tipos usados en el sistema

#ifndef _SYSTYPES_H_
#define _SYSTYPES_H_

#include <stdbool.h>

// Tiempo
typedef float TIME;

// Tiempos de llegada
// nodos de la lista
struct _NODEAL;
typedef struct _NODEAL{
	TIME t;
	struct NODEAL *next;
}NODEAL;
// lista de tiempos de llegada
typedef struct _ARRIVALSLIST {
	NODEAL *first;
	NODEAL *last;
}ARRIVALSLIST;

#endif // _SYSTYPES_H_
