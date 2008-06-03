// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#include "report.h"
#include "systypes.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Libreria de soporte a la presentacion del informe estadistico. Contiene todas las funciones auxiliares necesarias
// para poder presentar al usuario los resultados obtenidos en terminos del intervalo de confianza.

// Acceso a las variables externas de <sysroutines> para el calculo del error relativo.
extern TIME RT1, RT2, RT3;			// tiempo de respuesta de cada estacion
extern TIME SQRTRT1, SQRTRT2, SQRTRT3;		// sumatorio con el cuadrado de los tiempos de respuesta de cada estacion

// Variables para mostrar los resultados de la simulacion
TIME MRT1, MRT2, MRT3;
TIME CIRT1, CIRT2, CIRT3;

// Percentiles 0.975 de la distribucion t de student para distintos grados de
// libertad
//
double t_0975[] =
{
        12.71, 4.30, 3.18, 2.78, 2.57, 2.45, 2.36, 2.31, 2.26, 2.23, 		// dof = 1..30
        2.20, 2.18, 2.16, 2.14, 2.13, 2.12, 2.11, 2.10, 2.09, 2.09, 
        2.08, 2.07, 2.07, 2.06, 2.06, 2.06, 2.05, 2.05, 2.04, 2.04,
        2.02, 									// dof = 40
        2.01, 									// dof = 50
        2.00, 									// dof = 60
        1.98, 									// dof = 120
        1.96  									// dof = infinite
};

// Calcula la varianza muestral aplicando tomando para ello n como el numero de muestras, samples como el sumatorio
// de los cuadrados de las muestras y mean como la media muestral de las mismas muestras.
//
double sampleVariance (int n, TIME samples, TIME mean) {
	
	return (samples - (n * pow(mean, 2))) / (n - 1);
}

// Calcula el intervalo de confianza teniendo en cuenta que n es el numero de muestras, samples el sumatorio
// de los cuadrados de las muestras y mean la media muestral de las mismas muestras.
//
double confidenceInterval (int n, TIME samples, TIME mean) {

	return tstudent(n - 1) * sqrt(sampleVariance(n, samples, mean) / n);
}

// Retorna el valor percentil de la distribucion t de student para n grados de libertad.
//
double tstudent (int dof) {

	double t;

        if (0 < dof && dof <= 30) {
                t = t_0975[dof - 1];
        } else if (dof <= 40) {
                t = t_0975[30];
        } else if (dof <= 50) {
                t = t_0975[31];
        } else if (dof <= 60) {
                t = t_0975[32];
        } else if (dof <= 120) {
                t = t_0975[33];
        } else if (dof > 120) {
                t = t_0975[34];
        } else {
                printf("report.tstudent(): Invalid value for the degrees of freedom.");
                exit(EXIT_FAILURE);
        }
        return t;
}

// Comprueba si el error relativo es adecuado  para todos los parametros que se manejan en la simulacion, tiempo de respuesta,
// numero medio de clientes y utilizacion en funcion del valor n que representa el numero de replicas.
// Devuelve un valor falso si los cocientes de todos los intervalos de confianza y las medias muestrales son un valor menor 
// o igual a la cota de error (corregido) manejado por el simulador.
//
bool largeRelativeError (int n) {	

	// tiempo de respuesta nodo 1
	TIME meanRT1 	= RT1 / n;
	TIME ciRT1 	    = confidenceInterval(n, SQRTRT1, meanRT1);

	if ((ciRT1 / meanRT1) > RELATIVEERROR)
		return true;

	// tiempo de respuesta nodo 2
	TIME meanRT2 	= RT2 / n;  
	TIME ciRT2 	= confidenceInterval(n, SQRTRT2, meanRT2);

	if ((ciRT2 / meanRT2) > RELATIVEERROR)
		return true;

	// tiempo de respuesta nodo 3
	TIME meanRT3 	= RT3 / n;  
	TIME ciRT3 	= confidenceInterval(n, SQRTRT3, meanRT3);

	if ((ciRT3 / meanRT3) > RELATIVEERROR)
		return true;		

	// el error relativo ya no es demasiado grande
	MRT1 = meanRT1;
	MRT2 = meanRT2;
	MRT3 = meanRT3;
	CIRT1 = ciRT1;
	CIRT2 = ciRT2;
	CIRT3 = ciRT3;

	return false;
}

// Muestra los resultados obtenidos en la simulacion
// El parametro n es el numero de replicas ejecutadas
//
void showResults (int n) {

	printf("\nreport.showResults():   Mean Values:\n");
	printf("report.showResults():   ------------\n");
	printf("report.showResults():   Response Time Station 1:\t%f\n", MRT1);
	printf("report.showResults():   Response Time Station 2:\t%f\n", MRT2);
	printf("report.showResults():   Response Time Station 3:\t%f\n", MRT3);   	
	printf("\n");
    printf("report.showResults(): 	Confidence Intervals:\n");
	printf("report.showResults(): 	---------------------\n");
	printf("report.showResults(): 	Executed Replications:\t\t%i\n", n);
	printf("report.showResults(): 	Confidence Level:\t\t95%%\n");
	printf("report.showResults(): 	Response Time Station 1:\t%f\n", CIRT1);
	printf("report.showResults(): 	Response Time Station 2:\t%f\n", CIRT2);
	printf("report.showResults(): 	Response Time Station 3:\t%f\n", CIRT3);    
}
