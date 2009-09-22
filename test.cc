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
	mpz_matrix m(N,M), p, q;
	matrix m2(N,M), p2, q2;
	for(int i = 0; i<N; i++) {
		for(int j = 0; j<M; j++) {
			mpz_inp_str(m.get(i,j),fin,10);
			m2.get(i,j) = mpz_get_si(m.get(i,j));
		}
	}
	m.write(NULL);
	smithNormalForm(m,p,q);
	smithNormalForm(m2,p2,q2);
	for(int i = 0; i<m.rows() && i < m.cols(); i++) {
		gmp_printf("%Zd\t%d\n",m.get(i,i),m2.get(i,i));
	}
	return 0;
}
