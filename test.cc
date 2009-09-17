#include<matrix.hh>
#include<bezout.hh>
#include<iostream>
#include<cstdlib>
#include<cstdio>

using namespace std;

int main(int argc, char** argv) {
	matrix m(2,3), p, q;
	m.get(0,0) = 8;
	m.get(0,1) = 4;
	m.get(0,2) = 8;
	m.get(1,0) = 4;
	m.get(1,1) = 8;
	m.get(1,2) = 4;
	cout<<m<<endl;
	smithNormalForm(m,p,q);
	cout<<m<<endl;
}
