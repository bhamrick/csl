#include<simplex.hh>
#include<queue>
#include<map>

using namespace std;

scomplex com;
vector<bool> vis[2];
vector< vector< pair<int,int> > > incidences;
map<pair<int,int>,int> edgeid;

int orientation(int a, int b) {
	return (int)(a>b);
}

int orientation(int a, int b, int c) {
	return (int)((a>b) ^ (a>c) ^ (b>c));
}

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Usage: %s filename\n",argv[0]);
		return 1;
	}
	FILE *fin = fopen(argv[1],"r");
	double centerx, centery, centerz;
	simplex *s;
	point *p;
	fscanf(fin,"%d",&com.dim);
	if(com.dim != 2) {
		printf("Error: %s is only for surfaces\n",argv[0]);
		return 1;
	}
	int n[3], tot=0;
	fscanf(fin,"%d",&n[0]);
	centerx=centery=centerz=0.0;
	for(int i = 0; i<n[0]; i++) {
		p = new point();
		fscanf(fin,"%lf%lf%lf",&(p->x),&(p->y),&(p->z));
		centerx+=p->x;
		centery+=p->y;
		centerz+=p->z;
		com.points.push_back(p);
	}
	centerx/=n[0];
	centery/=n[0];
	centerz/=n[0];
	tot+=n[0];
	for(int i = 0; i<n[0]; i++) {
		com.points[i]->x-=centerx;
		com.points[i]->y-=centery;
		com.points[i]->z-=centerz;
	}
	for(int d = 1; d<=com.dim; d++) {
		fscanf(fin,"%d",&n[d]);
		tot+=n[d];
		for(int i = 0; i<n[d]; i++) {
			s = new simplex();
			s->dim = d;
			for(int j = 0; j<=d; j++) {
				int ind;
				fscanf(fin,"%d",&ind);
				s->verts.push_back(ind);
			}
			sort(s->verts.begin(),s->verts.end());
			com.simplices.push_back(s);
			vis[0].push_back(false);
			vis[1].push_back(false);
		}
	}
//Insert edge ids
	for(int i = 0; i<n[1]; i++) {
		edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[1])] = i;
	}
//Begin computation of incidences
	for(int i = 0; i<n[1]+n[2]; i++) {
		incidences.push_back(vector< pair<int,int> >());
	}
	for(int i = n[1]; i<n[1]+n[2]; i++) {
		incidences[i].push_back(make_pair(edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[1])],0));
		incidences[i].push_back(make_pair(edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[2])],1));
		incidences[i].push_back(make_pair(edgeid[make_pair(com.simplices[i]->verts[1],com.simplices[i]->verts[2])],0));
		incidences[edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[1])]].push_back(make_pair(i,0));
		incidences[edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[2])]].push_back(make_pair(i,1));
		incidences[edgeid[make_pair(com.simplices[i]->verts[1],com.simplices[i]->verts[2])]].push_back(make_pair(i,0));
	}
//End computation of incidences
//Begin computation of orientability
	queue< pair<int,int> > q;
	q.push(make_pair(n[1],0));
	vis[0][n[1]]=true;
	while(!q.empty()) {
		pair<int,int> p = q.front();
		q.pop();
		printf("%d %d\n",p.first,p.second);
		if(com.simplices[p.first]->dim == 2) {
			for(int i = 0; i<incidences[p.first].size(); i++) {
				int eid;
				bool orient;
				if(!vis[incidences[p.first][i].second ^ p.second ^ 1][incidences[p.first][i].first]) {
					eid = incidences[p.first][i].first;
					orient = incidences[p.first][i].second ^ p.second ^ 1;
				}
//				printf("%d %d\n",eid,orient);
				for(int j = 0; j<incidences[eid].size(); j++) {
					if(incidences[eid][j].first != p.first) {
						if(!vis[incidences[eid][j].second ^ orient][incidences[eid][j].first]) {
							vis[incidences[eid][j].second ^ orient][incidences[eid][j].first] = true;
							q.push(make_pair(incidences[eid][j].first,incidences[eid][j].second ^ orient));
						}
					}
				}
			}
		}
	}
	printf("%s\n",vis[1][n[1]] ? "Non-orientable" : "Orientable");
	bool orientable = !vis[1][n[1]];
//End computation of orientability
	int chi = n[0] - n[1] + n[2]; // Euler Characteristic
	printf("Euler Characteristic: %d\n",chi);
	if(orientable) {
		printf("Surface is a sphere with %d handles\n",(2-chi)/2);
	} else {
		printf("Surface is a sphere with %d crosscaps\n",2-chi);
	}
	return 0;
}
