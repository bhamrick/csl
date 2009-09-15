#include<bezout.hh>

int gcd(int a, int b, int& x, int& y) {
	int ax=1, ay=0, bx=0, by=1;
	while(b != 0) {
		int q = a/b, r = a%b;
		int tx = ax - q*bx, ty = ay-q*by;
		a = b;
		b = r;
		ax = bx;
		ay = by;
		bx = tx;
		by = ty;
	}
	x = ax;
	y = ay;
	if(a < 0) {
		a = -a;
		x = -x;
		y = -y;
	}
	return a;
}
