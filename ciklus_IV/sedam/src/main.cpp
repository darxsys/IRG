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

ObjectModel* original;
MyVector eye;
MyVector center;
MyVector light_source;

int source_intensity;
int ambient_intensity;
float empirical_coef;
float ambient_coef;
bool state;

void drawBody(ObjectModel* o) {
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < o->faces_.size(); ++i) {
		glBegin(GL_LINE_LOOP);
		{
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
	//glutSwapBuffers();	
}

void drawConstantColoredBody(ObjectModel* o) {
	for (int i = 0; i < o->faces_.size(); ++i) {
		glBegin(GL_POLYGON);
		{
			float intensity = o->faces_[i].intensity;
			float x1 = o->vertices_[o->faces_.at(i).v1].x;
			float y1 = o->vertices_[o->faces_.at(i).v1].y;
			float z1 = o->vertices_[o->faces_.at(i).v1].z;

			float x2 = o->vertices_[o->faces_.at(i).v2].x;
			float y2 = o->vertices_[o->faces_.at(i).v2].y;
			float z2 = o->vertices_[o->faces_.at(i).v2].z;

			float x3 = o->vertices_[o->faces_.at(i).v3].x;
			float y3 = o->vertices_[o->faces_.at(i).v3].y;
			float z3 = o->vertices_[o->faces_.at(i).v3].z;

			glColor3f(intensity / 255, 0.5, 0);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
		}
		glEnd();
	}	
}

void drawGouraudColoredBody(ObjectModel* o) {
	int index;
	float intensity;
	for (int i = 0; i < o->faces_.size(); ++i) {
		glBegin(GL_POLYGON);
		{
			index = o->faces_.at(i).v1;
			// printf("Indeks: %d\n", index);
			intensity = o->vertices_[index].intensity;
			float x1 = o->vertices_[index].x;
			float y1 = o->vertices_[index].y;
			float z1 = o->vertices_[index].z;
			glColor3f(intensity / 255., 0, 0);
			glVertex3f(x1, y1, z1);

			index = o->faces_.at(i).v2;
			// printf("Indeks: %d\n", index);
			intensity = o->vertices_[index].intensity;
			float x2 = o->vertices_[index].x;
			float y2 = o->vertices_[index].y;
			float z2 = o->vertices_[index].z;
			// glColor3f(intensity / 255., 0, 0);
			glColor3f((float)intensity / 255., 0, 0);
			glVertex3f(x2, y2, z2);

			index = o->faces_.at(i).v3;
			// printf("Indeks: %d\n", index);
			intensity = o->vertices_[index].intensity;
			float x3 = o->vertices_[index].x;
			float y3 = o->vertices_[index].y;
			float z3 = o->vertices_[index].z;
			// glColor3f(intensity / 255., 0, 0);
			glColor3f((float)intensity / 255., 0, 0);
			glVertex3f(x3, y3, z3);
		}
		glEnd();
	}	
}

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

		// printf("Bila ova tocka: %f %f %f\n", *x1, *y1, *z1);

		MatrixMultiply(homogenous, 4, 1, Matrix, 4, 4, &result);
		float denominator = result[3];

		*x1 = result[0] / denominator;
		*y1 = result[1] / denominator;
		*z1 = result[2] / denominator;
		// printf("Postala ova tocka: %f %f %f\n", *x1, *y1, *z1);
		free(result);
	}

	return n;
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

void constantColoredScene() {
	MyVector L; MyVector N; float modul;
	Point3D poly_center;
	// std::vector<float> intensities(original->coeffs_.size());

	for (int i = 0; i < original->coeffs_.size(); ++i) {
		poly_center.x = (original->vertices_[original->faces_[original->coeffs_[i].face].v1].x + 
			original->vertices_[original->faces_[original->coeffs_[i].face].v3].x + 
			original->vertices_[original->faces_[original->coeffs_[i].face].v2].x) / 3;

		poly_center.y = (original->vertices_[original->faces_[original->coeffs_[i].face].v1].y + 
			original->vertices_[original->faces_[original->coeffs_[i].face].v3].y + 
			original->vertices_[original->faces_[original->coeffs_[i].face].v2].y) / 3;

		poly_center.z = (original->vertices_[original->faces_[original->coeffs_[i].face].v1].z + 
			original->vertices_[original->faces_[original->coeffs_[i].face].v3].z + 
			original->vertices_[original->faces_[original->coeffs_[i].face].v2].z) / 3;

		L.i = light_source.i - poly_center.x;
		L.j = light_source.j - poly_center.y;
		L.k = light_source.k - poly_center.z;
		modul = sqrt(L.i * L.i + L.j * L.j + L.k * L.k);
		L.i /= modul;
		L.j /= modul;
		L.k /= modul;

		N.i = original->coeffs_[i].a;
		N.j = original->coeffs_[i].b;
		N.k = original->coeffs_[i].c;
		modul = sqrt(N.i*N.i + N.j*N.j + N.k*N.k);
		N.i /= modul;
		N.j /= modul;
		N.k /= modul;

		float intensity = ambient_intensity * ambient_coef;
		float difuse = source_intensity * empirical_coef * MyVectorScalarProduct(L, N);
		if (difuse < 0) difuse = 0;
		intensity += difuse;
		// intensities[original->coeffs_[i].face] = intensity;
		original->faces_[original->coeffs_[i].face].intensity = intensity;
	}

	ObjectModel* n = removePolygons(original);
	float* MatrixViewT;
	float* MatrixProjectionT;
	MakeTransform(eye, center, &MatrixViewT, &MatrixProjectionT);
	float* Matrix;
	MatrixMultiply(MatrixViewT, 4, 4, MatrixProjectionT, 4, 4, &Matrix);
	// MatrixPrint(Matrix, 4, 4);
	ObjectModel* o = TransformCopy(n, Matrix);
	// o->normalize();
	// o->setCoeffs();
	free(MatrixViewT); free(MatrixProjectionT); free(Matrix);


	drawConstantColoredBody(o);
}

void gouraudColoredScene() {
	MyVector L; MyVector N; MyVector tmp_sum;
	Vertex3D v1, v2, v3;

	ObjectModel* n = original;

	std::vector<MyVector> sum_norms = *new std::vector<MyVector>(n->vertices_.size());
	std::vector<int> num_norms(n->vertices_.size(), 0);


	// srednje normale
	for (int i = 0; i < n->coeffs_.size(); ++i) {
		int index = n->faces_[n->coeffs_[i].face].v1;
		v1 = n->vertices_[index];

		N.i = n->coeffs_[i].a;
		N.j = n->coeffs_[i].b;
		N.k = n->coeffs_[i].c;
		N = MyVectorNorm(N);
		if (num_norms[index] == 0) {
			sum_norms[index].i = 0; 
			sum_norms[index].j = 0;
			sum_norms[index].k = 0;
		}
		MyVectorAdd(N, sum_norms[index], &tmp_sum);
		sum_norms[index] = tmp_sum;
		++num_norms[index];

		// if (index == 0) {
		// 	MyVectorPrint(sum_norms[index]);
		// }

		index = n->faces_[n->coeffs_[i].face].v2;
		v2 = n->vertices_[index];

		if (num_norms[index] == 0) {
			sum_norms[index].i = 0; 
			sum_norms[index].j = 0;
			sum_norms[index].k = 0;
		}
		MyVectorAdd(N, sum_norms[index], &tmp_sum);
		sum_norms[index] = tmp_sum;
		++num_norms[index];

		index = n->faces_[n->coeffs_[i].face].v3;
		v3 = n->vertices_[index];

		if (num_norms[index] == 0) {
			sum_norms[index].i = 0; 
			sum_norms[index].j = 0;
			sum_norms[index].k = 0;
		}
		MyVectorAdd(N, sum_norms[index], &tmp_sum);
		sum_norms[index] = tmp_sum;
		++num_norms[index];
	}

	// normiraj to sve
	for (int i = 0; i < sum_norms.size(); ++i) {
		sum_norms[i].i /= num_norms[i];
		sum_norms[i].j /= num_norms[i];
		sum_norms[i].k /= num_norms[i];
		sum_norms[i] = MyVectorNorm(sum_norms[i]);
		// MyVectorPrint(sum_norms[i]);
	}

	// izracunati intenzitete na osnovu normala u vrhovima i polozaja prema izvoru
	for (int i = 0; i < n->vertices_.size(); ++i) {
		L.i = light_source.i - n->vertices_[i].x;
		L.j = light_source.j - n->vertices_[i].y;
		L.k = light_source.k - n->vertices_[i].z;
		L = MyVectorNorm(L);

		float intensity = ambient_intensity * ambient_coef;
		double scalar = MyVectorScalarProduct(L, sum_norms[i]) * empirical_coef * source_intensity;
		if (scalar < 0.) {
			// printf("Manji sam od nula.\n");
			scalar = 0;
		}
		intensity += scalar;
		n->vertices_[i].intensity = intensity;
	}

	ObjectModel* o = removePolygons(original);
	float* MatrixViewT;
	float* MatrixProjectionT;
	MakeTransform(eye, center, &MatrixViewT, &MatrixProjectionT);
	float* Matrix;
	MatrixMultiply(MatrixViewT, 4, 4, MatrixProjectionT, 4, 4, &Matrix);
	// MatrixPrint(Matrix, 4, 4);
	n = TransformCopy(o, Matrix);
	// o->normalize();
	// o->setCoeffs();
	free(MatrixViewT); free(MatrixProjectionT); free(Matrix);
	drawGouraudColoredBody(n);
	delete o;
}

void renderScene() {
	ObjectModel* n = removePolygons(original);
	//pronadji matricu transformacije za postojece ociste i glediste
	float* MatrixViewT;
	float* MatrixProjectionT;
	MakeTransform(eye, center, &MatrixViewT, &MatrixProjectionT);
	float* Matrix;
	MatrixMultiply(MatrixViewT, 4, 4, MatrixProjectionT, 4, 4, &Matrix);
	// MatrixPrint(Matrix, 4, 4);
	ObjectModel* o = TransformCopy(n, Matrix);
	// o->normalize();
	// o->setCoeffs();

	free(MatrixViewT); free(MatrixProjectionT); free(Matrix);

	drawBody(o);
	glutSwapBuffers();
	delete n; 
	// delete o; //delete original;
	// renderColoredScene(o);
}

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-3, 3, -3 ,3);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display() {
	glClearColor(0.0, 1.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	renderScene();
	// renderColoredScene();
}

void keyboard(unsigned char theKey,int mouseX, int mouseY) {
	if (theKey == 'o') { // bojaj
		printf("Unesite polozaj izvora.\n");
		scanf("%f %f %f", &light_source.i, &light_source.j, &light_source.k);

		glClear(GL_COLOR_BUFFER_BIT);
		constantColoredScene();
		glutSwapBuffers();
		state = 0;
	} else if (theKey == 'g') {
		//Goro
		printf("Unesite polozaj izvora.\n");
		scanf("%f %f %f", &light_source.i, &light_source.j, &light_source.k);

		glClear(GL_COLOR_BUFFER_BIT);
		gouraudColoredScene();
		glutSwapBuffers();
		state = 1;
	} else if (state && theKey == 'a') {
		eye.i -= 1;
		glClear(GL_COLOR_BUFFER_BIT);
		gouraudColoredScene();
		glutSwapBuffers();		
	} else if (state && theKey == 'A') {
		eye.i += 1;
		glClear(GL_COLOR_BUFFER_BIT);
		gouraudColoredScene();
		glutSwapBuffers();			
	} else if (state && theKey == 'l') {
		light_source.i += 0.5;
		printf ("Trenutni polozaj izvora:\n");
		MyVectorPrint(light_source);
		// light_source.j += 0.5;
		// light_source.k += 0.5;
		glClear(GL_COLOR_BUFFER_BIT);
		gouraudColoredScene();
		glutSwapBuffers();		
	} else if (state && theKey == 'L') {
		light_source.i -= 0.5;
		printf ("Trenutni polozaj izvora:\n");
		MyVectorPrint(light_source);		
		// light_source.j -= 0.5;
		// light_source.k -= 0.5;
		glClear(GL_COLOR_BUFFER_BIT);
		gouraudColoredScene();
		glutSwapBuffers();		
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf ("./program objekt\n");
		exit(1);
	}

	original = new ObjectModel(argv[1]);
	original->normalize();

	eye.i = 3; eye.j = 4; eye.k = 7;
	center.i = 0; center.j = 0; center.k = 0;

	source_intensity = 190;
	ambient_intensity = 200;
	empirical_coef = 0.7;
	ambient_coef = 0.3;
	state = 0;

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(1200, 300);
	glutInit(&argc, argv);

	int window = glutCreateWindow("Primjer");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}