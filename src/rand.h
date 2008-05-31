// $LastChangedDate$
// $LastChangedRevision$
// $LastChangedBy$

#ifndef RAND_H_
#define RAND_H_

#define SEG 7        // segmento usado

float urand(int stream);
void urandst(long zset, int stream);
long urandgt(int stream);
double exponential(double lambda);

#endif // RAND_H_

