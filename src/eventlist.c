// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "eventlist.h"
#include "systypes.h"
#include "sysroutines.h"

// Implementacion de la lista de eventos a usar en el simulador

// variables para almacenar los tiempos de llegada de los clientes que estan usando los servidores
//TIME arrivalsS1[SERVERSC1];
//TIME arrivalsS2[SERVERSC2];

// Inicializa la lista de eventos estableciendo un valor temporal nulo para todos los eventos considerados.
//
void initEL (EVENTLIST l) {

	int i;
	for (i=0; i<EVENTLISTSIZE; i++)
		l[i] = NULLTIME;
}

// Determina si una lista de eventos esta vacia o no
//
bool emptyEL (EVENTLIST l) {

	int i = 0;
	while (i < EVENTLISTSIZE) {
		if (l[i] != NULLTIME)
			return false;
		i++;
	}
	return true;
}

// Establece el valor temporal t para el evento de llegada e en la lista de eventos l.
// El evento podra ser la llegada de un cliente a uno de los 3 nodos de la red.
//
void setArrivalEventEL (EVENTLIST l, EVENT e, TIME t) {

	l[e] = t;
}

// Establece el valor temporal d para el evento de salida e en la lista de eventos l.
// Se buscara un servidor disponible segun el tipo de evento y se anotara el instante de llegada a del cliente
// al sistema.
//
void setDepartureEventEL (EVENTLIST l, EVENT e, TIME a, TIME d) {

	// instante de salida
	l[e] = d;
	// almacenar el instante de llegada del cliente
	// TODO
}

// Retorna la posicion cuyo instante temporal es mas proximo al valor actual del reloj del simulador.
//
EVENT getClosestEventEL (EVENTLIST l) {

	if (emptyEL(l)) {
		printf("eventlist.getClosestEventEL(): ERROR, the event list is empty\n");
		exit(EXIT_FAILURE);
	}

	int e = 1;
	int chosen = 0;

	while (e < EVENTLISTSIZE) {
		if (((l[e] != NULLTIME)&&(l[e] < l[chosen]))||(l[chosen] == NULLTIME))
			chosen = e;
		e++;
	}
	
	if (DEBUG)
		printf("eventlist.getClosestEventEL(): position chosen <%i> with time <%3.2f>\n", chosen, l[chosen]);

	return chosen;
}

// Retorna el instante temporal asociado al evento e en la lista de eventos l.
//
TIME getTimeEL (EVENTLIST l, EVENT e) {

	return l[e];
}

char *eventToString (EVENT e) {

	switch (e) {
		case A1:
			return "A1";
			break;
		case A2:
			return "A2";
			break;
		case A3:
			return "A3";
			break;
		case D1:
			return "D1";
			break;
		case D2:
			return "D2";
			break;
		case D3:
			return "D3";
			break;			
	}	
}
// Muestra el contenido de una lista de eventos.
//
void showEL (EVENTLIST l) {
	
	int i;
	for (i = 0; i < EVENTLISTSIZE; i++) {
		printf("eventlist.showEL(): %s ", eventToString(i));	
		if (l[i] == NULLTIME)
			printf("<EMPTY>\n");
		else
			printf("<%f>\n", l[i]);
	}
}

