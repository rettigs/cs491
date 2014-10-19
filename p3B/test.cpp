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

    Mat4 T = Mat4();

    T.SetTranslate(2., 3., 4.);
    T.Print ("Translate = ");
    (T * V).Print ("T * V = ");

    T.SetScale(5., -6., 7.5);
    T.Print ("Scale = ");
    (T * V).Print ("T * V = ");

    T.SetRotateX(30.);
    T.Print ("X Rotate = ");
    (T * V).Print ("T * V = ");

    T.SetRotateY(30.);
    T.Print ("Y Rotate = ");
    (T * V).Print ("T * V = ");

    T.SetRotateZ(30.);
    T.Print ("Z Rotate = ");
    (T * V).Print ("T * V = ");

    return 0;
}
