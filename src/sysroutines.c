// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <stdio.h>
#include <math.h>
#include "sysroutines.h"
#include "systypes.h"
#include "arrivalslist.h"
#include "eventlist.h"
#include "rand.h"

// rutinas para los eventos considerados en el sistema

// distribuciones utilizadas
float lambda;		// Con 0 < lambda < 0.02 para que la red sea estable
float mu1 	= 0.1;	// Tiempo medio servicio nodo 1 = 10 ms
float mu2 	= 0.06;	// Tiempo medio servicio nodo 2 = 15 ms
float mu3	= 0.05;	// Tiempo medio servicio nodo 3 = 20 ms

// Variables de estado del sistema
TIME clock;
EVENTLIST eventList;		// lista de eventos del sistema
bool serverStatus1, serverStatus2, serverStatus3; 	// ocupacion servidores de los 3 nodos
ARRIVALSLIST arrivals1, arrivals2, arrivals3;		// colas de los 3 nodos 
TIME lastEvent;			// instante en que se dio el ultimo evento

// Contadores estadisticos (para cada una de las replicas)
int served1, served2, served3;	// numero de clientes que han recibido servicio en cada estacion
int served;			// numero de clientes que han recibido servicio en el sistema (han salido)
TIME rTSum1, rTSum2, rTSum3;	// tiempo de respuesta acumulado para los clientes en cada estacion

// Prepara el simulador para ejecutar una nueva simulacion.
//
void simulatorInit (float theLambda) {
	
	lambda = theLambda;
	printf("simulatorinit: %f, %f\n", theLambda, 0.005);
	if (DEBUG) {
		printf("sysroutines.simulatorInit(): Lambda equals to <%f>", lambda);
	}
}

// Prepara el simulador para ejecutar una nueva replica
//
void replicationInit (void) {
	
	clock = INITIALTIME;
	lastEvent = INITIALTIME;

	// configurar la lista de eventos
	initEL(eventList);
	setArrivalEventEL(eventList, A1, exponential(lambda));
	
	if (DEBUG)
       		showEL(eventList);

	serverStatus1 = serverStatus2 = serverStatus3 = 0;
	initAL(&arrivals1);
	initAL(&arrivals2);
	initAL(&arrivals3);

	// contadores estadisticos
	served1 = served2 = served3 = 0;
	rTSum1 = rTSum2 = rTSum3 = INITIALTIME; 
}

