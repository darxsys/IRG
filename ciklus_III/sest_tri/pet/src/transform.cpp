#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "transform.h"
#include "matrix.h"

void MakeTransform(MyVector eye, MyVector center, float** MatrixViewT, float** MatrixProjectionT) {
	float* T1;
	float* center_homogenous = (float* )malloc(4 * sizeof(float));
	center_homogenous[0] = center.i;
	center_homogenous[1] = center.j;
	center_homogenous[2] = center.k;
	center_homogenous[3] = 1;

	float* in_between1;
	// printf("Here\n");

	InitializeMatrix(&T1, 4, 4);	
	MakeTranslateMatrix(-eye.i, -eye.j, -eye.k, &T1);
	//  MatrixPrint(T1, 4, 4);
	// printf("\n");	
	MatrixMultiply(center_homogenous, 4, 1, T1, 4, 4, &in_between1);
	//MatrixPrint(in_between1, 4, 1);

	float* T2;
	float denominator = sqrt(in_between1[0] * in_between1[0] + in_between1[1] * in_between1[1]);
	InitializeMatrix(&T2, 4, 4);
	if (denominator != 0.0) {
		float sin_alpha = in_between1[1] / denominator;
		float cos_alpha = in_between1[0] / denominator;
		MakeRotateMatrixZ(sin_alpha, cos_alpha, &T2);		
	}
	
	
	//  MatrixPrint(T2, 4, 4);
	// printf("\n");

	float* in_between2;
	MatrixMultiply(in_between1, 4, 1, T2, 4, 4, &in_between2);
	//MatrixPrint(in_between2, 4, 1);


	float* T3;
	denominator = sqrt(in_between2[0] * in_between2[0] + in_between2[2] * in_between2[2]);
	InitializeMatrix(&T3, 4, 4);
	float H = 0;	
	if (denominator != 0.0) {
		float sin_beta = in_between2[0] / denominator;
		float cos_beta = in_between2[2] / denominator;
		MakeRotateMatrixY(sin_beta, cos_beta, &T3);
		H = denominator;
	}
	 // MatrixPrint(T3, 4, 4);
	 // printf("\n");

	float* T4;
	InitializeMatrix(&T4, 4, 4);
	T4[0] = 0;
	T4[1] = -1;
	T4[4] = 1;
	T4[5] = 0;
	 // MatrixPrint(T4, 4, 4);

	float* T5;
	InitializeMatrix(&T5, 4, 4);
	T5[0] = -1;
	 // MatrixPrint(T5, 4, 4);

	float* temp1;
	float* temp2;
	MatrixMultiply(T1, 4, 4, T2, 4, 4, &temp1);
	// MatrixMultiply(temp1, 4, 4, T3, 4, 4, MatrixViewT);
	MatrixMultiply(temp1, 4, 4, T3, 4, 4, &temp2);
	free(temp1);
	MatrixMultiply(temp2, 4, 4, T4, 4, 4, &temp1);
	free(temp2);
	MatrixMultiply(temp1, 4, 4, T5, 4, 4, MatrixViewT);

	InitializeMatrix(MatrixProjectionT, 4, 4);
	(*MatrixProjectionT)[10] = 0;
	(*MatrixProjectionT)[15] = 0;
	//Projection
	if (H != 0.0) {
		(*MatrixProjectionT)[11] = 1 / H;
		
	} else {
		(*MatrixProjectionT)[11] = 1;
	}
	// MatrixPrint(*MatrixProjectionT, 4, 4);
}

void MakeTranslateMatrix(float x, float y, float z, float** T) {
	//(*T) = (float* )malloc(sizeof(float) * 16);
	(*T)[0] = (*T)[5] = (*T)[10] = (*T)[15] = 1;
	(*T)[12] = x; (*T)[13] = y; (*T)[14] = z;

}

void MakeRotateMatrixZ(float sin_alpha, float cos_alpha, float** T) {
	//(*T) = (float* )malloc(sizeof(float) * 16);
	(*T)[0] = (*T)[5] = cos_alpha;
	(*T)[1] = -sin_alpha;	
	(*T)[4] = sin_alpha;
}

void MakeRotateMatrixY(float sin_alpha, float cos_alpha, float** T) {
	(*T)[0] = (*T)[10] = cos_alpha;
	(*T)[2] = sin_alpha;
	(*T)[8] = -sin_alpha;
}

