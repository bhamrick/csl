#include<matrix.hh>
#include<bezout.hh>
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<gmp.h>

using namespace std;

int main(int argc, char** argv) {
	int N, M;
	FILE *fin = fopen("matrix.in","r");
	fscanf(fin,"%d%d",&N,&M);
	mpz_t a, b, c, x, y;
	mpz_init(a);
	mpz_init(b);
	mpz_init(c);
	mpz_init(x);
	mpz_init(y);
	char *s = (char*)malloc(1000);
	scanf("%s",s);
	mpz_set_str(a,s,10);
	scanf("%s",s);
	mpz_set_str(b,s,10);
	mpz_gcdext(c,x,y,a,b);
	mpz_out_str(NULL,10,c);
	printf("\n");
	mpz_out_str(NULL,10,x);
	printf(" ");
	mpz_out_str(NULL,10,y);
	printf("\n");
	return 0;
}
