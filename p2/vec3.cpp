#include "vec3.h"

Vec3::Vec3( float x, float y, float z )
{
    v0 = x;
    v1 = y;
    v2 = z;
}

Vec3&
Vec3::operator=( const Vec3& rhs )
{
    this->v0 = rhs.v0;
    this->v1 = rhs.v1;
    this->v2 = rhs.v2;
    return *this;
}

Vec3
Vec3::operator+( const Vec3& that )
{
    // "this" is a pointer to this class's vec3  (this->name)
    // "that" is a reference to the other vector (that.name)
    Vec3 result;
    result.v0 = ???;
    result.v1 = ???;
    result.v2 = ???;
    return result;
}

Vec3
Vec3::operator-( const Vec3& that )
{
    // "this" is a pointer to this class's vec3  (this->name)
    // "that" is a reference to the other vector (that.name)
    Vec3 result;
    result.v0 = ???;
    result.v1 = ???;
    result.v2 = ???;
    return result;
}

Vec3
Vec3::operator-( )
{
    // "this" is a pointer to this class's vec3  (this->name)
    // there is no "that" here
    Vec3 result;
    result.v0 = ???;
    result.v1 = ???;
    result.v2 = ???;
    return result;
}

Vec3
Vec3::Cross( Vec3& that )
{
    // "this" is a pointer to this class's vec3  (this->name)
    // "that" is a reference to the other vector (that.name)
    Vec3 result;
    result.v0 = ???;
    result.v1 = ???;
    result.v2 = ???;
    return result;
}

float
Vec3::Dot( Vec3& that )
{
    // "this" is a pointer to this class's vec3  (this->name)
    // "that" is a reference to the other vector (that.name)
    float d = ???;
    return d;
}

float
Vec3::Length( )
{
    // "this" is a pointer to this class's vec3  (this->name)
    float len = ???;
    return len;
}

void
Vec3::Print( char *str, FILE *fp )
{
    fprintf( fp, "%s [ %8.3f %8.3f %8.3f ]\n", str, this->v0, this->v1, this->v2 );
}

Vec3
Vec3::Unit( )
{
    // "this" is a pointer to this class's vec3  (this->name)
    Vec3 result;
    result.v0 = ???;
    result.v1 = ???;
    result.v2 = ???;
    return result;
}
