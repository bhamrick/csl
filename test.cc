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
	matrix m(N,M), p, q;
	for(int i = 0; i<N; i++) {
		for(int j = 0; j<M; j++) {
			fscanf(fin,"%d",&m.get(i,j));
		}
	}
	cout << m << endl;
	smithNormalForm(m,p,q);
	for(int i = 0; i<m.rows() && i < m.cols(); i++) {
		printf("%d\n",m.get(i,i));
	}
	cout << p << endl;
	cout << q << endl;
	return 0;
}
