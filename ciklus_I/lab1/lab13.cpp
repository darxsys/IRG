#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float x;
    float y;
    float z;
} Point;

void MatrixPrint(float* M, int width, int height) {
    //printf("%f\n\n", M[3]);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("%f ", M[i * width + j]);
        }
        printf("\n");
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

void PointInput(Point* P) {
    scanf("%f %f %f", &(P->x), &(P->y), &(P->z));
}

void CalculateCoordinates(Point A, Point B, Point C, Point T, Point* P) {
    float *M1 = (float* )malloc(9 * sizeof(float));
    M1[0] = A.x;
    M1[1] = B.x;
    M1[2] = C.x;
    M1[3] = A.y;
    M1[4] = B.y;
    M1[5] = C.y;
    M1[6] = A.z;
    M1[7] = B.z;
    M1[8] = C.z;

    float *M2 = (float* )malloc(3 * sizeof(float));
    M2[0] = T.x;
    M2[1] = T.y;
    M2[2] = T.z;

    float *m1_inverse;
    MatrixInvert3x3(M1, &m1_inverse);
    float *result;
    MatrixMultiply(m1_inverse, 3, 3, M2, 1, 3, &result);

    P->x = result[0];
    P->y = result[1];
    P->z = result[2];
}

void PointPrint(Point P) {
    printf("%f %f %f\n", P.x, P.y, P.z);
}

int main(void) {
    Point A, B, C, T, P;
    PointInput(&A);
    PointInput(&B);
    PointInput(&C);
    PointInput(&T);
    CalculateCoordinates(A, B, C, T, &P);
    PointPrint(P);
    return 0;
}