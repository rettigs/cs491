void
Forward(float th1, float th2, float th3, float l1, float l2, Mat4& m1g, Mat4& m2g, Mat4& m3g)
{
    m1g.SetIdentity();
    m2g.SetIdentity();
    m3g.SetIdentity();

    Mat4 r1 = Mat4();
    r1.SetRotateZ(th1);
    Mat4 r2 = Mat4();
    r2.SetRotateZ(th2);
    Mat4 r3 = Mat4();
    r3.SetRotateZ(th3);
    Mat4 t2 = Mat4();
    t2.SetTranslate(l1, 0., 0.);
    Mat4 t3 = Mat4();
    t3.SetTranslate(l2, 0., 0.);

    m1g = r1 * m1g;
    m2g = r1 * t2 * r2 * m2g;
    m3g = r1 * t2 * r2 * t3 * r3 * m3g;
}
