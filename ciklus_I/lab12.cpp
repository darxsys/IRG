#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int main(void) {
    float M1[9], M2[3], *M3;
    float *inverse;
    int i = 0;
    for (i = 0; i < 3; ++i) {
        scanf("%f", &M1[i]);
    }
    scanf("%f", &M2[0]);
    for (; i < 6; ++i) {
        scanf("%f", &M1[i]);
    }
    scanf("%f", &M2[1]);
    for(; i < 9; ++i) {
        scanf("%f", &M1[i]);
    }
    scanf("%f", &M2[2]);
    //printf("%f\n\n", M2[3]);

    //MatrixPrint(M1, 3, 3);
    //MatrixPrint(M2, 1, 3);
    MatrixInvert3x3(M1, &inverse);
    //MatrixPrint(inverse, 3, 3);
    MatrixMultiply(inverse, 3, 3, M2, 1, 3, &M3);
    MatrixPrint(M3, 1, 3);
    return 0;
}