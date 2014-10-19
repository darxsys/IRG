#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    float i;
    float j;
    float k;
} MyVector;

void MyVectorInput(MyVector* v);
void MyVectorPrint(MyVector v);
void MyVectorAdd(MyVector v1, MyVector v2, MyVector* v3);
double MyVectorScalarProduct(MyVector v1, MyVector v2);
void MyVectorProduct(MyVector v1, MyVector v2, MyVector* v3);
MyVector MyVectorNorm(MyVector L);
double MyVectorAbs(MyVector v);
void MyVectorReverse(MyVector v1, MyVector* v2);
void MyVectorNormed(MyVector v1, MyVector* v2);
void MatrixInput(float* matrix, int width, int height);
void MatrixAdd(float* matrix1, float* matrix2, int width, int height, float** M);
void MatrixPrint(float* M, int width, int height);
void MatrixTranspose(float* M1, int width, int height, float** M2);
void MatrixMultiply(float* M1, int width1, int height1, float* M2, int width2, int height2, float** M3);
void MatrixInvert3x3(float *M1, float** M2);
void InitializeMatrix(float** M, int width, int height);
#endif