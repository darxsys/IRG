#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "object_model.h"
#include "matrix.h"

void MakeRotateMatrixY(float sin_alpha, float cos_alpha, float** T);
void MakeRotateMatrixZ(float sin_alpha, float cos_alpha, float** T);
void MakeTranslateMatrix(float x, float y, float z, float** T);
void MakeTransform(MyVector eye, MyVector center, float** MatrixViewT, float** MatrixProjectionT);

#endif
