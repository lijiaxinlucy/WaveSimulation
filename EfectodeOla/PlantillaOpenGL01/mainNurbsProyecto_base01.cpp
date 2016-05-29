// Cubica

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0

GLfloat knots01[8] = {
	0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0
};

GLfloat knots02[9] = {
	0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0
};

GLfloat ctrlpointsCurveNurbs01[4][3] = {
	{-10.0, 0.0, 0.0}, {-5.0, 0.0, 8.0},
	{5.0, 0.0, 8.0}, {10.0, 0.0, 0.0}
};

GLfloat ctrlpointsCurveNurbs02[5][3] = {
	{-2.0, 2.0, 0.0}, {-1.0, 2.0, 2.0}, 
	{1.0, 2.0, -2.0}, {2.0, 2.0, 0.0}, {2.0, 2.0, 1.0}
};

GLfloat ctrPoints01[4][3] = {
	{-4.0,0.0,0.0},{-2.0,5.0,0.0},{2.0,5.0,0.0},{4.0,0.0,0.0}
};

GLfloat ctlpointsNurbsSurf[4][4][3];
GLfloat knotsSurfs[25] = {0};



GLUnurbsObj *theNurb01, *theNurb02, *theNurbSurf;
float t;

GLfloat ctlpoints[21][21][3] = {0};
GLfloat L = 0.78;
GLfloat A = 0.4;
GLfloat S = 2;
GLfloat Dx = 0;
GLfloat Dz = -1;
GLfloat W = 8;
GLfloat G = 1;

GLUnurbsObj *theNurb;

#define PI 3.1415926535897932384626433832795


void puntosNurb(){
	glColor3f(1.0,0.0,0.0);
	float xpoint = -10;
	float ypoint = 0;
	float zpoint = -10;

	glBegin(GL_LINES);
	for(int i = 0; i <=20; i++){
		for(int j = 0; j <=20; j++){
			ctlpoints[i][j][0]=xpoint++;
			ctlpoints[i][j][1]=0;
			ctlpoints[i][j][2]=zpoint;
		}
		xpoint = -10;
		ypoint = 0;
		zpoint++;
	}
		
	glEnd();

	glColor3f(0.0,1.0,0.0);

}

void init_surface(){

}

void changePoints(int value){

	/*t += 0.1;
		for (int i = 0; i <21; i++) {
			for (int j = 0; j < 21; j++) {
				ctlpoints[i][j][1] = 3*sin(t);
			}
		}
	/*ctlpoints[1][1][1] = 3*sin(t);
	ctlpoints[1][2][1] = -3*sin(t);
	ctlpoints[2][1][1] = -3*sin(t);
	ctlpoints[2][2][1] = 3*sin(t);*/

	glutTimerFunc(10,changePoints,1);
	glutPostRedisplay();
}

GLfloat newY(GLfloat X, GLfloat Z, GLfloat Wi, GLfloat Ti, GLfloat Gi, int olaID){
	return A*sin((Dx*X+Dz*Z)*Wi+Ti*Gi);
}

void updateY(int value){
	t += 0.1;
	W = 2*PI / L;
	G = S*2*PI / L;
		for (int i = 0; i <21; i++) {
			for (int j = 0; j < 21; j++) {
					ctlpoints[i][j][1] = newY(ctlpoints[i][j][0],ctlpoints[i][j][2],W,t,G,0);
					printf("NewY = %f\n",ctlpoints[i][j][1]);
			}
		}

	glutTimerFunc(10,updateY,1);
	glutPostRedisplay();
}

void init(){
	knotsSurfs[4]=0.058;
	for (int i = 0; i<4; i++){
		knotsSurfs[i]=0;
		knotsSurfs[24-i]=1;
	}
	for (int i = 5; i<21; i++){
		knotsSurfs[i]=knotsSurfs[i-1]+0.058;
	}
	for (int i = 0; i<25; i++){
		printf("knotsSurfs[%d] %f\n",i,knotsSurfs[i]);
	}
	theNurb01 = gluNewNurbsRenderer(); //
	theNurb02 = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb01, GLU_SAMPLING_TOLERANCE, 15.0);
	gluNurbsProperty(theNurb02, GLU_SAMPLING_TOLERANCE, 15.0);

	theNurbSurf = gluNewNurbsRenderer();
	gluNurbsProperty(theNurbSurf, GLU_SAMPLING_TOLERANCE, 15.0);
	gluNurbsProperty(theNurbSurf, GLU_DISPLAY_MODE, GLU_FILL);
	init_surface();
	/*
	ctlpointsNurbsSurf[1][1][1] = 3.0;
	ctlpointsNurbsSurf[1][2][1] = 3.0;
	ctlpointsNurbsSurf[2][1][1] = 3.0;
	ctlpointsNurbsSurf[2][2][1] = 3.0;
	*/
	puntosNurb();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glutTimerFunc(10,changePoints,1);
	glutTimerFunc(10,updateY,1);
	t = 0.0;
}


void ejesCoordenada() {
	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	
	float aspectratio;
	glViewport(0,0,w,h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   aspectratio = (float) w / (float) h;
   /*
   if (w<=h){
	   glOrtho(-10,10,-10/aspectratio, 10/aspectratio,1.0,-1.0);
   }
   else{
	   glOrtho(-10*aspectratio,10*aspectratio,-10,10,1.0,-1.0);
   }
   */

   gluPerspective(30, aspectratio, 1.0, 200.0);
  
}


void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(20.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	const GLfloat pos[] = {10.0, 10.0, 0.0, 0.0};
	glLightfv(GL_LIGHT0,GL_POSITION, pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glDisable(GL_LIGHTING);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;
	// Luz y material

	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 60.0 };
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	

    GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   


	// Render Grid 
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glPushMatrix();
	glRotatef(90,1.0,0.0,0.0);
    glColor3f( 0.0, 0.7, 0.7 );
   /* glBegin( GL_LINES );
    zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
    for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
	{
	xLocal = DEF_floorGridScale * loopX;
	glVertex3f( xLocal, -zExtent, 0.0 );
	glVertex3f( xLocal, zExtent,  0.0 );
	}
    xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
	{
	zLocal = DEF_floorGridScale * loopZ;
	glVertex3f( -xExtent, zLocal, 0.0 );
	glVertex3f(  xExtent, zLocal, 0.0 );
	}
    glEnd();*/
	//ejesCoordenada();
    glPopMatrix();
	glEnable(GL_LIGHTING);
	// Fin Grid
	
	glColor3f(0.0,1.0,0.0);
	glPointSize(10.0);
	/*glBegin(GL_POINTS);
		glVertex2f(0.0,0.0);
	glEnd();*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);


	//Bezier
	/*
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrPoints01[0][0]);
	glEnable(GL_MAP1_VERTEX_3);

	int i;
	float res = 30.0f; // Resolucion de la curva
	glColor3f(1.0,1.0,0.0);

	glBegin(GL_LINE_STRIP);
		for (i=0; i<=res; i++){
			glEvalCoord1f( (GLfloat) i / res);
		} 
	glEnd();

	//Grafica Ptos de Control
	glPointSize(10.0);
	glColor3f(0.0,1.0,1.0);
	glBegin(GL_POINTS);
		for (i = 0; i < 4; i++){
			glVertex3fv(&ctrPoints01[i][0]);
		}
	glEnd();
	*/
	/*
	//Curva Nurbs
	glColor3f(0.0,1.0,0.0);
	gluBeginCurve(theNurb01);
		gluNurbsCurve(theNurb01, 8, knots01, 3, &ctrlpointsCurveNurbs01[0][0], 4, GL_MAP1_VERTEX_3);
	gluEndCurve(theNurb01);

	//Grafica Ptos de Control
	glPointSize(15.0);
	glColor3f(0.0,1.0,1.0);
	glBegin(GL_POINTS);
		for (i = 0; i < 4; i++){
			glVertex3fv(&ctrlpointsCurveNurbs01[i][0]);
		}
	glEnd();

	glColor3f(0.0,1.0,0.0);
	gluBeginCurve(theNurb02);
		gluNurbsCurve(theNurb02, 8, knots02, 3, &ctrlpointsCurveNurbs02[0][0], 4, GL_MAP1_VERTEX_3);
	gluEndCurve(theNurb02);

	//Grafica Ptos de Control
	glPointSize(10.0);
	glColor3f(0.0,1.0,1.0);
	glBegin(GL_POINTS);
		for (i = 0; i < 4; i++){
			glVertex3fv(&ctrlpointsCurveNurbs02[i][0]);
		}
	glEnd();
	*/


	gluBeginSurface(theNurbSurf);

	gluNurbsSurface(theNurbSurf, 25, knotsSurfs, 25, knotsSurfs, 21*3, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurbSurf);

	//glutSolidSphere(4, 30, 30);

	glPopMatrix();
	
		/*int i,j;
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		for (i = 0; i <21; i++) {
			for (j = 0; j < 21; j++) {
	            glVertex3f(ctlpoints[i][j][0], 	ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);*/
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();
}


int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Nurbs Proyecto - Ola");

	init();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
		
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	

	glutMainLoop();
	return 0;

}
