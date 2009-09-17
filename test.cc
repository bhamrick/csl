#include<matrix.hh>
#include<bezout.hh>
#include<iostream>
#include<cstdlib>
#include<cstdio>

using namespace std;

int main(int argc, char** argv) {
	int N, M;
	FILE *fin = fopen("matrix.in","r");
	fscanf(fin,"%d%d",&N,&M);
	matrix m(N,M), p, q;
	for(int i = 0; i<N; i++) {
		for(int j = 0; j<M; j++) {
			int a;
			fscanf(fin,"%d",&a);
			m.get(i,j) = a;
		}
	}
	cout<<m<<endl;
	smithNormalForm(m,p,q);
	cout<<m<<endl;
}
