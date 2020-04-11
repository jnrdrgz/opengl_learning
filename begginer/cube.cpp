#include <GL/freeglut.h>
#include <stdio.h>

#define GLUT_KEY_ESCAPE 27

const GLsizei windowW = 500;
const GLsizei windowH = 500;

GLfloat cubeRotateX = 45.0f;
GLfloat cubeRotateY = 45.0f;

bool keys[255];


//INIT OPEN GL
GLvoid establishProjectionMatrix(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 200.0f );
}

GLvoid initGL(GLsizei width, GLsizei height) {
	establishProjectionMatrix(width, height);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_PERSPECTIVE_CORRECTION_HINT); 

}

GLvoid displayFPS(GLvoid) {
	static long lastTime = glutGet(GLUT_ELAPSED_TIME);
	static long loops = 0;
	static GLfloat fps = 0.0f;

	int newTime = glutGet(GLUT_ELAPSED_TIME);

	if (newTime - lastTime > 100) { 
		float newFPS = (float) loops / float(newTime-lastTime) * 1000.0f;

		fps = (fps + newFPS) / 2.0f;

		char title[80];
		sprintf_s(title, "OpenGL - DEMO %f", fps);

		glutSetWindowTitle(title);

		lastTime = newTime;
		loops = 0;
 	}

	loops++;
}

GLvoid draw_cube() {
	//draw cube

	glBegin(GL_QUADS);
	//FRONT FACE
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	//BACK FACE
	glColor3f(1.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	//TOP FACE
	glColor3f(1.0f, 0.5f, 0.0f);

	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	//BOTTOM FACE
	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);



	//LEFT FACE
	glColor3f(1.0f, 0.0f, 1.0f);

	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//RIGHT FACE
	glColor3f(1.0f, 0.0f, 1.0f);

	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();
}

GLvoid drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -5.0f);
	glRotatef(cubeRotateX, 1, 0, 0);
	glRotatef(cubeRotateY, 0, 1, 0);
	
	draw_cube();
	
	glFlush();

	glutSwapBuffers();

	displayFPS();
}

//keys
GLboolean checkKeys(GLvoid) {

	const GLfloat speed = 1.0f;

	if (keys[GLUT_KEY_ESCAPE])
		return true;

	if (keys[GLUT_KEY_LEFT])
		cubeRotateY -= speed;

	if (keys[GLUT_KEY_RIGHT])
		cubeRotateY += speed;

	if (keys[GLUT_KEY_UP])
		cubeRotateX -= speed;

	if (keys[GLUT_KEY_DOWN])
		cubeRotateX += speed;

	return false;
}

//keyboard
GLvoid keyboardCB(unsigned char key, int x, int y) {
	keys[key] = true;
}

GLvoid keyboardUpCB(unsigned char key, int x, int y) {
	keys[key] = false;
}

GLvoid specialCB(int key, int x, int y) {
	keys[key] = true;
}

GLvoid specialUpCB(int key, int x, int y) {
	keys[key] = false;
}

GLvoid timerLoop(int value) {
	if (checkKeys()) {
		exit(0);
	}

	glutPostRedisplay();

	glutTimerFunc(1, timerLoop, 0);
}


int main(int argc, char* args[])
{

	glutInit(&argc, args);

	//Create OpenGL 2.1 context
	//glutInitContextVersion(2, 1);

	glutInitDisplayMode(GLUT_DOUBLE);

	int windowID = glutCreateWindow("OPENGL Cube Demo");
	glutReshapeWindow(windowW, windowH);

	//init
	initGL(windowW, windowH);

	//render
	glutDisplayFunc(drawScene);

	//key handling
	glutKeyboardFunc(keyboardCB);
	glutKeyboardUpFunc(keyboardUpCB);
	glutSpecialFunc(specialCB);
	glutSpecialUpFunc(specialUpCB);

	//loop
	glutTimerFunc(1, timerLoop, 0);

	glutMainLoop();

	return 0;
}