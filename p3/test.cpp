#include "mat4.h"

int
main(int argc, char *argv[ ])
{
    Mat4().Print("I = ");

    Mat4 M1(1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.);
    M1.Print("M1 = ");

    Mat4 M2 = M1;
    M2.Print("M2 = ");

    (M1 * M2).Print("M1 * M2 = ");

    Vec3 V(1., 2., 3.);
    V.Print("V =");
    (M1 * V).Print("M1 * V =");

    return 0;
}
