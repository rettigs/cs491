#include "vec3.cpp"
#include "mat4.cpp"

void
Forward( float th1, float th2, float th3,  float l1, float l2,    Mat4& m1g, Mat4& m2g, Mat4& m3g )
{
// set the matrices to identity so at least you will have something sane to look at... :-)

	m1g.SetIdentity( );
	m2g.SetIdentity( );
	m3g.SetIdentity( );

	// ?????

	m1g.Print( "m1g =", FP );
	m2g.Print( "m2g =", FP );
	m3g.Print( "m3g =", FP );
}
