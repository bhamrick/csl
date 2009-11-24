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
	q.push(make_pair(0,0));
	vis[0][0]=true;
	while(!q.empty()) {
		pair<int,int> p = q.front();
		q.pop();
		printf("%d %d\n",p.first,p.second);
		if(com.simplices[p.first]->dim == 2) {
			for(int i = 0; i<incidences[p.first].size(); i++) {
				if(!vis[incidences[p.first][i].second ^ p.second ^ 1][incidences[p.first][i].first]) {
					q.push(make_pair(incidences[p.first][i].first,incidences[p.first][i].second ^ p.second ^ 1));
					vis[incidences[p.first][i].second ^ p.second ^ 1][incidences[p.first][i].first]	= true;
				}
			}
		} else {
			for(int i = 0; i<incidences[p.first].size(); i++) {
				if(!vis[incidences[p.first][i].second ^ p.second][incidences[p.first][i].first]) {
					q.push(make_pair(incidences[p.first][i].first,incidences[p.first][i].second ^ p.second));
					vis[incidences[p.first][i].second][incidences[p.first][i].first]=1;
				}
			}
		}
	}
	printf("%s\n",vis[1][0] ? "Non-orientable" : "Orientable");
//End computation of orientability
	return 0;
}
