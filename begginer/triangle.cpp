#include <GL/freeglut.h>
#include <stdio.h>

void displayTr() {
	//Clear the frame buffer
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw triangle
	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(0.0, 1.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2f(1.0, -1.0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(-1.0, -1.0);
	glEnd();

	// Flush
	glFlush();
	glutSwapBuffers();
}



int main(int argc, char* args[])
{
	//One time glut initialization
	glutInit(&argc, args);

	// GLUT_SINGLE cause flickering
	glutInitDisplayMode(GLUT_DOUBLE);

	int windowId = glutCreateWindow("OpenGL 3D Buzz");

	glutDisplayFunc(displayTr);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glutMainLoop();

	return 0;
}