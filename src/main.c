// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <stdio.h>
#include <stdlib.h>

//
#include "systypes.h"
#include "arrivalslist.h"
#include "eventlist.h"
#include "report.h"
//

// Acceso a las variables externas de <sysroutines> para el calculo del error relativo.
extern TIME RT1, RT2, RT3;			// tiempo de respuesta de cada estacion
extern TIME SQRTRT1, SQRTRT2, SQRTRT3;		// sumatorio con el cuadrado de los tiempos de respuesta de cada estacion

int main(int argc, char *argv[]) {
	
	if (argc != 4) {
		printf("main.main(): ERROR there's at least one missing parameter\n");
		printf("main.main(): Usage: simulator lambda_value replication_time debug(0/1)\n");
		exit(EXIT_FAILURE);
	}
	DEBUG = (strcmp(argv[3], "1")==0)?true:false;
	
	// inicializacion del simulador
	simulatorInit(atof(argv[1]), atof(argv[2]));

	// Aplicacion del metodo secuencial con error relativo
	// ejecucion de las replicas iniciales
	//int i;
	//for (i=0; i<MINREPLICATIONS; i++)
	//for (i=0; i<10; i++)
	//	replicate();	
	
	//printf("Varianza: %f\n", sampleVariance(10, SQRTRT1, RT1/10));
    
	replicate();
	replicate();
	int n = 2;
	while (largeRelativeError(n)) {
		replicate();
		n++;
	}	
	

//	showResults();	

	//system("PAUSE");	
	return 0;
}
