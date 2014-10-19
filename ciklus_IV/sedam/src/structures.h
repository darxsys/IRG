#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    float a;
    float b;
    float c;
    float d;
    int face;
} Plain3D;

typedef struct {
    float x;
    float y;
    float z;
    float intensity;
} Vertex3D;

typedef struct {
    int v1;
    int v2;
    int v3;
    float intensity;
} Face3D;

#endif // STRUCTURES_H