#include "vec3.h"

int
main( int argc, char * argv[ ] )
{
    Vec3 b( 1., 2., 3. );
    Vec3 c( 5., 6., 7. );

    Vec3 a = c;
    Vec3 au = a.Unit( );
    a.Print( "a =" );
    au.Print( "au =" );
    b.Print( "b =" );
    c.Print( "c =" );

    a = Vec3( 2., -5., 8. );
    a.Print( "a =" );

    Vec3 ma = -a;
    ma.Print( "-a = " );

    Vec3 e = a + b;
    e.Print( "a+b =" );

    e = a - b;
    e.Print( "a-b =" );


    float f = ( a + b ).Length( );
    fprintf( stderr, "f = %8.3f\n", f );

    Vec3 g = a.Cross(b);
    g.Print( "a x b =" );

    . . .

    return 0;
}
