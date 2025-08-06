#include <iostream>
#include <cmath>
#include <cstring>
#include <unistd.h>

using namespace std;

int main() {

   int dots[8][3] = {
        {-1,-1,-1},{1,-1,-1},
        {-1,1,-1},{-1,-1,1},
        {1,1,-1},{1,-1,1},
        {-1,1,1},{1,1,1}
    };
    int edges[12][2] = {
        {0, 1}, {1, 4}, {4, 2}, {2, 0},
        {3, 5}, {5, 7}, {7, 6}, {6, 3},
        {0, 3}, {1, 5}, {4, 7}, {2, 6}
    };


    float A = 0, B = 0;
    const int width = 44;
    const int height = 22;

    for (;;) {
        float zbuff[968] = {0};
        char screen[968];
        memset(screen, 32, 968);

        float sinA = sin(A), cosA = cos(A),
              sinB = sin(B), cosB = cos(B);
        for(int i=0; i<12; i++) {

            int a = edges[i][0], b = edges[i][1];

            int x0 = dots[a][0], y0 = dots[a][1],
                z0 = dots[a][2], x1 = dots[b][0],
                y1 = dots[b][1], z1 = dots[b][2];

            int dx = x1-x0, dy = y1-y0, dz = z1-z0;
            float len = sqrt(dx*dx + dy*dy + dz*dz);

            float ux = dx / len;
            float uy = dy / len;
            float uz = dz / len;

            float step = 0.02f;
            for (float t = 0; t <= len; t += step) {
                float x = x0 + t * ux;
                float y = y0 + t * uy;
                float z = z0 + t * uz;

                float y1 = y*cosA - z*sinA;
                float z1 = y*sinA + z*cosA;
                float x2 = x*cosB - y1*sinB;
                float y2 = x*sinB + y1*cosB;

                float zbu = 1/(z1+5);
                int scale = 30;
                int xscr = width/2 + scale*x2*zbu;
                int yscr = height/2 - scale*y2*zbu;

                int scrInx = xscr + (yscr*width);

                if(xscr >= 0 && xscr < width &&
                        yscr >= 0 && yscr < height &&
                        zbu > zbuff[scrInx] ) {
                    screen[scrInx] ='"';
                    zbuff[scrInx] = zbu;
                }
            }
        }
        cout << "\x1b[H";
        for (int k = 0; k < width*height ; k++) {
            putchar(k % width ? screen[k] : '\n');
        }

        A += 0.03;
        B += 0.02;
        usleep(30000);
    }

    return 0;
}