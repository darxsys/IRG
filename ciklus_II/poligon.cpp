#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <vector>

typedef struct {
    int x;
    int y;
} Point2D;

typedef struct {
    int a;
    int b;
    int c;
} Edge2D;

typedef struct {
    Point2D vertex;
    Edge2D edge;
    bool left;
} PolyElem;

std::vector<PolyElem*> polygon_elements;

GLuint window_width = 300;
GLuint window_height = 300;
bool mouse_passive = false;
bool my_state = false;
bool convex = false;
bool fill_flag = false;

//void CheckOrientation(std::vector<PolyElem*>& polygon_elements, int* konv, int* orij)


bool CheckIfConvex(std::vector<PolyElem*>& elements, int* orientation) {
    int n = elements.size();
    if (n < 3)
        return true;

    int i, i0, r;
    int on = 0;
    int below = 0; 
    int above = 0;

    i0 = n - 2;
    for (i = 0; i < n; ++i, ++i0) {
        if (i0 >= n) {
            i0 = 0;
        }
        r = elements[i0]->edge.a * elements[i]->vertex.x + elements[i0]->edge.b * elements[i]->vertex.y
            + elements[i0]->edge.c;

        if (r == 0) on++;
        else if (r > 0) above++;
        else below++; 
    }
    *orientation = 0;    
    if (below == 0) {
        return true;
    } else if (above == 0) {
        *orientation = 1;
        return true;
    }
    return false;
}

void CheckInPolygon(std::vector<PolyElem*>& elements, int x, int y, int* status) {
    if (elements.size() < 2) {
        //printf("Niste zadali poligon!\n");
        *status = -1;
        return;
    }

    int n = elements.size();
    int current;
    int last;
    int i;

    last = elements[0]->edge.a * x + elements[0]->edge.b * y + elements[0]->edge.c;
    if (!last) {
        *status = 1;
        return;
    }
    for (i = 1; i < n; ++i) {
        current = elements[i]->edge.a * x + elements[i]->edge.b * y + elements[i]->edge.c;
        if ((current < 0 && last > 0) || (current > 0 && last < 0)) {
            *status = 0;
            return;
        }
        if (!current) {
            *status = 1;
            return;
        }
    }
    *status = 2;
}

void FillPolygon(std::vector<PolyElem*>& elements) {
    int i, i0, y;
    int xmin, xmax, ymin, ymax;
    double L, D, x;
    int n = elements.size();
    int tmp;

    if (n < 3)
        return;

    xmin = xmax = elements[0]->vertex.x;
    ymin = ymax = elements[0]->vertex.y;
    for (i = 1; i < n; ++i) {
        if (xmin > elements[i]->vertex.x) xmin = elements[i]->vertex.x;
        if (xmax < elements[i]->vertex.x) xmax = elements[i]->vertex.x;
        if (ymin > elements[i]->vertex.y) ymin = elements[i]->vertex.y;
        if (ymax < elements[i]->vertex.y) ymax = elements[i]->vertex.y;
    }

    for (y = ymin; y <= ymax; ++y) {
        L = xmin; D = xmax;
        i0 = n - 1;
        for (i = 0; i < n; ++i) {
            //Edge is horizontal
            if (elements[i]->edge.a == 0) {
                if (elements[i]->vertex.y == y) {
                    if (elements[i0]->vertex.x < elements[i]->vertex.x) {
                        L = elements[i0]->vertex.x;
                        D = elements[i]->vertex.x;
                    } else {
                        L = elements[i]->vertex.x;
                        D = elements[i0]->vertex.x;
                    }
                    break;
                }
            } else {
                x = (-elements[i0]->edge.b * y - elements[i0]->edge.c) / (double)elements[i0]->edge.a;
                CheckIfConvex(elements, &tmp);
                if (tmp) {
                    if (elements[i0]->left) {
                        if (L < x) L = x;
                    } else {
                        if (D > x) D = x;
                    }
                } else {
                    if (!elements[i0]->left) {
                        if (L < x) L = x;
                    } else {
                        if (D > x) D = x;
                    }                    
                }
            }
            i0 = i;
        }
        glBegin(GL_LINES); {
            glVertex2i(ceil(L), window_height - y);
            glVertex2i(ceil(D), window_height - y);
        }
        glEnd();
    }
}

void CalculatePolygonCoeff(std::vector<PolyElem*>& elements) {
    int n = elements.size();
    int i = 0;
    int i0 = n-1;

    if (n < 2) {
        elements[0]->edge.a = 0;
        elements[0]->edge.b = 0;
        elements[0]->edge.c = 0;
        return;
    }

    for (i = 0; i < n; i += (n - 1)) {
        elements[i0]->edge.a = elements[i0]->vertex.y - elements[i]->vertex.y;
        elements[i0]->edge.b = -(elements[i0]->vertex.x - elements[i]->vertex.x);
        elements[i0]->edge.c = elements[i0]->vertex.x * elements[i]->vertex.y 
                                - elements[i0]->vertex.y * elements[i]->vertex.x;

        //printf("Vrh %d, koeficijenti %d %d %d\n", i0, elements[i0]->edge.a, elements[i0]->edge.b, elements[i0]->edge.c);

        elements[i0]->left = (elements[i0]->vertex.y < elements[i]->vertex.y) ? true : false;
        i0 = n - 2;
    }
}

void DrawPolygon(std::vector<PolyElem*>& elements) {
    int n = elements.size();
    int i = 0;
    int i0 = n-1;

    glBegin(GL_LINES); {
        for (i = 0; i < n; ++i) {
            glVertex2i(elements[i]->vertex.x, 
                window_height - elements[i]->vertex.y);
            glVertex2i(elements[i0]->vertex.x, 
                window_height - elements[i0]->vertex.y);
            i0 = i;
        }
    }
    glEnd();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
    polygon_elements.clear();
}

void Reshape(int width, int height) {
    window_height = height;
    window_width = width;
    polygon_elements.clear();

    glViewport(0, 0, window_width, window_height);
    glLoadIdentity();
    gluOrtho2D(0, window_width, 0, window_height);
    if (convex) {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    } else {
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    }    
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSwapBuffers();
}

void MouseClick(int button, int state, int x, int y) {
    if (!my_state && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        PolyElem* poly = (PolyElem* )malloc(sizeof(PolyElem));
        poly->vertex.x = x;
        poly->vertex.y = y;
        int tmp;

        if (mouse_passive) {
            polygon_elements[polygon_elements.size() - 1] = poly;
        } else {
            polygon_elements.push_back(poly);
        }
        glClear(GL_COLOR_BUFFER_BIT);

        if (convex && !CheckIfConvex(polygon_elements, &tmp)) {
            printf("Tocka nije dobra, nije sacuvana konveksnost poligona!\n");
            polygon_elements.erase(polygon_elements.end() - 1);
            //CalculatePolygonCoeff(polygon_elements);
        }
        CalculatePolygonCoeff(polygon_elements);
        DrawPolygon(polygon_elements);

        if (fill_flag) {
            FillPolygon(polygon_elements);
        }
        mouse_passive = false;
        glutSwapBuffers();

    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //Check if a point is inside of a polygon
        int check;
        CheckInPolygon(polygon_elements, x, y, &check);
        if (!check) {
            printf("Tocka nije u poligonu.\n");
        } else if (check == 1) {
            printf("Tocka je na bridu poligona.\n");
        } else if (check == 2) {
            printf("Tocka je u poligonu.\n");
        } else {
            printf("Niste zadali poligon.\n");
        }
    }
}

void MousePassive(int x, int y) {
    if (!my_state && polygon_elements.size() > 0) {
        PolyElem* poly = (PolyElem* )malloc(sizeof(PolyElem));
        poly->vertex.x = x;
        poly->vertex.y = y; 

        if (mouse_passive) {
            polygon_elements[polygon_elements.size() - 1] = poly;
        } else {
            polygon_elements.push_back(poly);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        CalculatePolygonCoeff(polygon_elements);
        DrawPolygon(polygon_elements);
        if (fill_flag) {
            FillPolygon(polygon_elements);
        }
        mouse_passive = true;
        glutSwapBuffers();
    }
}

void Keyboard(unsigned char key, int mouseX, int mouseY) {
    int tmp;
    switch (key) {
        case 'k':
            if (!my_state) {
                if (!convex) {
                    if (!CheckIfConvex(polygon_elements, &tmp)) {
                        printf("Nije moguce promijeniti zastavicu.\n Trenutni poligon nije konveksan.\n");
                    } else {
                        convex = true;
                        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
                        //glClear(GL_COLOR_BUFFER_BIT);
                    }
                } else {
                    convex = false;
                    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
                }
                glClear(GL_COLOR_BUFFER_BIT);
                DrawPolygon(polygon_elements);
                if (fill_flag)
                    FillPolygon(polygon_elements);
                glutSwapBuffers();                
               
            }
            break;
        
        case 'p':
            if (!my_state) {
                fill_flag ^= 1;
                if (fill_flag) {
                    FillPolygon(polygon_elements);
                } else {
                    glClear(GL_COLOR_BUFFER_BIT);
                    DrawPolygon(polygon_elements);
                }
                glutSwapBuffers();
            }
            break;

        case 'n':
            my_state ^= 1;
            if (!my_state) {
                polygon_elements.clear();
                convex = fill_flag = false;
                glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glutSwapBuffers();
            }
    }
    //printf("here\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Crtanje i ispunjavanje poligona.");

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutPassiveMotionFunc(MousePassive);
    glutMouseFunc(MouseClick);
    glutKeyboardFunc(Keyboard);

    //glColor3f(0.0f, 0.0f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    //glutSwapBuffers();
    glutMainLoop();
    return 0;
}