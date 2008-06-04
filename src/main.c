// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <stdio.h>
#include <stdlib.h>

#include "sysroutines.h"
#include "report.h"

int main(int argc, char *argv[]) {
	
	if (argc != 5) {
		printf("main.main(): ERROR there's at least one missing parameter\n");
		printf("main.main(): Usage: simulator lambda_value replication_time min_reps_number debug(0/1)\n");
		exit(EXIT_FAILURE);
	}
	DEBUG = (strcmp(argv[4], "1")==0)?true:false;
	
	// inicializacion del simulador
	simulatorInit(atof(argv[1]), atof(argv[2]));

	// Aplicacion del metodo secuencial con error relativo		
    
    // Numero minimo de replicas que ejecutamos
    int i;
    for (i=0; i<atoi(argv[3]); i++)
        replicate();

	int n = atoi(argv[3]);
	
	while (largeRelativeError(n)) {
		replicate();
		n++;
	}		

    // mostrar los resultados
	showResults(n);	

	return 0;
}
