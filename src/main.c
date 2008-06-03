// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <stdio.h>
#include <stdlib.h>

#include "sysroutines.h"
#include "report.h"

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
    
    // Como minimo ejecutamos dos replicas
	replicate();
	replicate();
	int n = 2;
	
	while (largeRelativeError(n)) {
		replicate();
		n++;
	}		

    // mostrar los resultados
	showResults(n);	

	return 0;
}
