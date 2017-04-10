// 
// This file contains programs for generating random numbers with
// uniform distributions and exponential distributions.
//
#include <math.h>

double Seed = 1111.0;

/*******************************************/
/* returns a uniform (0,1) random variable */
/*******************************************/
double uni_rv()           
{
    double k = 16807.0;
    double m = 2.147483647e9;
    double rv;

    Seed=fmod((k*Seed),m);	
    rv=Seed/m;
    return(rv);
}

/*******************************/
/* given arrival rate lambda   */
/* returns an exponential r.v. */ 
/*******************************/
double exp_rv(double lambda)
{
    double exp;
    exp = ((-1) / lambda) * log(uni_rv());
    return(exp);
}
