#include<matrix.hh>
#include<unistd.h>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<gmp.h>

#define MINN 2
#define MAXN 50
#define TRIALS 5
#define MAXC 5
#define MINTIME 5

int main(int argc, char** argv) {
	FILE *fout = fopen("times.txt","w");
	srand(time(NULL)*getpid());
	for(int i = MINN; i<=MAXN; i++) {
		printf("%d\n",i);
		time_t start, end;
		time(&start);
		int ntrials = TRIALS;
		for(int j = 0; j<TRIALS; j++) {
			mpz_matrix m(i,i), p, q;
			for(int a = 0; a<i; a++) {
				for(int b = 0; b<i; b++) {
					mpz_set_si(m.get(a,b),rand()%(2*MAXC+1)-MAXC);
				}
			}
			smithNormalForm(m,p,q);
		}
		time(&end);
		while(difftime(end,start) < MINTIME) {
			mpz_matrix m(i,i), p, q;
			ntrials++;
			for(int a = 0; a<i; a++) {
				for(int b = 0; b<i; b++) {
					mpz_set_si(m.get(a,b),rand()%(2*MAXC+1)-MAXC);
				}
			}
			smithNormalForm(m,p,q);
			time(&end);
		}
		fprintf(fout,"%d %f\n",i,difftime(end,start)/ntrials);
	}
	fclose(fout);
	return 0;
}
