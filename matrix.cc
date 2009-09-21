#include<matrix.hh>
#include<bezout.hh>

using namespace std;

matrix::matrix() {
	n=0;
	m=0;
	a=NULL;
}

mpz_matrix::mpz_matrix() {
	n=0;
	m=0;
	a=NULL;
	mpz_init(zero);
}

matrix::matrix(int n, int m) {
	this->n = n;
	this->m = m;
	a = (int*)malloc(n*m*sizeof(int));
	for(int i = 0; i<n*m; i++) a[i]=0;
}

mpz_matrix::mpz_matrix(int n, int m) {
	this->n = n;
	this->m = m;
	a = (mpz_t*)malloc(n*m*sizeof(mpz_t));
	for(int i = 0; i<n*m; i++) mpz_init(a[i]);
	mpz_init(zero);
}

mpz_matrix::~mpz_matrix() {
	mpz_clear(zero);
	for(int i = 0; i<n*m; i++) {
		mpz_clear(a[i]);
	}
	free(a);
}

int matrix::rows() {
	return n;
}

int mpz_matrix::rows() {
	return n;
}

int matrix::cols() {
	return m;
}

int mpz_matrix::cols() {
	return m;
}

void matrix::setRows(int n) {
	setDims(n,m);
}

void mpz_matrix::setRows(int n) {
	setDims(n,m);
}

void matrix::setCols(int m) {
	setDims(n,m);
}

void mpz_matrix::setCols(int m) {
	setDims(n,m);
}

void matrix::setDims(int n, int m) {
	int *t = (int*)malloc(n*m*sizeof(int));
	for(int i = 0; i<n; i++) {
		for(int j = 0; j<m; j++) {
			if(i < this->n && j < this->m) {
				t[i*m+j]=a[i*this->m+j];
			} else {
				t[i*m+j]=0;
			}
		}
	}
	free(a);
	a = t;
	this->n = n;
	this->m = m;
}

void mpz_matrix::setDims(int n, int m) {
	mpz_t *t = (mpz_t*)malloc(n*m*sizeof(mpz_t));
	for(int i = 0; i<n*m; i++) {
		mpz_init(t[i]);
	}
	for(int i = 0; i<n; i++) {
		for(int j = 0; j<m; j++) {
			if(i < this->n && j < this->m
		}
	}
}

int& matrix::get(int i, int j) {
	if(i < 0 || i >= n || j < 0 || j >= m) return zero;
	return *(a+(i*m+j));
}

matrix& matrix::operator=(matrix& other) {
	if(&other == this) return *this;
	setDims(other.n,other.m);
	for(int i = 0; i<n; i++) {
		for(int j = 0; j<m; j++) {
			get(i,j) = other.get(i,j);
		}
	}
	return *this;
}

ostream& operator<<(ostream& out, matrix& m) {
	out<<endl;
	for(int i = 0; i<m.rows(); i++) {
		if(i==0) out<<"[ [";
		else out<<"  [";
		for(int j = 0; j<m.cols(); j++) {
			out<<" ";
			out.width(8);
			out<<m.get(i,j);
		}
		if(i==m.rows()-1) out<<"\t] ]"<<endl;
		else out<<"\t]"<<endl;
	}
	return out;
}

void smithNormalForm(matrix &a, matrix &p, matrix &q) {
	int n = a.rows(), m = a.cols();
	matrix tp(n,n), tq(m,m);
	p=tp;
	q=tq;
	for(int i = 0; i<n && i<m; i++) {
		bool found = false;
		//choose a pivot and move it to a_i,i
		for(int j = i; j<m; j++) {
			bool brk = false;
			for(int k = i; k<n; k++) {
				if(a.get(k,j) != 0) {
					found = true;
					//switch column i with column j
					for(int l = i; l<n; l++) {
						int t = a.get(l,i);
						a.get(l,i) = a.get(l,j);
						a.get(l,j) = t;
					}
					//switch row i with row k
					for(int l = i; l<m; l++) {
						int t = a.get(i,l);
						a.get(i,l) = a.get(k,l);
						a.get(k,l) = t;
					}
					brk = true;
					break;
				}
			}
			if(brk) break;
		}
		if(!found) break;
		//improve the pivot
		bool done = false;
		while(!done) {
			done = true;
			//improve row-wise
			for(int j = i+1; j<n; j++) {
				if(a.get(j,i) % a.get(i,i) == 0) {
					//if divisible, clear it
					int q = a.get(j,i) / a.get(i,i);
					for(int k = i; k<m; k++) {
						a.get(j,k) -= q*a.get(i,k);
					}
				} else {
					//if not divisible, make it divisible and clear it and mark flag to continue computation
					done = false;
					int e, x, y, alpha, beta;
					e = gcd(a.get(i,i),a.get(j,i),x,y);
					alpha = a.get(i,i)/e;
					beta = a.get(j,i)/e;
					for(int k = i; k<m; k++) {
						int c = a.get(i,k), d = a.get(j,k);
						a.get(i,k) = c*x + d*y;
						a.get(j,k) = -c*beta + d*alpha;
					}
					int q = a.get(j,i) / a.get(i,i);
					for(int k = i; k<m; k++) {
						a.get(j,k) -= q*a.get(i,k);
					}
				}
			}
			//improve column-wise
			for(int j = i+1; j<m; j++) {
				if(a.get(i,j) % a.get(i,i) == 0) {
					//if divisible, clear it
					int q = a.get(i,j) / a.get(i,i);
					for(int k = i; k<n; k++) {
						a.get(k,j) -= q*a.get(k,i);
					}
				} else {
					//if not divisible, make it divisible and clear it and mark flag to continue computation
					done = false;
					int e, x, y, alpha, beta;
					e = gcd(a.get(i,i),a.get(i,j),x,y);
					alpha = a.get(i,i)/e;
					beta = a.get(i,j)/e;
					for(int k = i; k<n; k++) {
						int c = a.get(k,i), d = a.get(k,j);
						a.get(k,i) = c*x + d*y;
						a.get(k,j) = -c*beta + d*alpha;
					}
					int q = a.get(i,j) / a.get(i,i);
					for(int k = i; k<n; k++) {
						a.get(k,j) -= q*a.get(k,i);
					}
				}
			}
		}
		if(a.get(i,i) < 0) a.get(i,i) *= -1;
	}
	for(int i = 1; i<n && i<m; i++) {
		for(int j = i; j>0; j--) {
			if(a.get(j,j) % a.get(j-1,j-1) == 0) break;
			else if(a.get(j-1,j-1) % a.get(j,j) == 0) {
				int t = a.get(j,j);
				a.get(j,j) = a.get(j-1,j-1);
				a.get(j-1,j-1) = t;
			} else {
				int q = a.get(j-1,j-1), r = a.get(j,j), x, y;
				int e = gcd(q,r,x,y);
				a.get(j-1,j-1) = e;
				a.get(j,j) = q*r/e;
			}
		}
	}
}
