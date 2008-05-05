// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#ifndef _ARRIVALSLIST_H_
#define _ARRIVALSLIST_H_

#include "systypes.h"

// implementacion

// nodos de la lista
struct NODEAL{
	TIME t;
	struct NODEAL *next;
};
typedef struct NODEAL NODEAL;

// lista de tiempos de llegada
typedef struct ARRIVALSLIST {
	NODEAL *first;
	NODEAL *last;
}ARRIVALSLIST;


// operaciones
void initAL (ARRIVALSLIST *l);
bool emptyAL (ARRIVALSLIST *l);
void addArrivalAL (ARRIVALSLIST *l, TIME t);
void delArrivalAL (ARRIVALSLIST *l);
TIME getArrivalAL (ARRIVALSLIST *l);
void showAL (ARRIVALSLIST *l);

#endif // _ARRIVALSLIST_H_
