#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//TODO Matrices into structures

typedef struct {
    float i;
    float j;
    float k ;
} Vector;

const char* INPUT_FORMAT = "%f %f %f";

void VectorInput(Vector* v) {
    scanf(INPUT_FORMAT, &(v->i), &(v->j), &(v->k));
}

//TODO Make a fine looking output
void VectorPrint(Vector v) {
    printf("%fi + %fj + %fk\n", v.i, v.j, v.k);
}

void VectorAdd(Vector v1, Vector v2, Vector* v3) {
    v3->i = v1.i + v2.i;
    v3->j = v1.j + v2.j;
    v3->k = v1.k + v2.k;
}

double VectorScalarProduct(Vector v1, Vector v2) {
    Vector v3;
    v3.i = v1.i * v2.i;
    v3.j = v1.j * v2.j;
    v3.k = v1.k * v2.k;
    return v3.i + v3.j + v3.k;
}

void VectorProduct(Vector v1, Vector v2, Vector *v3) {
    Vector v4;
    v4.i = v1.j*v2.k - v2.j*v1.k;
    v4.j = -(v1.i*v2.k - v2.i*v1.k);
    v4.k = v1.i * v2.j - v2.i * v1.j;
    v3->i = v4.i;
    v3->j = v4.j;
    v3->k = v4.k;
}

double VectorAbs(Vector v) {
    return sqrt(pow(v.i, 2) + pow(v.j, 2) + pow(v.k, 2));   
}

void VectorReverse(Vector v1, Vector* v2) {
    v2->i = -v1.i;
    v2->j = -v1.j;
    v2->k = -v1.k;
}

void VectorNormed(Vector v1, Vector* v2) {
    double v1_abs = VectorAbs(v1);
    v2->i = v1.i / v1_abs;
    v2->j = v1.j / v1_abs;
    v2->k = v1.k / v1_abs;
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
            //printf("Analysing row i: %d from first and column j: %d from second\n", i, j);
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

int main(void) {
    Vector v1, v2, v3, v4;
    float s;
    //printf("Unesite komponente prvoga i drugoga vektora\n");
    VectorInput(&v1);
    VectorInput(&v2);
    printf("\n");

    //Task 1
    VectorAdd(v1, v2, &v1);
    VectorPrint(v1);
    printf("\n");
    //Task 2
    s = VectorScalarProduct(v1, v2);
    printf("%f\n", s);
    printf("\n");

    //Task 3
    //printf("Unesite vektor s kojim zelite vektorski pomnoziti v1\n");
    VectorInput(&v2);
    VectorProduct(v1, v2, &v2);
    VectorPrint(v2);
    printf("\n");

    //Task 4
    //TODO Check this because of double stuff
    VectorNormed(v2, &v3);
    VectorPrint(v3);
    printf("\n");

    //Task 5
    VectorReverse(v2, &v4);
    VectorPrint(v4);
    printf("\n");

    //Task 6
    float *matrix1, *matrix2, *M1, *M2, *M3;
    int width1, height1, width2, height2;
    //printf("Unesite matrice za zbrajanje (dimenzije pa brojke)\n");
    scanf("%d %d", &width1, &height1);
    matrix1 = (float* )malloc(width1 * height1 * sizeof(float));
    MatrixInput(matrix1, width1, height1);
    scanf("%d %d", &width2, &height2);
    matrix2 = (float* )malloc(width2 * height2 * sizeof(float));
    MatrixInput(matrix2, width2, height2);
    MatrixAdd(matrix1, matrix2, width2, height2, &M1);
    MatrixPrint(M1, width2, height2);
    printf("\n");

    //Task 7
    float* tmp;
    MatrixTranspose(matrix2, width2, height2, &tmp);
    //MatrixPrint(M2, height2, width2);
    MatrixMultiply(matrix1, width1, height1, tmp, height2, width2, &M2);
    MatrixPrint(M2, width1, width2);
    printf("\n");

    //Task 8
    free(tmp);
    MatrixInvert3x3(matrix2, &tmp);
    MatrixMultiply(matrix1, width1, height1, tmp, width2, height2, &M3);
    MatrixPrint(M3, width1, height2);

    free(tmp);
    free(matrix1);
    free(matrix2);
    free(M1);
    free(M2);
    free(M3);
    return 0;
}