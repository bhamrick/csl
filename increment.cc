#define CURSES
//#define NDEBUG

#include<simplex.hh>
#include<vector>
#include<queue>
#include<map>
#include<csignal>

#ifdef CURSES

#include<curses.h>

#endif

using namespace std;

#ifndef NDEBUG
void pause() {

#ifdef CURSES
	getch();
#endif

}
#else
void pause() {}
#endif

void cleanexit(int code) {

#ifdef CURSES
	endwin();
#endif
	exit(code);

}

void show(int dim, vector< vector<int> > *generators) {
	move(0,0);
	for(int d = 0; d<=dim; d++) {
		printw("H%d generators\n",d);
		for(int i = 0; i<generators[d].size(); i++) {
			printw("%d",generators[d][i][0]);
			for(int j = 1; j<generators[d][i].size(); j++) {
				printw("-%d",generators[d][i][j]);
			}
			printw("\n");
		}
		printw("\n");
	}
}

int get_id(vector<int>);

vector< vector<int> > cycles;
vector< vector<int> > *generators;
vector< vector<int> > simplices;
vector< vector<int> > boundary;
vector< vector<int> > coboundary;
map< vector<int>,int > simplex_lookup;

int main(int argc, char** argv) {
	
#ifdef CURSES
	initscr(); cbreak(); noecho();
	nonl(); intrflush(stdscr, FALSE);
	keypad(stdscr,TRUE);
#endif

	signal(SIGABRT, &cleanexit);
	signal(SIGTERM, &cleanexit);
	signal(SIGINT, &cleanexit);

	if(argc < 2) {
		printf("Usage: %s filename\n",argv[0]);
		return 1;
	}
	FILE *fin = fopen(argv[1],"r");
	int N[3], dim;
	fscanf(fin,"%d",&dim);
	
	fscanf(fin,"%d",&N[0]);
	generators = new vector< vector<int> >[dim+1];
	for(int i = 0; i < N[0]; i++) {
		double x, y, z;
		fscanf(fin,"%lf%lf%lf",&x,&y,&z);
		
		generators[0].push_back(vector<int>());
		generators[0][i].push_back(i);
		
		erase();
		show(dim,generators);
		
		vector<int> verts;
		verts.push_back(i);
		
		simplices.push_back(verts);
		boundary.push_back(vector<int>());
		coboundary.push_back(vector<int>());
		simplex_lookup[verts] = i;
		
		pause();
	}
	int off = 0;
	for(int d = 1; d <= dim; d++) {
		fscanf(fin,"%d",&N[d]);
		off+=N[d-1];
		for(int i = 0; i<N[d]; i++) {
			boundary.push_back(vector<int>());
			coboundary.push_back(vector<int>());
		
			int id = off + i;

			erase();
			printw("Working on simplex %d (dimension %d)\n",id,d);
			
			vector<int> verts;
			for(int j = 0; j<d+1; j++) {
				int v;
				fscanf(fin,"%d",&v);
				verts.push_back(v);
			}
			
			sort(verts.begin(),verts.end());
			
			simplex_lookup[verts] = id;
			
			//compute coboundary
			for(int j = 0; j<d+1; j++) {
				vector<int> bverts;
				for(int k = 0; k<d+1; k++) {
					if(j!=k) {
						bverts.push_back(verts[k]);
					}
				}
				int bound = simplex_lookup[bverts];
				if(j & 1) bound = ~bound;
				printw("Bounding simplex: %d\n",bound);
				if(bound < 0) {
					coboundary[~bound].push_back(~id);
				} else {
					coboundary[bound].push_back(id);
				}
				boundary[id].push_back(bound);
			}
			
			// split into cases
			if(d == 1) { // 1-simplex
				// detect cycle
				int par[N[0]];
				for(int i = 0; i<N[0]; i++) {
					par[i] = -1;
				}
				queue<int> q;
				int a = boundary[id][0];
				int b = boundary[id][1];
				par[~b] = a;
				q.push(~b);
				while(!q.empty()) {
					int v = q.front();
					q.pop();
					//printw("%d %d\n",v,coboundary[v].size());
					for(int j = 0; j<coboundary[v].size(); j++) {
						int s = coboundary[v][j], o;
						bool neg = (s < 0);
						if(neg) s = ~s;
						if(s == id) continue; // ignore edge being added
						if(boundary[s][0] == v || boundary[s][0] == ~v) {
							o = boundary[s][1];
						} else {
							o = boundary[s][0];
						}
						if(o < 0) o = ~o;
						if(par[o]==-1) {
							par[o] = v;
							q.push(o);
						}
					}
				}
				if(par[a]==-1) {
					// No cycle
					printw("No cycle\n");
				} else {
					// Cycle
					printw("Cycle detected\n");
					vector<int> cyc;
					int v = a;
					do {
						vector<int> sim;
						sim.push_back(v);
						sim.push_back(par[v]);
						int simid = get_id(sim);
						printw("%d: %d %d\n",simid,v,par[v]);
						cyc.push_back(simid);
						v = par[v];
					} while(v != a);
					cycles.push_back(cyc);
				}
			} else if(d == 2) { // 2-simplex
				// detect cycle
			}
			pause();
		}
	}

	cleanexit(0);
}

int get_id(vector<int> verts) {
	// compute sign of permutation
	int N = verts.size();
	bool neg = 0;
	for(int i = 0; i<N-1; i++) {
		for(int j = i+1; j<N; j++) {
			if(verts[i] > verts[j]) neg = !neg;
		}
	}
	// sort vertices
	sort(verts.begin(),verts.end());
	// do a lookup
	int id = simplex_lookup[verts];
	if(neg) return ~id;
	else return id;
}
