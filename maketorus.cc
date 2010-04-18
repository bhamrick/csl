#include<cmath>
#include<cstdio>
#include<cstdlib>

using namespace std;

int **id;
const double pi = 3.141592653589793238463;

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Usage: %s N\nThis program creates a torus triangulated as an N by N square with opposite sides associated (thus it will contain a total of 6*N^2 simplices) and writes it to torusN.in\n",argv[0]);
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
	for(int i = 0; i<N; i++) {
		id[N][i] = id[0][i];
		id[i][N] = id[i][0];
	}
	char *fname = new char[20];
	sprintf(fname,"torus%d.in",N);
	FILE *fout = fopen(fname,"w");
	fprintf(fout,"2\n");
	fprintf(fout,"%d\n",N*N);
	for(int i = 0; i<N*N; i++) {
		int a = i/N, b = i%N;
		double u = 2*pi*a/N, v = 2*pi*b/N;
		double x = cos(u)*(1 - 0.3 * cos(v)), y = sin(u)*(1-0.3 * cos(v)), z = 0.3*sin(v);
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
