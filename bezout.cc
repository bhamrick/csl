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

void gcd(mpz_t g, mpz_t a, mpz_t b, mpz_t x, mpz_t y) {
	mpz_t ax, ay, bx, by, q, r, tx, ty, ta, tb;
	mpz_init(ta);
	mpz_init(tb);
	mpz_set(ta,a);
	mpz_set(tb,b);
	mpz_init(ax);
	mpz_init(ay);
	mpz_init(bx);
	mpz_init(by);
	mpz_init(q);
	mpz_init(r);
	mpz_init(tx);
	mpz_init(ty);
	mpz_set_si(ax,1);
	mpz_set_si(ay,0);
	mpz_set_si(bx,0);
	mpz_set_si(by,1);
	while(mpz_sgn(b) != 0) {
		mpz_cdiv_qr(q,r,a,b);
		mpz_set(tx,ax);
		mpz_set(ty,ay);
		mpz_submul(tx,q,bx);
		mpz_submul(ty,q,by);
		mpz_set(a,b);
		mpz_set(b,r);
		mpz_set(ax,bx);
		mpz_set(ay,by);
		mpz_set(bx,tx);
		mpz_set(by,ty);
	}
	mpz_set(x,ax);
	mpz_set(y,ay);
	if(mpz_sgn(a) < 0) {
		mpz_neg(a,a);
		mpz_neg(x,x);
		mpz_neg(y,y);
	}
	mpz_clear(ax);
	mpz_clear(ay);
	mpz_clear(bx);
	mpz_clear(by);
	mpz_clear(q);
	mpz_clear(r);
	mpz_clear(tx);
	mpz_clear(ty);
	mpz_set(g,a);
	mpz_set(a,ta);
	mpz_set(b,tb);
	mpz_clear(ta);
	mpz_clear(tb);
}
