#define CURSES
//#define NDEBUG

//#include<simplex.hh>
#include<bezout.hh>
#include<vector>
#include<queue>
#include<map>
#include<csignal>
#include<algorithm>

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

map< int, map<int,int> > cycles; // fundamental cycles
map< int, map<int,int> > cyclerep; // representation in terms of generators
map< int, int > sdim; // dimension of simplices
vector< map<int,int> > *generators; // generators
vector<int> *torsion; // torsion coefficients
vector< vector<int> > simplices; // vertices of simplices
vector< vector<int> > boundary; // bounding simplices
vector< vector<int> > coboundary; // cobounding simplices
map< vector<int>,int > simplex_lookup; // lookup by sorted vertex list
int dim;

void show() {
/*
	printw("Fundamental Cycles:\n");
	for(map< int,map<int,int> >::iterator iter = cycles.begin(); iter!=cycles.end(); iter++) {
		printw("<%d>:",(*iter).first);
		for(map<int,int>::iterator it = (*iter).second.begin(); it != (*iter).second.end(); it++) {
			if((*it).second != 0) printw(" %d[%d]",(*it).second,(*it).first);
		}
		printw(" =");
		for(map<int,int>::iterator it = cyclerep[(*iter).first].begin(); it!=cyclerep[(*iter).first].end(); it++) {
			if((*it).second != 0) printw(" %d(%d)",(*it).second,(*it).first);
		}
		printw("\n");
	}
*/	for(int d = 0; d<=dim; d++) {
		printw("H%d generators\n",d);
		for(int i = 0; i<generators[d].size(); i++) {
			map<int,int> cyc;
			for(map<int,int>::iterator iter = generators[d][i].begin(); iter!=generators[d][i].end(); iter++) {
				for(map<int,int>::iterator it = cycles[(*iter).first].begin(); it != cycles[(*iter).first].end(); it++) {
					cyc[(*it).first] = cyc[(*it).first] + (*it).second * (*iter).second;
				}
			}
			if(torsion[d][i] != 1 && torsion[d][i] != -1) {
				printw("%d: ",i);
				for(map<int,int>::iterator iter = cyc.begin(); iter != cyc.end(); iter++) {
					if((*iter).second != 0) printw("%d[%d] ",(*iter).second,(*iter).first);
				}
				printw("Torsion: %d\n",torsion[d][i]);
			}
		}
	}
}

int get_id(vector<int>);

void quotient(int d, map<int,int> rel) {
	printw("Quotient H_%d by 0=",d);
	for(map<int,int>::iterator iter = rel.begin(); iter!=rel.end(); iter++) {
		printw("%d(%d) ",(*iter).second,(*iter).first);
	}
	printw("\n");
	// quotient H_d by the relation specified by rel = 0
	int N = generators[d].size();
	map< int, map<int,int> > mat;
	for(int i = 0; i<N; i++) {
		mat[i][i] = torsion[d][i];
	}
	for(map<int,int>::iterator iter = rel.begin(); iter != rel.end(); iter++) {
		mat[N][(*iter).first] = (*iter).second;
	}
//	for(int i = 0; i<N+1; i++) {
//		for(int j = 0; j<N; j++) {
//			printw("%d ",mat[i][j]);
//		}
//		printw("\n");
//	}
//	printw("\n");
	//Smith Normal Form code -- does not use sparseness
	for(int i = 0; i<N; i++) {
//		for(int ii = 0; ii<N+1; ii++) {
//			for(int jj = 0; jj<N; jj++) {
//				printw("%d ",mat[ii][jj]);
//			}
//			printw("\n");
//		}
//		printw("\n");
//		printw("%d %d\n",i,mat[i][i]);
		bool found = false;
		//choose a pivot and move it to a_i,i
		for(int j = i; j<N; j++) {
			bool brk = false;
			for(int k = i; k<N+1; k++) {
				if(mat[k][j]!=0) {
					found = true;
//					printw("Pivot: %d %d\n",k,j);
					//switch column i with column j
					for(int l = i; l<N+1; l++) {
						int t = mat[l][i];
						mat[l][i] = mat[l][j];
						mat[l][j] = t;
					}
					//switch generators i and j
					map<int,int> gi(generators[d][i]);
					generators[d][i] = generators[d][j];
					generators[d][j] = gi;
					//switch coefficients of i and j in cycle reps
					for(map< int,map<int,int> >::iterator iter = cyclerep.begin(); iter != cyclerep.end(); iter++) {
						if(sdim[(*iter).first]==d) {
							int t = (*iter).second[i];
							(*iter).second[i] = (*iter).second[j];
							(*iter).second[j] = t;
						}
					}
					//switch row i with row k
					for(int l = i; l<N; l++) {
						int t = mat[i][l];
						mat[i][l] = mat[k][l];
						mat[k][l] = t;
					}
					brk = true;
					break;
				}
			}
			if(brk) break;
		}
		if(!found) break;
		bool done = false;
		while(!done) {
			done = true;
			//improve row-wise -- no change to generators
			for(int j = i+1; j<N+1; j++) {
				if(mat[j][i] % mat[i][i] == 0) {
					// if divisible, clear it
					int q = mat[j][i] / mat[i][i];
					for(int k = i; k<N; k++) {
						mat[j][k] -= q*mat[i][k];
					}
				} else {
					//if not divisible, make it divisible and clear it and mark flag to continue computation
					done = false;
					int e, x, y, alpha, beta;
					e = gcd(mat[i][i],mat[j][i],x,y);
					alpha = mat[i][i]/e;
					beta = mat[j][i]/e;
					for(int k = 0; k<N; k++) {
						int c = mat[i][k], d = mat[j][k];
						mat[i][k] = c*x + d*y;
						mat[j][k] = -c*beta + d*alpha;
					}
					int q = mat[j][i] / mat[i][i];
					for(int k = 0; k<N; k++) {
						mat[j][k] -= q*mat[i][k];
					}
				}
			}
			//improve column-wise -- changes generators and cycle representations
			for(int j = i+1; j<N; j++) {
				if(mat[i][j] % mat[i][i] == 0) {
					// if divisible, clear it
					int q = mat[i][j] / mat[i][i];
					for(int k = 0; k<N+1; k++) {
						mat[k][j] -= q*mat[k][i];
					}
					//<i> becomes <i> + q*<j>
					for(map<int,int>::iterator iter = generators[d][j].begin(); iter != generators[d][j].end(); iter++) {
						generators[d][i][(*iter).first] += q*(*iter).second;
					}
					//For each cycle, subtract q times the coefficient of <i> to the coefficient of <j>
					for(map< int, map<int,int> >::iterator iter = cyclerep.begin(); iter != cyclerep.end(); iter++) {
						if(sdim[(*iter).first]==d) {
							(*iter).second[j] -= q*(*iter).second[i];
						}
					}
				} else {
					//if not divisible make it devisible, clear it, and mark flag to continue computation
					done = false;
					int e, x, y, alpha, beta;
					e = gcd(mat[i][i],mat[i][j],x,y);
					alpha = mat[i][i]/e;
					beta = mat[i][j]/e;
					for(int k = 0; k<N+1; k++) {
						int c = mat[k][i], d = mat[k][j];
						mat[k][i] = c*x + d*y;
						mat[k][j] = -c*beta + d*alpha;
					}
					//<i> becomes alpha*<i> + beta*<j>, <j> becomes -y*<i> + x*<j>
					map<int,int> ti(generators[d][i]), tj(generators[d][j]);
					for(map<int,int>::iterator iter = ti.begin(); iter != ti.end(); iter++) {
						generators[d][i][(*iter).first] = alpha*ti[(*iter).first] + beta*tj[(*iter).first];
						generators[d][j][(*iter).first] = -y*ti[(*iter).first] + x*tj[(*iter).first];
					}
					for(map<int,int>::iterator iter = tj.begin(); iter != tj.end(); iter++) {
						generators[d][i][(*iter).first] = alpha*ti[(*iter).first] + beta*tj[(*iter).first];
						generators[d][j][(*iter).first] = -y*ti[(*iter).first] + x*tj[(*iter).first];
					}
					//For each cycle, c_i -> x*c_i + y*c_j, c_j -> -beta*c_i + alpha*c_j
					for(map< int, map<int,int> >::iterator iter = cyclerep.begin(); iter != cyclerep.end(); iter++) {
						if(sdim[(*iter).first]==d) {
							int ci = (*iter).second[i], cj = (*iter).second[j];
							(*iter).second[i] = x*ci + y*cj;
							(*iter).second[j] = -beta*ci + alpha*cj;
						}
					}
					int q = mat[i][j] / mat[i][i];
					for(int k = 0; k<N+1; k++) {
						mat[k][j] = mat[k][j] - q*mat[k][i];
					}
					//<i> becomes <i> - q*<j>
					for(map<int,int>::iterator iter = generators[d][j].begin(); iter != generators[d][j].end(); iter++) {
						generators[d][i][(*iter).first] -= q*(*iter).second;
					}
					//For each cycle, add q times the coefficient of <i> to the coefficient of <j>
					for(map< int, map<int,int> >::iterator iter = cyclerep.begin(); iter != cyclerep.end(); iter++) {
						if(sdim[(*iter).first]==d) {
							(*iter).second[j] += q*(*iter).second[i];
						}
					}
				}
			}
		}
	}
//	for(int i = 0; i<N+1; i++) {
//		for(int j = 0; j<N; j++) {
//			printw("%d ",mat[i][j]);
//		}
//		printw("\n");
//	}
//	printw("\n");
	//Transfer torsion coefficients back
	for(int i = 0; i<N; i++) {
		torsion[d][i] = mat[i][i];
	}
}

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Usage: %s filename\n",argv[0]);
		return 1;
	}
	
#ifdef CURSES
	initscr(); cbreak(); noecho();
	nonl(); intrflush(stdscr, FALSE);
	keypad(stdscr,TRUE);
#endif

	signal(SIGABRT, &cleanexit);
	signal(SIGTERM, &cleanexit);
	signal(SIGINT, &cleanexit);

	FILE *fin = fopen(argv[1],"r");
	int N[3];
	fscanf(fin,"%d",&dim);
	
	fscanf(fin,"%d",&N[0]);
	generators = new vector< map<int,int> >[dim+1];
	torsion = new vector<int>[dim+1];
	for(int i = 0; i < N[0]; i++) {
		double x, y, z;
		fscanf(fin,"%lf%lf%lf",&x,&y,&z);
		
		generators[0].push_back(map<int,int>());
		generators[0][i][i]=1;
		torsion[0].push_back(0);
		cycles[i][i]=1;
		cyclerep[i][generators[0].size()-1]=1;
		sdim[i] = 0;
		
		erase();
		show();
		
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
				if(j & 1) bound = ~bound; // negate bounding simplex if necessary
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
				sdim[id] = 1;
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
					printw("Boundary: 1[%d] -1[%d]\n",boundary[id][0],~boundary[id][1]);
					map<int,int> rel;
					for(int i = 0; i<=d; i++) {
						int sim = boundary[id][i];
						int coeff = 1;
						if(sim < 0) {
							sim = ~sim;
							coeff = -1;
						}
						for(map<int,int>::iterator iter = cyclerep[sim].begin(); iter!=cyclerep[sim].end(); iter++) {
							rel[(*iter).first] += coeff*(*iter).second;
						}
					}
					quotient(d-1,rel);
				} else {
					// Cycle
					printw("Cycle detected\n");
					map<int,int> cyc;
					int v = a;
					do {
						vector<int> sim;
						sim.push_back(v);
						sim.push_back(par[v]);
						int simid = get_id(sim);
//						printw("%d: %d %d\n",simid,v,par[v]);
						if(simid < 0) {
							cyc[~simid] = cyc[~simid]-1;
						} else {
							cyc[simid] = cyc[simid]+1;
						}
						v = par[v];
					} while(v != a);
					cycles[id] = cyc;
					map<int,int> gen;
					gen[id] = 1;
					generators[1].push_back(gen);
					torsion[1].push_back(0);
					cyclerep[id][generators[1].size()-1] = 1;
				}
			} else if(d == 2) { // 2-simplex
				sdim[id] = 2;
				// detect cycle
				bool iscycle = true;
				int vis[N[2]];
				for(int i = 0; i<N[2]; i++) {
					vis[i] = 0;
				}
				queue<int> q;
				map<int,int> cyc;
				q.push(id);
				// push necessary simplices for a cycle
				while(!q.empty() && iscycle) {
					// get simplex id and orientation
					int sim = q.front();
					q.pop();
					bool neg = sim < 0;
					if(neg) sim = ~sim;
					// if we arrive at a simplex in the opposite orientation, there is no cycle
					if((vis[sim-off] == 1 && neg) || (vis[sim-off]==-1 && !neg)) {
						iscycle = false;
						break;
					}
					if(vis[sim-off]) continue;
					vis[sim-off] = neg ? -1 : 1;
					if(neg) {
						if(~sim < 0) {
							cyc[sim] = cyc[sim] - 1;
						} else {
							cyc[~sim] = cyc[~sim] + 1;
						}
					} else {
						if(sim < 0) {
							cyc[~sim] = cyc[~sim] - 1;
						} else {
							cyc[sim] = cyc[sim] + 1;
						}
					}
					for(int i = 0; i<boundary[sim].size(); i++) {
						int bsim = boundary[sim][i];
						bool bneg = bsim < 0;
						if(bneg) bsim = ~bsim;
						if(coboundary[bsim].size() < 2) {
							iscycle = false;
							break;
						} else {
							int osim = coboundary[bsim][0];
							if(osim == sim || osim == ~sim) {
								osim = coboundary[bsim][1];
							}
							// correct for previous negations and negate once more to cancel out boundary
							if(bneg ^ neg ^ 1) osim = ~osim;
							q.push(osim);
						}
					}
				}
				if(iscycle) {
					printw("Cycle\n");
					cycles[id] = cyc;
					map<int,int> gen;
					gen[id]=1;
					generators[2].push_back(gen);
					torsion[2].push_back(0);
					cyclerep[id][generators[2].size()-1]=1;
				} else {
					printw("No cycle\n");
					map<int,int> bound; // map simplex -> coefficient for boundary
					map<int,int> rep; // representation in terms of fundamental cycles
					priority_queue<int> pq;
					for(int i = 0; i<boundary[id].size(); i++) {
						if(boundary[id][i] < 0) {
							bound[~boundary[id][i]] = bound[~boundary[id][i]]-1;
						} else {
							bound[boundary[id][i]] = bound[boundary[id][i]]+1;
						}
					}
					for(map<int,int>::iterator iter = bound.begin(); iter!=bound.end(); iter++) {
						pq.push((*iter).first);
					}
					printw("Boundary:\n");
					for(map<int,int>::iterator iter=bound.begin(); iter!=bound.end(); iter++) {
						if((*iter).second != 0) printw("%d[%d] ",(*iter).second,(*iter).first);
					}
					printw("\n");
					while(!pq.empty()) {
						int sim = pq.top();
						pq.pop();
						if(bound[sim]!=0) {
							int c = bound[sim];
							rep[sim] = c;
							for(map<int,int>::iterator iter = cycles[sim].begin(); iter != cycles[sim].end(); iter++) {
								if(bound[(*iter).first] == 0) {
									pq.push((*iter).first);
								}
								bound[(*iter).first] = bound[(*iter).first] - (*iter).second * c;
								if(bound[(*iter).first] == 0) {
									bound.erase((*iter).first);
								}
							}
						}
					}
					printw("Boundary Representation:\n");
					for(map<int,int>::iterator iter = rep.begin(); iter != rep.end(); iter++) {
						if((*iter).second != 0) printw("%d<%d> ",(*iter).second,(*iter).first);
					}
					printw("\n");
					map<int,int> rel;
					for(map<int,int>::iterator it = rep.begin(); it!=rep.end(); it++) {
						for(map<int,int>::iterator iter = cyclerep[(*it).first].begin(); iter!=cyclerep[(*it).first].end(); iter++) {
							rel[(*iter).first] += (*it).second*(*iter).second;
						}
					}
					quotient(d-1,rel);
				}
			}
			show();
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
