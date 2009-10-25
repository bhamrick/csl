#include<simplex.hh>
#include<GL/glut.h>
#include<map>

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

using namespace std;

scomplex com;
vector< vector<int> > surfaces;
vector< vector<int> > incidences; // edges are incident to faces and vice-versa
map<pair<int,int>,int> edgeid;
vector<int> surfaceid;
vector<bool> vis;
bool dragging;
int dragx, dragy;
double alpha = 0, beta = 0, s=1;
double centerx=0.0, centery=0.0, centerz=0.0;

struct color {
	double r, g, b;
} *colors;

void ffill(int v, int c) {
	if(vis[v]) return;
	vis[v] = true;
	surfaceid[v] = c;
	surfaces[c].push_back(v);
	for(int i = 0; i<incidences[v].size(); i++) {
		ffill(incidences[v][i],c);
	}
}

void displayFunc(int value) {
	glLoadIdentity();
	glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glBegin(GL_QUADS);
	glColor3d(1.0,1.0,1.0);
	glVertex3d(-1.0, 1.0,-1.0);
	glVertex3d( 1.0, 1.0,-1.0);
	glVertex3d( 1.0,-1.0,-1.0);
	glVertex3d(-1.0,-1.0,-1.0);
	glEnd();
	glFlush();
	glDepthFunc(GL_LESS);
	glScalef(s,s,s);
	glRotatef(alpha,0.0f,1.0f,0.0f);
	glRotatef(beta,1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	for(int i = 0; i<com.simplices.size(); i++) {
		if(com.simplices[i]->dim == 1) {
			glColor3d(colors[surfaceid[i]].r,colors[surfaceid[i]].g,colors[surfaceid[i]].b);
			for(int j = 0; j<2; j++) {
				glVertex3d(com.points[com.simplices[i]->verts[j]]->x,com.points[com.simplices[i]->verts[j]]->y,com.points[com.simplices[i]->verts[j]]->z);
			}
		}
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
	glutTimerFunc(1000/60,displayFunc,0);
}

void initDragFunc(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		dragging = true;
		dragx = x;
		dragy = y;
	} else {
		if(button == GLUT_WHEEL_UP) {
			s += 0.1;
		} else if(button == GLUT_WHEEL_DOWN) {
			s -= 0.1;
		}
		dragging = false;
	}
}

void dragFunc(int x, int y) {
	if(dragging) {
		alpha += 1*(x-dragx);
		beta += 1*(y-dragy);
		dragx = x;
		dragy = y;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(200,200);
	glutCreateWindow("Homology Computation of Surfaces");
	if(argc < 2) {
		printf("Usage: %s filename\n",argv[0]);
		return 1;
	}
	FILE *fin = fopen(argv[1],"r");
	simplex *s;
	point *p;
	fscanf(fin,"%d",&com.dim);
	if(com.dim != 2) {
		printf("Error: %s is only for surfaces embedded in R3\n");
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
		}
	}
	colors = new color[tot];
	for(int i = 0; i<tot; i++) {
		colors[i].r = (double)rand()/RAND_MAX;
		colors[i].g = (double)rand()/RAND_MAX;
		colors[i].b = (double)rand()/RAND_MAX;
	}
//Insert edge ids
	for(int i = 0; i<n[1]; i++) {
		edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[1])] = i;
	}
//Begin computation of incidences
	for(int i = 0; i<n[1]+n[2]; i++) {
		incidences.push_back(vector<int>());
	}
	for(int i = n[1]; i<n[1]+n[2]; i++) {
		incidences[i].push_back(edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[1])]);
		incidences[i].push_back(edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[2])]);
		incidences[i].push_back(edgeid[make_pair(com.simplices[i]->verts[1],com.simplices[i]->verts[2])]);
		incidences[edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[1])]].push_back(i);
		incidences[edgeid[make_pair(com.simplices[i]->verts[0],com.simplices[i]->verts[2])]].push_back(i);
		incidences[edgeid[make_pair(com.simplices[i]->verts[1],com.simplices[i]->verts[2])]].push_back(i);
	}
//End computation of incidences
//Begin computation of surfaces
	surfaces.push_back(vector<int>());
	int count = 0;
	for(int i = 0; i<n[1]+n[2]; i++) {
		vis.push_back(false);
		surfaceid.push_back(0);
	}
	for(int i = 0; i<n[1]+n[2]; i++) {
		if(!vis[i]) {
			surfaces.push_back(vector<int>());
			ffill(i, count);
			sort(surfaces[count].begin(),surfaces[count].end());
			count++;
		}
	}
//End computation of surfaces
	printf("%d surfaces\n",count);
	int b0 = 1, b1=0, b2 = count-1;
	for(int i = 0; i<vis.size(); i++) vis[i]=false;
	while(vis.size() < n[0]) vis.push_back(false);
//Begin computation of genus of surfaces and first betti number
	for(int i = 0; i<count; i++) {
		int chi = 0;
		for(vector<int>::iterator iter = surfaces[i].begin(); iter!=surfaces[i].end(); iter++) {
			if((com.simplices[*iter]->dim)&1) {
				chi--;
			} else {
				chi++;
			}
			if(com.simplices[*iter]->dim==1) {
				if(!vis[com.simplices[*iter]->verts[0]]) {
					vis[com.simplices[*iter]->verts[0]]=1;
					chi++;
				}
				if(!vis[com.simplices[*iter]->verts[1]]) {
					vis[com.simplices[*iter]->verts[1]]=1;
					chi++;
				}
			}
		}
		printf("Surface %d Euler characteristic: %d\n",i,chi);
		//genus = 1-chi/2
		b1+=1-chi/2;
	}
//End computation of genus of surfaces and first betti number
	printf("Betti numbers: %d %d\n",b1,b2);
	glutTimerFunc(1000/60,displayFunc,0);
	glutMouseFunc(initDragFunc);
	glutMotionFunc(dragFunc);
	glutMainLoop();
}
