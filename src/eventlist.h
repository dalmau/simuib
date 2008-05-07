// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#ifndef _EVENTLIST_H_
#define _EVENTLIST_H_

#include "systypes.h"
// Eventos
#define A1 0	// llegada de un cliente al nodo 1
#define A2 1	// llegada de un cliente al nodo 2
#define A3 2	// llegada de un cliente al nodo 3
#define D1 3	// salida de un cliente del nodo 1
#define D2 4	// salida de un cliente del nodo 2
#define D3 5	// salida de un cliente del nodo 3
#define EVENTLISTSIZE 6

// implementacion

typedef int EVENT;

typedef TIME EVENTLIST[EVENTLISTSIZE];
typedef int POSITIONEL;

// operaciones
void initEL (EVENTLIST l);
bool emptyEL (EVENTLIST l);
void setArrivalEventEL (EVENTLIST l, EVENT e, TIME t);
void setDepartureEventEL (EVENTLIST l, EVENT e, TIME a, TIME d);
EVENT getClosestEventEL (EVENTLIST l);
TIME getTimeEL (EVENTLIST l, EVENT e);
TIME getArrivalEL (EVENTLIST l, EVENT e);
void showEL (EVENTLIST l);

#endif // _EVENTLIST_H_

