// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#ifndef _SYSROUTINES_H_
#define _SYSROUTINES_H_

#include "eventlist.h"

void simulatorInit (double theLambda, double maxReplicationTime);
void replicationInit (void);
void replicate (void);
EVENT timer (void);
// eventos
void arrival1 (void);
void departure1 (void);

#endif // _SYSROUTINES_H_
