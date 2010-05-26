#include<ctime>
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
	FILE *output = fopen("output.asy","w");
	FILE *times = fopen("times.dat","w");
	double viewx=6, viewy=8, viewz=4;
	timespec tp1, tp2;
	int count = 0;
	fprintf(output,"import settings;\nimport three;\n\noutformat=\"pdf\";\n\nsize(200);\n\ncurrentprojection=orthographic(%lf,%lf,%lf);\n\n",viewx,viewy,viewz);
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
		clock_gettime(CLOCK_REALTIME,&tp1);
		com->add_simplex(0,verts);
		clock_gettime(CLOCK_REALTIME,&tp2);
		count++;
		fprintf(times,"%d %lld\n",count,((long long)tp2.tv_sec*1000000000ll + tp2.tv_nsec) - ((long long)tp1.tv_sec*1000000000ll + tp1.tv_nsec));
	}
	for(int d = 1; d<=dim; d++) {
		fscanf(fin,"%d",&N[d]);
		for(int i = 0; i<N[d]; i++) {
			fprintf(stderr,"%d %d\n",d,i);
			vector<int> verts;
			for(int j = 0; j<=d; j++) {
				int v;
				fscanf(fin,"%d",&v);
				verts.push_back(v);
			}
			clock_gettime(CLOCK_REALTIME,&tp1);
			com->add_simplex(d,verts);
			clock_gettime(CLOCK_REALTIME,&tp2);
			if(d == 1) {
				fprintf(output,"draw((%lf,%lf,%lf)--(%lf,%lf,%lf));\n",coords[verts[0]][0],coords[verts[0]][1],coords[verts[0]][2],coords[verts[1]][0],coords[verts[1]][1],coords[verts[1]][2]);
			}
			count++;
			fprintf(times,"%d %lld\n",count,((long long)tp2.tv_sec*1000000000ll + tp2.tv_nsec) - ((long long)tp1.tv_sec*1000000000ll + tp1.tv_nsec));
		}
	}
	fprintf(output,"save();\n");
	fprintf(output,"shipout(\"input\");\n");
	vector<int> torsion = com->get_torsion(1);
	vector< map<int,int> > generators = com->get_generators(1);
	int index = 0;
	for(int i = 0; i<torsion.size(); i++) {
		if(torsion[i] != 1 && torsion[i] != -1) {
			printf("%d:",torsion[i]);
			index++;
			fprintf(output,"restore();\nsave();\n");
			for(map<int,int>::iterator iter = generators[i].begin(); iter != generators[i].end(); iter++) {
				//printf(" %d[%d]",(*iter).second,(*iter).first);
				if((*iter).second != 0) {
					printf("\t%d--%d",com->get_vertices((*iter).first)[0],com->get_vertices((*iter).first)[1]);
					int v1 = com->get_vertices((*iter).first)[0], v2 = com->get_vertices((*iter).first)[1];
					fprintf(output,"draw((%lf,%lf,%lf)--(%lf,%lf,%lf),red+2);\n",coords[v1][0],coords[v1][1],coords[v1][2],coords[v2][0],coords[v2][1],coords[v2][2]);
				}
			}
			fprintf(output,"shipout(\"output%d\");\n",index);
			printf("\n");
		}
	}
	fclose(fin);
	fclose(output);
	return 0;
}
