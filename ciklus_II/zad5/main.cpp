#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "object_model.h"

typedef struct {
    float i;
    float j;
    float k;
} Vector;

void VectorProduct(Vector v1, Vector v2, Vector *v3) {
    Vector v4;
    v4.i = v1.j*v2.k - v2.j*v1.k;
    v4.j = -(v1.i*v2.k - v2.i*v1.k);
    v4.k = v1.i * v2.j - v2.i * v1.j;
    v3->i = v4.i;
    v3->j = v4.j;
    v3->k = v4.k;
}

ObjectModel::ObjectModel(ObjectModel* from) {
    for (int i = 0; i < from->vertices_.size(); ++i) {
        this->vertices_.push_back(from->vertices_[i]);
    }
    for (int i = 0; i < this->faces_.size(); ++i) {
        this->faces_.push_back(from->faces_[i]);
    }

}

ObjectModel::ObjectModel(char* filename): filename_(filename) {
    this->readIn(filename_);
    this->setCoeffs();
    //std::cout << this->dumpToObj();

}

void ObjectModel::readIn(char* filename) {
    FILE* input_file = fopen(filename, "r");
    if (!input_file) {
        printf("Nemoguce ucitati ulaznu datoteku.\n");
        exit(1);
    }

    char c;
    while(fscanf(input_file, "%c\n", &c) != EOF) {
        if (c == 'v') {
            Vertex3D* v = (Vertex3D* )malloc(sizeof(Vertex3D));
            fscanf(input_file, "%f %f %f", &v->x, &v->y, &v->z);
            this->vertices_.push_back(v);
        } else if (c == 'f') {
            Face3D* f = (Face3D* )malloc(sizeof(Face3D));
            fscanf(input_file, "%d %d %d", &f->v1, &f->v2, &f->v3);
            f->v1--; f->v2--; f->v3--;
            this->faces_.push_back(f);
        }
    }
}

std::string ObjectModel::dumpToObj() {
    std::string obj_model("# Dumped object model\n");
    char buff[100];

    std::vector<Vertex3D*>::iterator it = this->vertices_.begin();

    for (; it != this->vertices_.end(); ++it) {
        //std::stringstream ss;
        //ss << "v " << this->vertices_[i]->x << ' ' << this->vertices_[i]->y << ' ' << this->vertices_[i]->z << "\n";
        sprintf(buff,"v %f %f %f\n", (*it)->x, (*it)->y, (*it)->z);
        obj_model.append(std::string(buff));
    }

    for (int i = 0; i < faces_.size(); ++i) {
        std::stringstream ss;
        ss << "f " << this->faces_[i]->v1 + 1 << ' ' << this->faces_[i]->v2 + 1 << ' ' << this->faces_[i]->v3 + 1 << "\n";
        obj_model.append(ss.str());
    }
    return obj_model;
}

ObjectModel* ObjectModel::copy() {
    //ObjectModel* copy = new ObjectModel(filename_);
    ObjectModel* copy = new ObjectModel(this);
    return copy;
}

void ObjectModel::normalize() {
    float xmin, xmax, ymin, ymax, zmin, zmax;
    xmin = xmax = this->vertices_[0]->x;
    ymin = ymax = this->vertices_[0]->y;
    zmin = zmax = this->vertices_[0]->z;
    float x_, y_, z_;

    for (int i = 1; i < this->vertices_.size(); ++i) {
        if (xmin > this->vertices_[i]->x)
            xmin = this->vertices_[i]->x;
        if (xmax < this->vertices_[i]->x)
            xmax = this->vertices_[i]->x;
        if (ymin > this->vertices_[i]->y)
            ymin = this->vertices_[i]->y;
        if (ymax < this->vertices_[i]->y)
            ymax = this->vertices_[i]->y;
        if (zmin > this->vertices_[i]->z)
            zmin = this->vertices_[i]->z;
        if (zmax < this->vertices_[i]->z)
            zmax = this->vertices_[i]->z;
    }

    x_ = (xmin + xmax) / 2;
    y_ = (ymin + ymax) / 2;
    z_ = (zmin + zmax) / 2;
    float M = std::max(xmax - xmin, std::max(ymax - ymin, zmax - zmin));

    for (int i = 0; i < this->vertices_.size(); ++i) {
        this->vertices_[i]->x = (this->vertices_[i]->x - x_) * 2 / M;
        this->vertices_[i]->y = (this->vertices_[i]->y - y_) * 2 / M;
        this->vertices_[i]->z = (this->vertices_[i]->z - z_) * 2 / M;
    }
}

void ObjectModel::checkPoint(float x, float y, float z, int* status) {
    int num_edge = 0;
    for (int i = 0; i < coeffs_.size(); ++i) {
        float r = coeffs_[i]->a*x + coeffs_[i]->b*y + coeffs_[i]->c*z + coeffs_[i]->d;
        if (r > 0) {
            *status = 0;
            return;
        } else if (r == 0) {
            num_edge++;
        }
    }
    if (num_edge)
        *status = 1;
    else
        *status = 2;
}

void ObjectModel::setCoeffs() {
    Vector v1, v2, v3;
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    //Calculate coeffs
    for (int i = 0; i < this->faces_.size(); ++i) {

        x1 = vertices_[faces_[i]->v1]->x;
        y1 = vertices_[faces_[i]->v1]->y;
        z1 = vertices_[faces_[i]->v1]->z;
        x2 = vertices_[faces_[i]->v2]->x;
        y2 = vertices_[faces_[i]->v2]->y;
        z2 = vertices_[faces_[i]->v2]->z;
        x3 = vertices_[faces_[i]->v3]->x;
        y3 = vertices_[faces_[i]->v3]->y;
        z3 = vertices_[faces_[i]->v3]->z;

        v1.i = (x2 - x1);
        v1.j = (y2 - y1);
        v1.k = (z2 - z1);

        v2.i = (x3 - x1);
        v2.j = (y3 - y1);
        v2.k = (z3 - z1);

        VectorProduct(v1, v2, &v3);

        Plain3D* plain = (Plain3D* )malloc(sizeof(Plain3D));
        plain->a = v3.i;
        plain->b = v3.j;
        plain->c = v3.k;
        plain->d = -(plain->a*x1 + plain->b*y1 + plain->c*z1);
        coeffs_.push_back(plain);
    }    

}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Trebam ulaznu .obj datoteku kao prvi i jedini argument komandne linije.\n");
        exit(1);
    }

    char* filename = argv[1];
    ObjectModel* obj = new ObjectModel(filename);

    printf("Unosite naredbe\n");
    float x, y, z;
    int status;
    char prompt[100];
    while(1) {
        fgets(prompt, sizeof(prompt), stdin);
        if (!strcmp(prompt, "normiraj\n")) {
            obj->normalize();
            std::cout << obj->dumpToObj();
            //getchar();
            continue;
        } else if (!strcmp(prompt, "quit\n")) {
            break;
        } else {
            sscanf(prompt, "%f %f %f", &x, &y, &z);
            obj->checkPoint(x, y, z, &status);
            if (!status) {
                printf("Tocka nije unutar tijela!\n");
            } else if (status == 1) {
                printf("Tocka je na rubu tijela\n");
            } else {
                printf("Tocka je unutar tijela\n");
            }
        }
    }

    return 0;
}