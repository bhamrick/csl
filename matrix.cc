#include<matrix.hh>

using namespace std;

matrix::matrix() {
	n=0;
	m=0;
	a=NULL;
}

matrix::matrix(int n, int m) {
	this->n = n;
	this->m = m;
	a = (int*)malloc(n*m*sizeof(int));
	for(int i = 0; i<n*m; i++) a[i] = 0;
}

matrix::~matrix() {
	free(a);
}

int matrix::rows() {
	return n;
}

int matrix::cols() {
	return m;
}

void matrix::setRows(int n) {
	setDims(n,m);
}

void matrix::setCols(int m) {
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
}

int& matrix::get(int i, int j) {
	if(i < 0 || i >= n || j < 0 || j >= m) return zero;
	return *(a+(i*m+j));
}

matrix& matrix::operator=(matrix& other) {
	if(&other == this) return *this;
	n = other.rows();
	m = other.cols();
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
}
