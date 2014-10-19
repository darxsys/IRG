#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <string>
#include <vector>
#include "structures.h"

class ObjectModel {
public:
    std::vector<Vertex3D> vertices_;
    std::vector<Face3D> faces_;      
    std::vector<Plain3D> coeffs_;
    ObjectModel(char* filename);
    ObjectModel(ObjectModel* from);
    void readIn(char* filename);
    ObjectModel* copy();
    void normalize();
    std::string dumpToObj();
    void checkPoint(float x, float y, float z, int* status); // status == 0 - izvan tijela, status == 1 - na rubu tijela, status == 2 - unutar tijela
private:
    void setCoeffs();
    char* filename_;
};

#endif

