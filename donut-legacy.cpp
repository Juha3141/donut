#include <iostream>
#include <cmath>
#include <cstring>
#include <unistd.h>

#define STEP 0.1
#define FOV_ANGLE 100

using namespace std;

typedef struct {
    double x,y,z;
}coord , vector;

coord rotate_by_x(coord c , double A) { return {c.x , c.y*cos(A)-c.z*sin(A) , c.y*sin(A)+c.z*cos(A)}; }
coord rotate_by_y(coord c , double A) { return {c.x*cos(A)-c.z*sin(A) , c.y , c.x*sin(A)+c.z*cos(A)}; }
coord rotate_by_z(coord c , double A) { return {c.x*cos(A)-c.y*sin(A) , c.x*sin(A)+c.y*cos(A) , c.z}; }

double transform_3d(double x_y , double z) {
    return x_y/(z*tan((FOV_ANGLE*(M_PI/180)/2)));
}

double dot_product(vector v1 , vector v2) {
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

const int row = 50;
const int col = 80;

int main(void) {
    // constants
    double r1 = 2 , r2 = 4;
    double spin_angle_x = 0 , spin_angle_y = 0;
    double ox = 0 , oy = 0 , oz = 10;
    char output[row][col] = {' ' , };
    char buffer[row][col] = {0 , };

    double wx0 = 0.0001;
    double ax = 0.1;
    double wy0 = 0.0001;
    double ay = 0.1;
    
    double t = 0;
    while(1) {
        for(int j = 0; j < row; j++) {
            for(int i = 0; i < col; i++) {
                output[j][i] = ' ';
            }
        }
        // spin_angle_x += (wx0+ax*t);
        // spin_angle_y += (wy0+ay*t);
        printf("\x1b[H");
        spin_angle_y += 0.001;
        spin_angle_x -= 0.001;
        t += 0.0000001;
        for(double angle1 = 0; angle1 < 2*M_PI; angle1 += STEP) {    
            coord base_circle = {r2+r1*cos(angle1) , 0 , r1*sin(angle1)};
            double z = r1*sin(angle1);
            for(double angle2 = 0; angle2 < 2*M_PI; angle2 += STEP) {
                coord donut; // completely good donut
                double xaxis = spin_angle_x;
                double yaxis = 0;

                donut = rotate_by_z(base_circle , angle2);
                donut = rotate_by_x(donut , xaxis);
                donut = rotate_by_y(donut , yaxis);
                donut = {donut.x+ox , donut.y+oy , donut.z+oz};

                double scrx = transform_3d(donut.x , donut.z);
                double scry = transform_3d(donut.y , donut.z);
                
                int r = (int)(scry*(row/2)+(row/2));
                int c = (int)(scrx*(col/2)+(col/2));

                coord normal = {cos(angle1) , sin(angle1) , 0};
                normal = rotate_by_z(normal , angle2);
                normal = rotate_by_x(normal , xaxis);
                normal = rotate_by_y(normal , yaxis);
                double L = dot_product(normal , {0,0,1});
                
                if(r >= 0 && c >= 0 && r < row && c < col) {
                    if(1/donut.z > buffer[r][c]) {
                        buffer[r][c] = 1/donut.z;
                        if(L > 0) output[r][c] = "`.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD"[(int)(L*40)];
                        else output[r][c] = '.';
                        // printf("(%0.5f , %0.5f , %0.5f)\n" , donut.x , donut.y , donut.z);
                    }
                }
            }
        }
        for(int j = 0; j < row; j++) {
            for(int i = 0; i < col; i++) {
                printf("%c" , output[j][i]);
            }
            printf("\n");
        }
    }
}