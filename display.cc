#include<simplex.hh>
#include<GL/glut.h>

using namespace std;

scomplex com;
double alpha = 0, beta = 0;

void displayFunc(int value) {
	alpha+=0.4;
	beta +=0.2;
	glLoadIdentity();
	glBegin(GL_QUADS);
	glColor3d(1.0,1.0,1.0);
	glVertex2d(-1.0, 1.0);
	glVertex2d( 1.0, 1.0);
	glVertex2d( 1.0,-1.0);
	glVertex2d(-1.0,-1.0);
	glEnd();
	glRotatef(alpha,0.0f,1.0f,0.0f);
	glRotatef(beta,1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glColor3d(0.0,0.0,0.0);
	for(int i = 0; i<com.simplices.size(); i++) {
		for(int j = 0; j<com.simplices[i]->verts.size(); j++) {
			for(int k = j+1; k<com.simplices[i]->verts.size(); k++) {
				glVertex3d(com.points[com.simplices[i]->verts[j]]->x,com.points[com.simplices[i]->verts[j]]->y,com.points[com.simplices[i]->verts[j]]->z);
				glVertex3d(com.points[com.simplices[i]->verts[k]]->x,com.points[com.simplices[i]->verts[k]]->y,com.points[com.simplices[i]->verts[k]]->z);
			}
		}
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
	glutTimerFunc(1000/60,displayFunc,0);
}

int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
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
	int n;
	fscanf(fin,"%d",&n);
	for(int i = 0; i<n; i++) {
		p = new point();
		fscanf(fin,"%lf%lf%lf",&(p->x),&(p->y),&(p->z));
		com.points.push_back(p);
	}
	for(int d = 1; d<=com.dim; d++) {
		fscanf(fin,"%d",&n);
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
	glutTimerFunc(1000/60,displayFunc,0);
	glutMainLoop();
}
