// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

// Contiene la definición de tipos usados en el sistema

#ifndef _SYSTYPES_H_
#define _SYSTYPES_H_

#include <stdbool.h>

// Debug
bool DEBUG;

// Tiempo
typedef double TIME;
#define NULLTIME -1.0
#define INITIALTIME 0.0

// Error relativo
#define ERRORLEVEL 0.1 					// por defecto del 10%
#define RELATIVEERROR ERRORLEVEL / (1.0 + ERRORLEVEL)	// error corregido
#define MINREPLICATIONS 10				// numero minimo inicial 
							//para el metodo secuencial

#endif // _SYSTYPES_H_
