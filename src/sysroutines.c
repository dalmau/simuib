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

// probabilidades de ramificacion y salida del sistema
float p12 	= 0.3;
float p13	= 0.7;
float p30	= 0.4;
float p31	= 0.6;

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

// Acumuladores para calcular la varianza muestral
TIME RT1, RT2, RT3;			// tiempo de respuesta de cada estacion
TIME SQRTRT1, SQRTRT2, SQRTRT3;		// sumatorio con el cuadrado de los tiempos de respuesta de cada estacion

// Prepara el simulador para ejecutar una nueva simulacion.
//
void simulatorInit (double theLambda, double maxReplicationTime) {
	
	lambda = theLambda;
	MAXREPLICATIONTIME = maxReplicationTime;

	RT1 = RT2 = RT3 = INITIALTIME;
	SQRTRT1 = SQRTRT2 = SQRTRT3 = NULLTIME;

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
			case A2:
				arrival2();
				break;
			case D2:
				departure2();
				break;
			case A3:
				arrival3();
				break;
			case D3:
				departure3();
				break;
			default:
				break;
		}			
	}

	if (DEBUG) {
		printf("sysroutines.replicate(): Replication <%5i> - Served Station 1: <%5i>\n", REPS, served1);
		printf("sysroutines.replicate(): Replication <%5i> - Served Station 2: <%5i>\n", REPS, served2);
		printf("sysroutines.replicate(): Replication <%5i> - Served Station 3: <%5i>\n", REPS, served3);
		printf("sysroutines.replicate(): Replication <%5i> - Served System:    <%5i>\n", REPS, served);
		printf("sysroutines.replicate(): Replication <%5i> - RT Station 1: <%3.5f>\n", REPS, rTSum1/served1);
		printf("sysroutines.replicate(): Replication <%5i> - RT Station 2: <%3.5f>\n", REPS, rTSum2/served2);
		printf("sysroutines.replicate(): Replication <%5i> - RT Station 3: <%3.5f>\n", REPS, rTSum3/served3);
		printf("sysroutines.replicate(): Replication <%5i> - RT System   : <%3.5f>\n", REPS, (rTSum1+rTSum2+rTSum3)/served);		
	}

	// acumular los contadores estadisticos

	RT1 += rTSum1/(double)served1;
	//RT2 += accumulatedRTC2/(float)servedC2;
	//RTC  += (accumulatedRTC1+accumulatedRTC2)/(float)(servedC1+servedC2);
	SQRTRT1 += pow(rTSum1/(double)served1, 2);
	//SQRTRTC2 += pow(accumulatedRTC2/(float)servedC2, 2);
	//SQRTRTC  += pow((accumulatedRTC1+accumulatedRTC2)/(float)(servedC1+servedC2), 2);
}

// Rutinas para procesar los eventos de la red

// Llegada de un cliente al nodo 1
//
void arrival1 (void) {
	
	if (DEBUG)
		printf("sysroutines.arrival1(): \t<%3.2f> One client arrived.\n", clock);
	
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

	lastEvent = clock;

	if (DEBUG)
		showEL(eventList);
}

// Salida de un cliente del nodo 1
//
void departure1 (void) {

	if (DEBUG)
		printf("sysroutines.departure1(): \t<%3.2f> One client left.\n", clock);

	// actualizar contadores estadisticos (tiempo de servicio)
	rTSum1 += clock - getArrivalEL(eventList, D1);
	served1++;

	setDepartureEventEL(eventList, D1, NULLTIME, NULLTIME);
	if (emptyAL(&arrivals1)) {
		serverBusy1 = false;		
	}
	else {
		// seleccionar el siguiente de la cola para entrar al servidor
		setDepartureEventEL(eventList, D1, clock, clock + exponential(mu1));
		// actualizar contadores estadisticos (tiempo de espera)
		rTSum1 += clock - getArrivalAL(&arrivals1);
		// eliminarlo de la cola de espera
		delArrivalAL(&arrivals1);
	}

	// encaminar el cliente hacia la estacion 2 o la estacion 3
	if (urand(SEG) < p12) {
		if (DEBUG)
			printf("sysroutines.departure1(): The client will go to station 2.\n");
		
		// servidor libre
		if (!serverBusy2) {
			serverBusy2 = true;
			// marcar su instante de salida
			setDepartureEventEL(eventList, D2, clock, clock + exponential(mu2));
		}
		// poner el cliente nuevamente en cola
		else {
			addArrivalAL(&arrivals2, clock);
		}		
	}
	else {
		if (DEBUG)
			printf("sysroutines.departure1(): The client will go to station 3.\n");	

		// servidor libre
		if (!serverBusy3) {
			serverBusy3 = true;
			// marcar su instante de salida
			setDepartureEventEL(eventList, D3, clock, clock + exponential(mu3));
		}
		// poner el cliente nuevamente en cola
		else {
			addArrivalAL(&arrivals3, clock);
		}		
	}
	
	lastEvent = clock;

	if (DEBUG)
		showEL(eventList);		
}

// Llegada de un cliente al nodo 2.
//
void arrival2 (void) {

	if (DEBUG)
		printf("sysroutines.arrival2(): \t<%3.2f> One client arrived.\n", clock);
	
	// el servidor esta desocupado
	if (!serverBusy2) {
		serverBusy2 = true;
		// marcar su instante de salida
		setDepartureEventEL(eventList, D2, clock, clock + exponential(mu2));
	}
	// poner el cliente en cola
	else {
		addArrivalAL(&arrivals2, clock);
	}
	
	lastEvent = clock;

	if (DEBUG)
		showEL(eventList);
}

// Salida de un cliente del nodo 2.
//
void departure2 (void) {

	if (DEBUG)
		printf("sysroutines.departure2(): \t<%3.2f> One client left.\n", clock);

	// actualizar contadores estadisticos (tiempo de servicio)
	rTSum2 += clock - getArrivalEL(eventList, D2);
	served2++;

	setDepartureEventEL(eventList, D2, NULLTIME, NULLTIME);
	if (emptyAL(&arrivals2)) {
		serverBusy2 = false;		
	}
	else {
		// seleccionar el siguiente de la cola para entrar al servidor
		setDepartureEventEL(eventList, D2, clock, clock + exponential(mu2));
		// actualizar contadores estadisticos (tiempo de espera)
		rTSum2 += clock - getArrivalAL(&arrivals2);		
		// eliminarlo de la cola de espera
		delArrivalAL(&arrivals2);
	}

	// encaminar el cliente hacia la estacion 1
	if (DEBUG)
		printf("sysroutines.departure2(): The client goes now to station 1.\n");
		
	// servidor libre
	if (!serverBusy1) {
		serverBusy1 = true;
		// marcar su instante de salida
		setDepartureEventEL(eventList, D1, clock, clock + exponential(mu1));
	}
	// poner el cliente nuevamente en cola
	else {
		addArrivalAL(&arrivals1, clock);
	}		
	
	lastEvent = clock;

	if (DEBUG)
		showEL(eventList);	
}

// Llegada de un cliente al nodo 3.
//
void arrival3 (void) {

	if (DEBUG)
		printf("sysroutines.arrival3(): \t<%3.2f> One client arrived.\n", clock);
	
	// el servidor esta desocupado
	if (!serverBusy3) {
		serverBusy3 = true;
		// marcar su instante de salida
		setDepartureEventEL(eventList, D3, clock, clock + exponential(mu3));
	}
	// poner el cliente en cola
	else {
		addArrivalAL(&arrivals3, clock);
	}
	
	lastEvent = clock;

	if (DEBUG)
		showEL(eventList);	
}

// Salida de un cliente del nodo 3.
//
void departure3 (void) {

	if (DEBUG)
		printf("sysroutines.departure3(): \t<%3.2f> One client left.\n", clock);

	// actualizar contadores estadisticos (tiempo de servicio)
	rTSum3 += clock - getArrivalEL(eventList, D3);
	served3++;

	setDepartureEventEL(eventList, D3, NULLTIME, NULLTIME);
	if (emptyAL(&arrivals3)) {
		serverBusy3 = false;		
	}
	else {
		// seleccionar el siguiente de la cola para entrar al servidor
		setDepartureEventEL(eventList, D3, clock, clock + exponential(mu3));
		// actualizar contadores estadisticos (tiempo de espera)
		rTSum3 += clock - getArrivalAL(&arrivals3);		
		// eliminarlo de la cola de espera
		delArrivalAL(&arrivals3);
	}

	// encaminar el cliente hacia la estacion 1 o hacia afuera del sistema
	if (urand(SEG) < p30) {
		if (DEBUG)
			printf("sysroutines.departure3(): The client will go out of the system.\n");

		// actualizar contadores estadisticos		
		served++;
	}
	else {
		if (DEBUG)
			printf("sysroutines.departure3(): The client will go to station 1.\n");	

		// servidor libre
		if (!serverBusy1) {
			serverBusy1 = true;
			// marcar su instante de salida
			setDepartureEventEL(eventList, D1, clock, clock + exponential(mu1));
		}
		// poner el cliente nuevamente en cola
		else {
			addArrivalAL(&arrivals1, clock);
		}		
	}
	
	lastEvent = clock;

	if (DEBUG)
		showEL(eventList);		
}

