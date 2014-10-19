#include <stdio.h>
#include <GL/glut.h>

#include "object_model.h"
#include "matrix.h"
#include "transform.h"

ObjectModel* o;

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-3, 3, -3, 3, 3, 7);
	//glFrustum(-20, 20, -20, 20, -100, 100);
	glViewport(0, 0, width, height);
}

void display() {
	glClearColor(0.0, 1.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 3, 0, 1, 0);

	glColor3f(1.0, 0.0, 0.0);

	for (int i = 0; i < o->faces_.size(); ++i) {
		glBegin(GL_LINE_LOOP);
		{
			float x1 = o->vertices_[o->faces_.at(i)->v1]->x;
			float y1 = o->vertices_[o->faces_.at(i)->v1]->y;
			float z1 = o->vertices_[o->faces_.at(i)->v1]->z;

			float x2 = o->vertices_[o->faces_.at(i)->v2]->x;
			float y2 = o->vertices_[o->faces_.at(i)->v2]->y;
			float z2 = o->vertices_[o->faces_.at(i)->v2]->z;

			float x3 = o->vertices_[o->faces_.at(i)->v3]->x;
			float y3 = o->vertices_[o->faces_.at(i)->v3]->y;
			float z3 = o->vertices_[o->faces_.at(i)->v3]->z;

			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
		}
		glEnd();
	}
	// glBegin(GL_LINE_STRIP); 
	// {
	// 	glVertex3f(-0.9f, -0.9f, -0.9f);
	// 	glVertex3f(0.9f, -0.9f, -0.9f);
	// }
	// glEnd();

	// glBegin(GL_LINE_STRIP);
	// {
	// 	glVertex3f(-0.9f, -0.7f, -0.9f);
	// 	glVertex3f(0.9f, -0.7f, 3.1f);
	// }
	// glEnd();
	glutSwapBuffers();
}

void Transform(ObjectModel* o, float* Matrix) {
	float* homogenous = (float* )malloc(sizeof(float) * 4);
	homogenous[3] = 1;
	float* result;
	// printf("here\n");
	// MatrixPrint(Matrix, 4, 4);

	for (int i = 0; i < o->vertices_.size(); ++i) {
		float* x1 = &o->vertices_[i]->x;
		float* y1 = &o->vertices_[i]->y;
		float* z1 = &o->vertices_[i]->z;

		printf("radim s koordinatama:\n");
		printf("%f %f %f\n", *x1, *y1, *z1);

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


		printf("Preslikalo se u:\n");
		printf("%f %f %f\n", *x1, *y1, *z1);

		// homogenous[0] = *x2;
		// homogenous[1] = *y2;
		// homogenous[2] = *z2;
		// MatrixMultiply(homogenous, 4, 1, Matrix, 4, 4, &result);
		// denominator = result[3];
		// printf ("denominator: %f\n", denominator);

		// *x2 = result[0] / denominator;
		// *y2 = result[1] / denominator;
		// *z2 = result[2] / denominator;
		// free(result);

		// homogenous[0] = *x3;
		// homogenous[1] = *y3;
		// homogenous[2] = *z3;
		// MatrixMultiply(homogenous, 4, 1, Matrix, 4, 4, &result);
		// denominator = result[3];
		// printf ("denominator: %f\n", denominator);

		// *x3 = result[0] / denominator;
		// *y3 = result[1] / denominator;
		// *z3 = result[2] / denominator;
		// free(result);
	}

}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf ("Samo datoteku zelim na komandnoj liniji.\n");
		exit(1);
	}

	o = new ObjectModel(argv[1]);
	// o->normalize();
	printf("Unesite koordinate ocista i gledista\n");
	MyVector eye;
	MyVector center;
	MyVectorInput(&eye);
	MyVectorInput(&center);

	int status;
	o->checkPoint(eye.i, eye.j, eye.k, &status);
	while (status) {
		printf("Glediste ne moze biti unutar tijela. Unesite koordinate gledista.\n");
		MyVectorInput(&eye);
		o->checkPoint(eye.i, eye.j, eye.k, &status);
	}

	float* MatrixViewT;
	float* MatrixProjectionT;
	MakeTransform(eye, center, &MatrixViewT, &MatrixProjectionT);
	// MatrixPrint(MatrixViewT, 4, 4);
	// MatrixPrint(MatrixProjectionT, 4, 4);
	float* Matrix;
	MatrixMultiply(MatrixViewT, 4, 4, MatrixProjectionT, 4, 4, &Matrix);
	Transform(o, Matrix);
	//MatrixPrint(Matrix, 4, 4);
	//o->normalize();

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