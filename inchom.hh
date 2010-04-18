#ifndef INCHOM_H
#define INCHOM_H

#include<algorithm>
#include<bezout.hh>
#include<map>
#include<queue>
#include<vector>

class scomplex {
	private:
	std::map< int, std::map<int,int> > cycles; // fundamental cycles
	std::map< int, std::map<int,int> > cyclerep; // representation in terms of generators
	std::map< int, int > sdim; // dimension of simplices
	std::vector< std::map<int,int> > generators[3]; // generators
	std::vector<int> torsion[3]; // torsion coefficients
	std::vector< std::vector<int> > simplices; // vertices of simplices
	std::vector< std::vector<int> > boundary; // bounding simplices
	std::vector< std::vector<int> > coboundary; // cobounding simplices
	std::map< std::vector<int>,int > simplex_lookup; // lookup by sorted vertex list
	int dim;

	int get_id(std::vector<int>);
	void quotient(int, std::map<int,int>);

	public:
	scomplex(int);
	void add_simplex(int,std::vector<int>&);
	std::vector< std::map<int,int> > get_generators(int);
	std::vector<int> get_torsion(int);
	std::vector<int> get_vertices(int);
};

#endif
