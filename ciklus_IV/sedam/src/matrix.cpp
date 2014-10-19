#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

const char* INPUT_FORMAT = "%f %f %f";

void MyVectorInput(MyVector* v) {
    scanf(INPUT_FORMAT, &(v->i), &(v->j), &(v->k));
}

void MyVectorPrint(MyVector v) {
    printf("%fi%+fj%+fk\n", v.i, v.j, v.k);
}

void MyVectorAdd(MyVector v1, MyVector v2, MyVector* v3) {
    v3->i = v1.i + v2.i;
    v3->j = v1.j + v2.j;
    v3->k = v1.k + v2.k;
}

double MyVectorScalarProduct(MyVector v1, MyVector v2) {
    MyVector v3;
    v3.i = v1.i * v2.i;
    v3.j = v1.j * v2.j;
    v3.k = v1.k * v2.k;
    return v3.i + v3.j + v3.k;
}

void MyVectorProduct(MyVector v1, MyVector v2, MyVector *v3) {
    MyVector v4;
    v4.i = v1.j*v2.k - v2.j*v1.k;
    v4.j = -(v1.i*v2.k - v2.i*v1.k);
    v4.k = v1.i * v2.j - v2.i * v1.j;
    v3->i = v4.i;
    v3->j = v4.j;
    v3->k = v4.k;
}

double MyVectorAbs(MyVector v) {
    return sqrt(pow(v.i, 2) + pow(v.j, 2) + pow(v.k, 2));   
}

void MyVectorReverse(MyVector v1, MyVector* v2) {
    v2->i = -v1.i;
    v2->j = -v1.j;
    v2->k = -v1.k;
}

void MyVectorNormed(MyVector v1, MyVector* v2) {
    double v1_abs = MyVectorAbs(v1);
    v2->i = v1.i / v1_abs;
    v2->j = v1.j / v1_abs;
    v2->k = v1.k / v1_abs;
}

MyVector MyVectorNorm(MyVector L) {
    float modul = sqrt(L.i * L.i + L.j * L.j + L.k * L.k);
    L.i /= modul;
    L.j /= modul;
    L.k /= modul;
    return L;
}

void MatrixInput(float* matrix, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        scanf("%f", &matrix[i]);
    }
}

void MatrixAdd(float* matrix1, float* matrix2, int width, int height, float** M) {
    *M = (float *)malloc(width * height * sizeof(float));
    for (int i = 0; i < width * height; ++i) {
        (*M)[i] = matrix1[i] + matrix2[i];
    }
}

void MatrixPrint(float* M, int width, int height) {
    //printf("%f\n\n", M[3]);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("%f ", M[i * width + j]);
        }
        printf("\n");
    }
}

void MatrixTranspose(float* M1, int width, int height, float** M2) {
    *M2 = (float *)malloc(width * height * sizeof(float));

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            (*M2)[i * width + j] = M1[j * width + i];
        }
    }
}

void MatrixMultiply(float* M1, int width1, int height1, float* M2, int width2, int height2, float** M3) {
    *M3 = (float *)malloc(height1 * width2 * sizeof(float));

    for (int i = 0; i < height1; ++i) {
        for (int j = 0; j < width2; ++j) {
            (*M3)[i * width2 + j] = 0;
            // printf("Analysing row i: %d from first and column j: %d from second\n", i, j);
            for (int k = 0; k < width1; ++k) {
                (*M3)[i * width2 + j] += M1[i * width1 + k] * M2[k * width2 + j];
            }
        }
    }
}

void MatrixInvert3x3(float *M1, float** M2) {
    *M2 = (float *)malloc(9 * sizeof(float));
    int determinant = M1[0] * (M1[4] * M1[8] - M1[5] * M1[7]) - M1[1] 
        * (M1[3] * M1[8] - M1[6] * M1[5]) + M1[2] * (M1[3] * M1[7] - M1[4] * M1[6]);
    if (!determinant) {
        printf("This matrix has no inverse. Exiting\n"); 
        exit(1);
    }
    (*M2)[0] = (M1[4] * M1[8] - M1[5] * M1[7]) / determinant;
    (*M2)[1] = (M1[2] * M1[7] - M1[1] * M1[8]) / determinant;
    (*M2)[2] = (M1[1] * M1[5] - M1[2] * M1[4]) / determinant;
    (*M2)[3] = (M1[5] * M1[6] - M1[3] * M1[8]) / determinant;
    (*M2)[4] = (M1[0] * M1[8] - M1[2] * M1[6]) / determinant;
    (*M2)[5] = (M1[2] * M1[3] - M1[0] * M1[5]) / determinant;
    (*M2)[6] = (M1[3] * M1[7] - M1[4] * M1[6]) / determinant;
    (*M2)[7] = (M1[1] * M1[6] - M1[0] * M1[7]) / determinant;
    (*M2)[8] = (M1[0] * M1[4] - M1[1] * M1[3]) / determinant;
}

void InitializeMatrix(float** M, int width, int height) {
    (*M) = (float* )malloc(sizeof(float) * width * height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == j) {
                //++j;
                (*M)[i * width + j] = 1;
            } else {
                (*M)[i * width + j] = 0;
            }
        }
    }
}