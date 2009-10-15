#ifndef SIMPLEX_H
#define SIMPLEX_H

#include<cstdio>
#include<cstdlib>
#include<vector>
#include<algorithm>

class point {
	public:
	double x, y, z;
};

class simplex {
	public:
	int dim;
	std::vector<int> verts;
	bool operator==(const simplex other) {
		return dim == other.dim && verts == other.verts;
	}
};

class scomplex {
	public:
	int dim;
	std::vector<point*> points;
	std::vector<simplex*> simplices;
};

#endif
