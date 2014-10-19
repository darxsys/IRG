#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    float a;
    float b;
    float c;
    float d;
} Plain3D;

typedef struct {
    float x;
    float y;
    float z;
} Vertex3D;

typedef struct {
    int v1;
    int v2;
    int v3;
} Face3D;

#endif // STRUCTURES_H