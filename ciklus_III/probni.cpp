#include <stdio.h>
#include <GL/glut.h>

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(width/4, height/4, 3*width/4, 3*height/4);
}

void display() {
	glClearColor(1.0, 1.0, 1.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP); 
	{
		glVertex3f(-0.9f, -0.9f, -0.9f);
		glVertex3f(0.9f, -0.9f, -0.9f);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	{
		glVertex3f(-0.9f, -0.7f, -0.9f);
		glVertex3f(0.9f, -0.7f, 3.1f);
	}
	glEnd();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	int window = glutCreateWindow("Primjer");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}