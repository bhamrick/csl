#include<simplex.hh>
#include<GL/glut.h>

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

using namespace std;

scomplex com;
bool dragging;
int dragx, dragy;
double alpha = 0, beta = 0, s=1;
double centerx=0.0, centery=0.0, centerz=0.0;

struct color {
	double r, g, b;
} *colors;

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
//	glBegin(GL_LINES);
//	glColor3d(0.0,0.0,0.0);
//	for(int i = 0; i<com.simplices.size(); i++) {
//		if(com.simplices[i]->dim == 1) {
//			for(int j = 0; j<2; j++) {
//				glVertex3d(com.points[com.simplices[i]->verts[j]]->x,com.points[com.simplices[i]->verts[j]]->y,com.points[com.simplices[i]->verts[j]]->z);
//			}
//		}
//	}
//	glEnd();
	glBegin(GL_TRIANGLES);
	for(int i = 0; i<com.simplices.size(); i++) {
		glColor3d(colors[i].r,colors[i].g,colors[i].b);
		if(com.simplices[i]->dim == 2) {
			for(int j = 0; j<3; j++) {
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
	glutCreateWindow("Simplicial Complex Visualization");
	if(argc < 2) {
		printf("Usage: %s filename\n",argv[0]);
		return 0;
	}
	FILE *fin = fopen(argv[1],"r");
	simplex *s;
	point *p;
	fscanf(fin,"%d",&com.dim);
	int n, tot=0;
	fscanf(fin,"%d",&n);
	centerx=centery=centerz=0.0;
	for(int i = 0; i<n; i++) {
		p = new point();
		fscanf(fin,"%lf%lf%lf",&(p->x),&(p->y),&(p->z));
		centerx+=p->x;
		centery+=p->y;
		centerz+=p->z;
		com.points.push_back(p);
	}
	centerx/=n;
	centery/=n;
	centerz/=n;
	tot+=n;
	for(int i = 0; i<n; i++) {
		com.points[i]->x-=centerx;
		com.points[i]->y-=centery;
		com.points[i]->z-=centerz;
	}
	for(int d = 1; d<=com.dim; d++) {
		fscanf(fin,"%d",&n);
		tot+=n;
		for(int i = 0; i<n; i++) {
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
	glutTimerFunc(1000/60,displayFunc,0);
	glutMouseFunc(initDragFunc);
	glutMotionFunc(dragFunc);
	glutMainLoop();
}
