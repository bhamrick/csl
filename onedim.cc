#include<simplex.hh>
#include<vector>
#include<queue>
#include<map>

using namespace std;

vector< vector<int> > generators1;
int *component;
int *par;
vector<int> *adj;

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Usage: %s filename\n",argv[0]);
		return 1;
	}
	FILE *fin = fopen(argv[1],"r");
	int N, M, dim;
	fscanf(fin,"%d",&dim);
	if(dim != 1) {
		printf("Error: %s is only for 1-simplices\n",argv[0]);
		return 1;
	}
	fscanf(fin,"%d",&N);
	double x, y, z;
	for(int i = 0; i<N; i++) {
		fscanf(fin,"%lf%lf%lf",&x,&y,&z);
	}
	component = new int[N];
	adj = new vector<int>[N];
	par = new int[N];
	for(int i = 0; i<N; i++) {
		component[i] = i;
	}
	fscanf(fin,"%d",&M);
	for(int i = 0; i<M; i++) {
		int a, b;
		fscanf(fin,"%d%d",&a,&b);
		if(component[a] == component[b]) {
			for(int j = 0; j<N; j++) {
				par[j] = -1;
			}
			par[b] = a;
			queue<int> q;
			q.push(b);
			while(!q.empty()) {
				int v = q.front();
				q.pop();
				for(int j = 0; j<adj[v].size(); j++) {
					if(par[adj[v][j]]==-1) {
						par[adj[v][j]] = v;
						q.push(adj[v][j]);
					}
				}
			}
			generators1.push_back(vector<int>());
			int v = a;
			do {
				generators1[generators1.size()-1].push_back(v);
				v = par[v];
			} while(v!=a);
			generators1[generators1.size()-1].push_back(a);
		} else {
			int oldcomp = component[b];
			for(int j = 0; j<N; j++) {
				if(component[j] == oldcomp) {
					component[j] = component[a];
				}
			}
		}
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	printf("Components (0-generators):\n");
	for(int i = 0; i<N; i++) {
		if(component[i] == i) {
			printf("%d\n",i);
		}
	}
	printf("\nIndependent Cycles (1-generators):\n");
	for(int i = 0; i<generators1.size(); i++) {
		printf("%d",generators1[i][0]);
		for(int j = 1; j<generators1[i].size(); j++) {
			printf("--%d",generators1[i][j]);
		}
		printf("\n");
	}
}
