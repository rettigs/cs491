#include "mat4.h"
#include "forward.cpp"

int
main(int argc, char *argv[])
{
    float a1 = 5.;
    float a2 = 10.;
    float a3 = 15.;

    float l1 = 5.;
    float l2 = 10.;
    float l3 = 15.;

    Mat4 m1, m2, m3;
    Forward(a1, a2, a3, l1, l2, m1, m2, m3);
}
