#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>

#include "object_model.h"
#include "transform.h"
#include "matrix.h"

typedef struct {
	float x;
	float y;
	float z;
} Point3D;

std::vector<Point3D> points;
std::vector<Point3D> bezierPoints;
ObjectModel* original;
MyVector eye;
MyVector center;

ObjectModel* TransformCopy(ObjectModel* o, float* Matrix) {
	ObjectModel* n = new ObjectModel(o);
	float* homogenous = (float* )malloc(sizeof(float) * 4);
	homogenous[3] = 1;
	float* result;
	// printf("here\n");
	// MatrixPrint(Matrix, 4, 4);

	for (int i = 0; i < n->vertices_.size(); ++i) {
		float* x1 = &n->vertices_[i]->x;
		float* y1 = &n->vertices_[i]->y;
		float* z1 = &n->vertices_[i]->z;

		// printf("radim s koordinatama:\n");
		// printf("%f %f %f\n", *x1, *y1, *z1);

		homogenous[0] = *x1;
		homogenous[1] = *y1;
		homogenous[2] = *z1;
		MatrixMultiply(homogenous, 4, 1, Matrix, 4, 4, &result);
		float denominator = result[3];
		// printf ("denominator: %f\n", denominator);
		*x1 = result[0] / denominator;
		*y1 = result[1] / denominator;
		*z1 = result[2] / denominator;
		free(result);

		// printf("Preslikalo se u:\n");
		// printf("%f %f %f\n", *x1, *y1, *z1);
	}
}

void compute_factors(std::vector<int>& factors) {
	int a = 1;
	int n = factors.size() - 1;
	int i;
	for (i = 1; i < factors.size(); ++i) {
		factors[i-1] = a;
		a = a * (n-i+1) / i;
	}
	factors[i-1] = a;
}

void draw_polygon(std::vector<Point3D>& points) {
	glBegin(GL_LINE_STRIP); 
	{
		for (int i = 0; i < points.size(); ++i) {
			glVertex3f(points[i].x, points[i].y, points[i].z);
		}
	}
	glEnd();
}

void draw_bezier(std::vector<Point3D>& points) {
	Point3D p;
	int n = points.size() - 1;
	std::vector<int> factors(points.size());
	double t, b;

	compute_factors(factors);
	//factors
	// for(int i = 0; i < factors.size(); ++i) {
	// 	printf("%d\n", factors[i]);
	// }
	t = 0.00;
	//int divs = 1 / 0.01;
	glBegin(GL_LINE_STRIP);
	{
		for(int i = 0; t < 1.0; ++i) {
			p.x = p.y = p.z = 0;
			for (int j = 0; j < points.size(); ++j) {
				if (j == 0) {
					b = factors[j] * pow(1 - t, n);
				} else if (j == n) {
					b = factors[j] * pow(t, n);
				} else {
					b = factors[j] * pow(t, j) * pow(1-t, n-j);
				}
				p.x += b * points[j].x;
				p.y += b * points[j].y;
				p.z += b * points[j].z;
			}
			printf("T: %lf\n", t);
			printf("Crtam tocku: %f %f %f\n", p.x, p.y, p.z);
			glVertex3f(p.x, p.y, p.z);
			//bezierPoints.push_back(p);
			t += 0.01;

			if (t > 1.0)
				break;
		}
	}
	glEnd();
}

void ReadFile(std::vector<Point3D>& points, FILE* f) {
	char buff[500];
	Point3D p;
	while(fscanf(f, "%[^\n]\n", buff) == 1) {
		sscanf(buff, "%f %f %f", &p.x, &p.y, &p.z);
		//printf("Ucitano je: %f %f %f\n", p.x, p.y, p.z);
		points.push_back(p);
	}
}

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glFrustum(-10, 10, -10, 10, 1, 100);
	glFrustum(-1, 1, -1, 1, 1.0f, 100.0f);
	// gluOrtho2D(-20, 20, 20, 20);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

}

void display() {
	glClearColor(0.0, 1.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0, 0.0, 0.0);
	draw_polygon(points);
	glColor3f(0.5, 0.5, 0.5);
	draw_bezier(points);
	glutSwapBuffers();

}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf ("Hocu samo datoteku za krivulju");
		exit(1);
	}

	FILE* f = fopen(argv[1], "r");
	if (!f) {
		printf("Neuspjesno otvaranje datoteke.\n");
		exit(1);
	}

	ReadFile(points, f);
	fclose(f);

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	int window = glutCreateWindow("Primjer");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}