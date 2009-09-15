#include<matrix.hh>
#include<bezout.hh>
#include<iostream>
#include<cstdlib>
#include<cstdio>

using namespace std;

int main(int argc, char** argv) {
	matrix m(4,4);
	for(int i = 0; i<4; i++) {
		for(int j = 0; j<4; j++) {
			m.get(i,j) = 4*i+j;
		}
	}
	cout<<m<<endl;
	while(true) {
		int a, b, x, y;
		cin >> a >> b;
		int g = gcd(a,b,x,y);
		cout << g << " = " << a << " * " << x << " + " << b << " * " << y << endl;
	}
}
