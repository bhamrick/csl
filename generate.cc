#include<ctime>
#include<cstdio>
#include<cstdlib>
#include<unistd.h>

using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL)*getpid());
	int N = 5, M = 5;
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
