#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>

typedef struct {
	double re;
	double im;
} complex;

double xmin = 0;
double xmax;
double ymin = 0;
double ymax;
double umin;
double umax;
double vmin;
double vmax;
double epsilon;
int limit;
complex c;

void ColorScheme(int n) {
	if (n == -1) {
		glColor3f(0.5f, 0.3f, 0.2f);
	} else if (limit < 16) {
		int r = ((n - 1)/(double)(limit - 1) * 255 + 0.5);
		int g = 255 -r;
		int b = ((n-1) % (limit/2)) * 255 / (limit/2);
		glColor3f((float)r/255.f, (float)(g/255.f), (float)(b/255.f));
	} else {
		int lim = limit < 32 ? limit : 32;
		int r = (n-1)/255/lim;
		int g = ((n-1)%(lim/4)) * 255 / (lim/4);
		int b = ((n-1)%(lim/8)) * 255 / (lim/8);
		glColor3f((float)(r/255.f), (float)(g/255.f), (float)(b/255.f));
	}
}

int DivergenceTest(complex z0, complex c, double epsilon, int limit) {
	complex z;
	z.re = z0.re; z.im = z0.im;
	double modul2 = z.re * z.re + z.im * z.im;
	// if (modul2 > epsilon * epsilon) {
	// 	return 0;
	// }
	for (int i = 1; i <= limit; ++i) {
		double next_re = z.re*z.re - z.im * z.im + c.re;
		double next_im = 2 * z.re * z.im + c.im;
		z.re = next_re;
		z.im = next_im;
		double modul2 = z.re * z.re + z.im * z.im;
		if (modul2 > epsilon * epsilon) return i;
	}
	return -1;
}

void RenderScene() {
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int y = ymin; y <= ymax; ++y) {
		for(int x = xmin; x <= xmax; ++x) {
			complex z0;
			z0.re = (x - xmin)/(double)(xmax - xmin) * (umax - umin) + umin;
			z0.im = (y - ymin)/(double)(ymax - ymin) * (vmax - vmin) + vmin;
			int n = DivergenceTest(z0, c, epsilon, limit);
			// if (n==-1) {
			// 	glColor3f(0.0f, 0.0f, 0.0f);
			// } else {
			// 	glColor3f((double)n/limit, 1.0-(double)n/limit, (double)n/limit);
			// 	// glColor3f(1.0f, 1.0f, 1.0f);
			// }
			ColorScheme(n);
			// printf("Crtam: %d %d\n", x, y);
			glVertex2i(x, y);
		}
	}
	glEnd();
}

void reshape(int width, int height) {
	xmax = width;
	ymax = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, (GLsizei)xmax, (GLsizei)ymax);
}

void display() {
	glClearColor(0.0, 1.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	RenderScene();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	printf("Unesite epsilon i maksimalan broj iteracija.\n");
	scanf("%lf %d", &epsilon, &limit);

	printf("Unesite podrucje kompleksne ravnine, umin, umaks, vmin, vmaks.\n");
	scanf("%lf %lf %lf %lf", &umin, &umax, &vmin, &vmax);

	printf("Unesite razlucivost zaslona, xmaks, ymaks.\n");
	scanf("%lf %lf", &xmax, &ymax);

	printf("Unesite realni i imaginarni dio konstante cEC.\n");
	scanf("%lf %lf", &c.re, &c.im);

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(xmax, ymax);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	int window = glutCreateWindow("Fraktali :)");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}