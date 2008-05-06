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

// Duracion de la Replica
TIME MAXREPLICATIONTIME;

// replicas EJECUTADAS
int REPS = 0;

// distribuciones utilizadas
double lambda;		// Con 0 < lambda < 0.02 para que la red sea estable
double mu1 	= 0.1;	// Tiempo medio servicio nodo 1 = 10 ms
double mu2 	= 0.06;	// Tiempo medio servicio nodo 2 = 15 ms
double mu3	= 0.05;	// Tiempo medio servicio nodo 3 = 20 ms

// Variables de estado del sistema
TIME clock;
EVENTLIST eventList;		// lista de eventos del sistema
bool serverBusy1, serverBusy2, serverBusy3; 	// ocupacion servidores de los 3 nodos
ARRIVALSLIST arrivals1, arrivals2, arrivals3;		// colas de los 3 nodos 
TIME lastEvent;			// instante en que se dio el ultimo evento

// Contadores estadisticos (para cada una de las replicas)
int served1, served2, served3;	// numero de clientes que han recibido servicio en cada estacion
int served;			// numero de clientes que han recibido servicio en el sistema (han salido)
TIME rTSum1, rTSum2, rTSum3;	// tiempo de respuesta acumulado para los clientes en cada estacion

// Prepara el simulador para ejecutar una nueva simulacion.
//
void simulatorInit (double theLambda, double maxReplicationTime) {
	
	lambda = theLambda;
	MAXREPLICATIONTIME = maxReplicationTime;

	if (DEBUG) {
		printf("sysroutines.simulatorInit(): Lambda equals to <%f>\n", lambda);
		printf("sysroutines.simulatorInit(): Time for each replication <%f>\n", MAXREPLICATIONTIME);
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

	serverBusy1 = serverBusy2 = serverBusy3 = false;
	initAL(&arrivals1);
	initAL(&arrivals2);
	initAL(&arrivals3);

	// contadores estadisticos
	served1 = served2 = served3 = 0;
	rTSum1 = rTSum2 = rTSum3 = INITIALTIME; 
}

// Rutina del temporizador que se encargara de determinar el siguiente evento a procesar, retorna
// el evento.
//
EVENT timer (void) {

	EVENT e = getClosestEventEL(eventList);
	clock = getTimeEL(eventList, e);

	if (DEBUG)
		printf("sysroutines.timer(): Event chosen will happen at <%3.2f>\n", clock);
	return e;	
}

// Ejecuta una replica en el simulador
//
void replicate (void) {

	REPS++;

	replicationInit();
	while (clock < MAXREPLICATIONTIME) {
		EVENT e = timer();
		switch (e) {
			case A1:
				arrival1();
				break;
				
			case D1:
				departure1();
				break;
			/*
			case AC2:
				arrivalC2();
				break;
			case DC2:
				departureC2(getArrivalEL(eventList, p), getServerEL(eventList, p));
				break;
			*/
			default:
				break;
		}			
	}
	/*
	// acumular los contadores estadisticos
	
	if (DEBUG) {
		printf("sysroutines.replicate(): Replication <%5i> - Response Time C1: <%3.5f>\n", REPS, accumulatedRTC1/servedC1);
		printf("sysroutines.replicate(): Replication <%5i> - Response Time C2: <%3.5f>\n", REPS, accumulatedRTC2/servedC2);
		printf("sysroutines.replicate(): Replication <%5i> - Utilization S1: <%3.5f>\n", REPS, areaBS1/(clock*SERVERSC1));
		printf("sysroutines.replicate(): Replication <%5i> - Utilization S2: <%3.5f>\n", REPS, areaBS2/(clock*SERVERSC2));
	}

	RTC1 += accumulatedRTC1/(float)servedC1;
	RTC2 += accumulatedRTC2/(float)servedC2;
	RTC  += (accumulatedRTC1+accumulatedRTC2)/(float)(servedC1+servedC2);
	MCNC1 += areaNC1/clock;
	MCNC2 += areaNC2/clock;
	MCN   += (areaNC1+areaNC2)/clock;
	MUS1 += areaBS1/(clock*SERVERSC1);
	MUS2 += areaBS2/(clock*SERVERSC2);
	MUS  += (areaBS1+areaBS2)/(clock*(SERVERSC1+SERVERSC2));
	SQRTRTC1 += pow(accumulatedRTC1/(float)servedC1, 2);
	SQRTRTC2 += pow(accumulatedRTC2/(float)servedC2, 2);
	SQRTRTC  += pow((accumulatedRTC1+accumulatedRTC2)/(float)(servedC1+servedC2), 2);
	SQRTMCNC1 += pow(areaNC1/clock, 2);
	SQRTMCNC2 += pow(areaNC2/clock, 2);
	SQRTMCN   += pow((areaNC1+areaNC2)/clock, 2);
	SQRTMUS1 += pow(areaBS1/(clock*SERVERSC1), 2);
	SQRTMUS2 += pow(areaBS2/(clock*SERVERSC2), 2);
	SQRTMUS  += pow((areaBS1+areaBS2)/(clock*(SERVERSC1+SERVERSC2)), 2);
	*/
}

// Rutinas para procesar los eventos de la red

// Llegada de un cliente al nodo 1
//
void arrival1 (void) {
	
	if (DEBUG)
		printf("sysroutines.arrival1(): \t<%3.2f> One client arrived.\n", clock);

	// actualizar contadores estadisticos
	//areaBS1 += (clock - lastEvent) * (float)busyS1;	
	
	// el servidor esta desocupado
	if (!serverBusy1) {
		serverBusy1 = true;
		// marcar su instante de salida
		setDepartureEventEL(eventList, D1, clock, clock + exponential(mu1));
	}
	// poner el cliente en cola
	else {
		addArrivalAL(&arrivals1, clock);
	}
	// siguiente llegada
	setArrivalEventEL(eventList, A1, clock + exponential(lambda));
	// actualizar contadores estadisticos
	//areaNC1 += (clock - lastEvent) * (float)activeC1;
	//activeC1++;
	lastEvent = clock;

	if (DEBUG)
		showEL(eventList);
}

// Salida de un cliente del nodo 1
//
void departure1 (void) {

	if (DEBUG)
		printf("sysroutines.departure1(): \t<%3.2f> One client left.\n", clock);

	// actualizar contadores estadisticos (tiempo de respuesta acumulado)
	//accumulatedRTC1 += clock - arrival;
	//if (DEBUG)
	//	printf("sysroutines.departureC1(): Response time: <%3.5f>\n", clock - arrival);

	//delArrivalAL(&queueC1);
	//servedC1++;
	// funcion B y funcion N
	//areaBS1 += (clock - lastEvent) * (float)busyS1;
	//areaNC1 += (clock - lastEvent) * (float)activeC1;
	//activeC1--;

	setDepartureEventEL(eventList, D1, NULLTIME, NULLTIME);
	if (emptyAL(&arrivals1)) {
		serverBusy1 = false;		
	}
	else {
		// seleccionar el siguiente de la cola para entrar al servidor
		setDepartureEventEL(eventList, D1, getArrivalAL(&arrivals1), clock + exponential(mu1));
		// eliminarlo de la cola de espera
		delArrivalAL(&arrivals1);
	}
/*
	// determinar si el cliente sale del sistema o vuelve a solicitar servicio
	if (urand(SEG) >= pOutC1) {
		if (DEBUG)
			printf("sysroutines.departureC1(): The client will enter the system again.\n");	
		activeC1++;
		// servidor libre
		if (busyS1 < SERVERSC1) {
			busyS1++;
			// marcar su instante de salida
			setDepartureEventEL(eventList, DC1, clock, clock + exponencial(muC1));
		}
		// poner el cliente nuevamente en cola
		// los contadores estadisticos ya fueron actualizados para el nuevo valor de clock
		else {
			addArrivalAL(&queueC1, clock);
		}
	}
*/	
	lastEvent = clock;

	//if (DEBUG)
	//	printf("sysroutines.departureC1(): Busy servers C1: <%i> of %i\n", busyS1, SERVERSC1);	

	if (DEBUG)
		showEL(eventList);		
}

