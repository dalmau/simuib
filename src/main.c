// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <stdio.h>
#include <stdlib.h>

//
#include "systypes.h"
#include "arrivalslist.h"
#include "eventlist.h"
//

int main(int argc, char *argv[]) {
	
	if (argc != 4) {
		printf("main.main(): ERROR there's at least one missing parameter\n");
		printf("main.main(): Usage: simulator lambda_value replication_time debug(0/1)\n");
		exit(EXIT_FAILURE);
	}
	DEBUG = (strcmp(argv[3], "1")==0)?true:false;
	
	simulatorInit(atof(argv[1]), atof(argv[2]));
	replicate();

	//system("PAUSE");	
	return 0;
}
