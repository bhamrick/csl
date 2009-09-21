#ifndef MATRIX_H
#define MATRIX_H

#include<iostream>
#include<cstdlib>
#include<cstdio>

class matrix {
	private:
	int *a, n, m, zero; // n rows, m columns
	public:
	matrix();
	matrix(int,int);
	~matrix();
	int rows();
	int cols();
	void setRows(int);
	void setCols(int);
	void setDims(int,int);
	int& get(int,int);
	matrix& operator=(matrix&);
};

std::ostream& operator<<(std::ostream&,matrix&);
void smithNormalForm(matrix&, matrix&, matrix&);

class mpz_matrix {
	private:
	mpz_t *a, zero;
	int n, m;
	public:
	mpz_matrix();
	mpz_matrix(int,int);
	~mpz_matrix();
	int rows();
	int cols();
	void setRows(int);
	void setCols(int);
	void setDims(int,int);
	mpz_t get(int,int);
};

#endif
