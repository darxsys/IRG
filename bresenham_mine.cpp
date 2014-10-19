#include <stdio.h>
#include <cmath>

void BresenhamLine(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    float dD = std::abs((float)dy/dx);
    printf("%f\n", dD);
    float D = dD - 0.5;
    int y = y0;

    for(int x = x0; x <= x1; ++x) {
        printf("Plotting: (%d, %d)\n", x, y);
        D += dD;        
        if (D > 0) {
            y += 1;
            D -= 1;
        }
    }
}

int main(void) {
    int x0, y0, x1, y1;
    printf("Enter the coordinates, please:\n");
    scanf("%d %d %d %d", &x0, &y0, &x1, &y1);
    BresenhamLine(x0, y0, x1, y1);
    return 0;
}