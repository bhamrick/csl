#ifndef BEZOUT_H
#define BEZOUT_H

#include<cstdio>
#include<cstdarg>
#include<gmp.h>

int gcd(int,int,int&,int&);
void gcd(mpz_t,mpz_t,mpz_t,mpz_t,mpz_t);

#endif
