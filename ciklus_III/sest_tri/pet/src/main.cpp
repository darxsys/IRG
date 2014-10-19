#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <iostream>

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

	for (int i = 0; i < n->vertices_.size(); ++i) {
		float* x1 = &(n->vertices_[i].x);
		float* y1 = &(n->vertices_[i].y);
		float* z1 = &(n->vertices_[i].z);

		homogenous[0] = *x1;
		homogenous[1] = *y1;
		homogenous[2] = *z1;
		MatrixMultiply(homogenous, 4, 1, Matrix, 4, 4, &result);
		float denominator = result[3];

		*x1 = result[0] / denominator;
		*y1 = result[1] / denominator;
		*z1 = result[2] / denominator;
		free(result);
	}

	return n;
}

void computeFactors(std::vector<int>& factors) {
	int a = 1;
	int n = factors.size() - 1;
	int i;
	for (i = 1; i < factors.size(); ++i) {
		factors[i-1] = a;
		a = a * (n-i+1) / i;
	}
	factors[i-1] = 1;
}

void drawPolygon(std::vector<Point3D>& points) {
	glBegin(GL_LINE_STRIP); 
	{
		for (int i = 0; i < points.size(); ++i) {
			glVertex3f(points[i].x, points[i].y, points[i].z);
		}
	}
	glEnd();
}

void calculateBezier(std::vector<Point3D>& points) {
	Point3D p;
	int n = points.size() - 1;
	std::vector<int> factors(points.size());
	double t, b;

	computeFactors(factors);
	t = 0.00;
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
		bezierPoints.push_back(p);
		t += 0.01;

		if (t > 1.0)
			break;
	}
}

ObjectModel* removePolygons(ObjectModel* original) {
	//printf("Here\n");
	ObjectModel* n = new ObjectModel(original);
	std::vector<int> toRemove;
	int removed = 0;

	for (int i = 0; i < n->coeffs_.size(); ++i) {
		float result = n->coeffs_[i].a * eye.i + n->coeffs_[i].b * eye.j + n->coeffs_[i].c * eye.k + n->coeffs_[i].d;
		if (result < 0.0) {
			toRemove.push_back(n->coeffs_[i].face);
		}
	}

	for (int i = 0; i < toRemove.size(); ++i) {
		int index = toRemove[i] - removed;
		n->faces_.erase(n->faces_.begin() + index);
		// printf("Brisem\n");
		removed++;
	}
	return n;
	//printf("There\n");
}

void drawBody(ObjectModel* o) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	for (int i = 0; i < o->faces_.size(); ++i) {
		glBegin(GL_LINE_LOOP);
		{
			// printf("lice\n");
			float x1 = o->vertices_[o->faces_.at(i).v1].x;
			float y1 = o->vertices_[o->faces_.at(i).v1].y;
			float z1 = o->vertices_[o->faces_.at(i).v1].z;

			float x2 = o->vertices_[o->faces_.at(i).v2].x;
			float y2 = o->vertices_[o->faces_.at(i).v2].y;
			float z2 = o->vertices_[o->faces_.at(i).v2].z;

			float x3 = o->vertices_[o->faces_.at(i).v3].x;
			float y3 = o->vertices_[o->faces_.at(i).v3].y;
			float z3 = o->vertices_[o->faces_.at(i).v3].z;

			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
		}
		glEnd();
	}
	glutSwapBuffers();	
}

void renderScene() {
	// ObjectModel* n = new ObjectModel(original);
	ObjectModel* n = removePolygons(original);

	//pronadji matricu transformacije za postojece ociste i glediste
	float* MatrixViewT;
	float* MatrixProjectionT;
	MakeTransform(eye, center, &MatrixViewT, &MatrixProjectionT);
	float* Matrix;
	MatrixMultiply(MatrixViewT, 4, 4, MatrixProjectionT, 4, 4, &Matrix);
	ObjectModel* o = TransformCopy(n, Matrix);
	o->normalize();	

	free(MatrixViewT); free(MatrixProjectionT); free(Matrix);

	drawBody(o);
	delete n; delete o; //delete original;
}

void animateSceneEyeMovement() {
	calculateBezier(points);
	for(int i = 0; i < bezierPoints.size(); ++i) {
		eye.i = bezierPoints[i].x;
		eye.j = bezierPoints[i].y;
		eye.k = bezierPoints[i].z;

		ObjectModel* n = removePolygons(original);

		//pronadji matricu transformacije za postojece ociste i glediste
		float* MatrixViewT;
		float* MatrixProjectionT;
		MakeTransform(eye, center, &MatrixViewT, &MatrixProjectionT);
		float* Matrix;
		MatrixMultiply(MatrixViewT, 4, 4, MatrixProjectionT, 4, 4, &Matrix);
		ObjectModel* o = TransformCopy(n, Matrix);
		o->normalize();	
		free(MatrixViewT); free(MatrixProjectionT); free(Matrix);

		drawBody(o);		
		delete n; delete o;
		sleep(0.5);
	}
	printf("Animacija gotova.\n");
	//delete original;
}

void animateSceneCenterMovement() {
	calculateBezier(points);
	for(int i = 0; i < bezierPoints.size(); ++i) {
		center.i = bezierPoints[i].x;
		center.j = bezierPoints[i].y;
		center.k = bezierPoints[i].z;

		ObjectModel* n = removePolygons(original);

		//pronadji matricu transformacije za postojece ociste i glediste
		float* MatrixViewT;
		float* MatrixProjectionT;
		MakeTransform(eye, center, &MatrixViewT, &MatrixProjectionT);
		float* Matrix;
		MatrixMultiply(MatrixViewT, 4, 4, MatrixProjectionT, 4, 4, &Matrix);
		ObjectModel* o = TransformCopy(n, Matrix);
		o->normalize();	
		free(MatrixViewT); free(MatrixProjectionT); free(Matrix);

		drawBody(o);		
		delete n; delete o;
		sleep(1);
	}
	//glutSwapBuffers();
	printf("Animacija gotova.\n");
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
	//glFrustum(-7, 7, -7, 7, 9.0f, 10.0f);
	gluOrtho2D(-3, 3, -3 ,3);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display() {
	glClearColor(0.0, 1.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	// renderScene();

	animateSceneEyeMovement();
	// animateSceneCenterMovement();

}

void InputEyeCenter(char* filename) {
	FILE* f = fopen(filename, "r");
	if (!f) {
		printf ("Ne mogu ucitati koordinate ocista i gledista.\n");
		exit(1);
	}
	char c;
	fscanf(f, "%c", &c);
	// printf("%c\n", c);
	if (c == 'O') {
		fscanf(f, "%f %f %f\n", &eye.i, &eye.j, &eye.k);
		fscanf(f, "%c", &c);
		if (c != 'G') {
			printf("Neispravno zadana ulazna datoteka. Glediste nije dobro.\n");
			exit(1);
		}
		fscanf(f, "%f %f %f", &center.i, &center.j, &center.k);
		return;

	} else if (c == 'G') {
		fscanf(f, "%f %f %f\n", &center.i, &center.j, &center.k);
		fscanf(f, "%c", &c);
		if (c != 'O') {
			printf("Neispravno zadana ulazna datoteka. Ociste nije dobro.\n");
			exit(1);
		}
		fscanf(f, "%f %f %f", &eye.i, &eye.j, &eye.k);
		return;
	}

	printf("Neispravno zadana ulazna datoteka.\n");
	exit(1);
}

int main(int argc, char** argv) {
	if (argc != 4) {
		printf ("Hocu samo datoteku za krivulju i objekt te ociste i glediste\n");
		exit(1);
	}

	FILE* f = fopen(argv[1], "r");
	if (!f) {
		printf("Neuspjesno otvaranje datoteke.\n");
		exit(1);
	}

	ReadFile(points, f);
	fclose(f);

	original = new ObjectModel(argv[2]);
	InputEyeCenter(argv[3]);

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