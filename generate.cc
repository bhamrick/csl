#include<ctime>
#include<cstdio>
#include<cstdlib>
#include<unistd.h>

using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL)*getpid());
	int N, M;
	if(argc < 2) {
		return 0;
	} else if(argc == 2) {
		sscanf(argv[1],"%d",&N);
		M = N;
	} else {
		sscanf(argv[1],"%d",&N);
		sscanf(argv[2],"%d",&M);
	}
	printf("%d %d\n",N,M);
	for(int i = 0; i<N; i++) {
		printf("%d",rand()%21-10);
		for(int j = 1; j<M; j++) {
			printf(" %d",rand()%21-10);
		}
		printf("\n");
	}
	return 0;
}
