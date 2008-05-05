// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "../systypes.h"
#include "../eventlist.h"

using namespace std;


// Prueba las listas de eventos

int main(int argc, char *argv[])
{
    EVENTLIST l;
    
    initEL(l);
    
    setEventEL(l, DC2, 12.1);
    setEventEL(l, AC1, 12.0);
    
    printf("AC1: %f\n", getTimeEL(l, AC1));
    printf("DC1: %f\n", getTimeEL(l, DC1));
    printf("AC2: %f\n", getTimeEL(l, AC2));
    printf("DC2: %f\n", getTimeEL(l, DC2));        
    
    
    EVENT e = getClosestEventEL(l);
    printf("instante temporal mas cercano: %f\n", getTimeEL(l, e));
    
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
