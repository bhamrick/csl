#include<inchom.hh>
#include<cstdio>
#include<cstdlib>

using namespace std;

double **coords;
scomplex *com;

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Usage: %s filename\n",argv[0]);
		return 1;
	}
	FILE *fin = fopen(argv[1],"r");
	int N[3], dim;
	fscanf(fin,"%d",&dim);
	if(dim > 2) return 1;

	com = new scomplex(dim);

	fscanf(fin,"%d",&N[0]);
	coords = new double*[N[0]];
	for(int i = 0; i<N[0]; i++) {
		coords[i] = new double[3];
		fscanf(fin,"%lf%lf%lf",&coords[i][0],&coords[i][1],&coords[i][2]);
		vector<int> verts;
		verts.push_back(i);
		com->add_simplex(0,verts);
	}
	for(int d = 1; d<=dim; d++) {
		fscanf(fin,"%d",&N[d]);
		for(int i = 0; i<N[d]; i++) {
			printf("%d %d\n",d,i);
			vector<int> verts;
			for(int j = 0; j<=d; j++) {
				int v;
				fscanf(fin,"%d",&v);
				verts.push_back(v);
			}
			com->add_simplex(d,verts);
		}
	}
	vector<int> torsion = com->get_torsion(1);
	vector< map<int,int> > generators = com->get_generators(1);
	for(int i = 0; i<torsion.size(); i++) {
		if(torsion[i] != 1 && torsion[i] != -1) {
			printf("%d:",torsion[i]);
			for(map<int,int>::iterator iter = generators[i].begin(); iter != generators[i].end(); iter++) {
				//printf(" %d[%d]",(*iter).second,(*iter).first);
				printf("\t%d--%d",com->get_vertices((*iter).first)[0],com->get_vertices((*iter).first)[1]);
			}
			printf("\n");
		}
	}
	fclose(fin);
	return 0;
}
