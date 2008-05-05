// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "../systypes.h"
#include "../arrivalslist.h"

using namespace std;

// test que comprueba el funcionamiento de las listas de tiempos de llegada

int main(int argc, char *argv[])
{
    ARRIVALSLIST l;
    
    initAL(&l);
    
    addArrivalAL(&l, 12.0);
    addArrivalAL(&l, 13.0);
    addArrivalAL(&l, 6.0);
    addArrivalAL(&l, 8.0);
    addArrivalAL(&l, 99999.0);
    
    showAL(&l);
    
    printf("\n\n");
    
    delArrivalAL(&l);
    delArrivalAL(&l);
    delArrivalAL(&l);
    delArrivalAL(&l);

    
    showAL(&l);
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
