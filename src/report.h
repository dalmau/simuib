// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#ifndef _REPORT_H_
#define _REPORT_H_

#include "systypes.h"

double sampleVariance (int n, TIME samples, TIME mean);
double confidenceInterval (int n, TIME samples, TIME mean);
double tstudent (int dof);
bool largeRelativeError (int n);

#endif // _REPORT_H_

