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
	
	if (argc != 3) {
		printf("main.main(): ERROR there's at least one missing parameter");
		printf("main.main(): Usage: simulator lambda_value debug(0/1)");
		exit(EXIT_FAILURE);
	}
	DEBUG = (strcmp(argv[2], "1")==0)?true:false;
	
	float l = (float)atof(argv[1]);
	printf("%s\n", argv[1]);
	printf("%f\n", l);
	simulatorInit(0.006f);  	

	//system("PAUSE");	
	return 0;
}
