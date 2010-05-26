#include<cmath>
#include<cstdio>
#include<cstdlib>

using namespace std;

int **id;
const double pi = 3.141592653589793238463;

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Usage: %s N\nThis program creates a klein bottle triangulated as an N by N square with opposite sides associated (thus it will contain a total of 6*N^2 simplices) and writes it to kleinN.in\n",argv[0]);
	}
	int N;
	sscanf(argv[1],"%d",&N);
	id = new int*[N+1];
	for(int i = 0; i<N+1; i++) {
		id[i] = new int[N+1];
	}
	for(int i = 0; i<N; i++) {
		for(int j = 0; j<N; j++) {
			id[i][j] = N*i + j;
		}
	}
	id[N][0] = id[0][N] = 0;
	for(int i = 0; i<N; i++) {
		id[N][i] = id[0][i];
	}
	for(int i = 0; i<=N; i++) {
		id[i][N] = id[(N-i+N/2)%N][0];
	}
	FILE *schem = fopen("schematic.asy","w");
	fprintf(schem,"import settings;\noutformat=\"pdf\";\nsize(200);\n");
	for(int i = 0; i<N+1; i++) {
		for(int j = 0; j<N+1; j++) {
			fprintf(schem,"dot((%d,%d));\n",j,-i);
			fprintf(schem,"label(\"%d\",(%d,%d),NE);\n",id[i][j],j,-i);
		}
	}
	for(int i = 0; i<N; i++) {
		for(int j = 0; j<N; j++) {
			fprintf(schem,"draw((%d,%d)--(%d,%d));\n",j,-i,j+1,-i);
			fprintf(schem,"draw((%d,%d)--(%d,%d));\n",j,-i,j+1,-i-1);
			fprintf(schem,"draw((%d,%d)--(%d,%d));\n",j,-i,j,-i-1);
		}
	}
	for(int i = 0; i<N; i++) {
		fprintf(schem,"draw((%d,%d)--(%d,%d));\n",i,-N,i+1,-N);
		fprintf(schem,"draw((%d,%d)--(%d,%d));\n",N,-i,N,-i-1);
	}
	fclose(schem);
	char *fname = new char[20];
	sprintf(fname,"klein%d.in",N);
	FILE *fout = fopen(fname,"w");
	fprintf(fout,"2\n");
	fprintf(fout,"%d\n",N*N);
	for(int i = 0; i<N*N; i++) {
		int a = i/N, b = i%N;
		double x,y,z;
		double u = 2*pi*a/N, v = 4*pi*b/N;
		if(v < pi) {
			x = (2.5 - 1.5*cos(v))*cos(u);
			y = (2.5 - 1.5*cos(v))*sin(u);
			z = -2.5*sin(v);
		} else if(v < 2*pi) {
			x = (2.5 - 1.5*cos(v))*cos(u);
			y = (2.5 - 1.5*cos(v))*sin(u);
			z = 3*v - 3*pi;
		} else if(v < 3*pi) {
			x = -2 + (2+cos(u))*cos(v);
			y = sin(u);
			z = (2+cos(u))*sin(v) + 3*pi;
		} else {
			x = -2 + 2*cos(v) - cos(u);
			y = sin(u);
			z = -3*v + 12*pi;
		}
//		double u = 2*pi*a/N, v = 2*pi*b/N;
//		double r = 1.0;
//		if(u < pi) {
//			x = 6*cos(u)*(1+sin(u)) + 4*r*(1-cos(u)/2)*cos(u)*cos(v);
//			y = 16*sin(u) + 4*r*(1-cos(u)/2)*sin(u)*cos(v);
//			z = 4*r*(1-cos(u)/2)*sin(v);
//		} else {
//			x = 6*cos(u)*(1+sin(u)) - 4*r*(1-cos(u)/2)*cos(v);
//			y = 16*sin(u);
//			z = 4*r*(1-cos(u)/2)*sin(v);
//		}
		x/=15; y/=15; z/=15;
		fprintf(fout,"%lf %lf %lf\n",x,y,z);
	}
	fprintf(fout,"%d\n",3*N*N);
	for(int i = 0; i<N; i++) {
		for(int j = 0; j<N; j++) {
			fprintf(fout,"%d %d\n",id[i][j],id[i+1][j]);
			fprintf(fout,"%d %d\n",id[i][j],id[i+1][j+1]);
			fprintf(fout,"%d %d\n",id[i][j],id[i][j+1]);
		}
	}
	fprintf(fout,"%d\n",2*N*N);
	for(int i = 0; i<N; i++) {
		for(int j = 0; j<N; j++) {
			fprintf(fout,"%d %d %d\n",id[i][j],id[i][j+1],id[i+1][j+1]);
			fprintf(fout,"%d %d %d\n",id[i][j],id[i+1][j],id[i+1][j+1]);
		}
	}
	fclose(fout);
	return 0;
}
