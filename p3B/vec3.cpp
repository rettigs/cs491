#include "vec3.h"

Vec3::Vec3(float x, float y, float z):
    x(x), y(y), z(z)
{}

Vec3& Vec3::operator=(const Vec3& rhs){
    Vec3 u = *this;
    u.x = rhs.x;
    u.y = rhs.y;
    u.z = rhs.z;
    return *this;
}

void Vec3::Print(char *str, FILE *fp){
    Vec3 u = *this;
    fprintf(fp, "%s [ %8.3f %8.3f %8.3f ]\n", str, u.x, u.y, u.z);
}

Vec3 Vec3::operator+(const Vec3& v){
    Vec3 u = *this;
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v){
    Vec3 u = *this;
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

Vec3 Vec3::operator-(){
    Vec3 u = *this;
    return Vec3(-u.x, -u.y, -u.z);
}

Vec3 Vec3::Unit(){
    Vec3 u = *this;
    float len = u.Length();
    return Vec3(u.x / len, u.y / len, u.z / len);
}

Vec3 Vec3::Cross(Vec3& v){
    Vec3 u = *this;
    return Vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

float Vec3::Dot(Vec3& v){
    Vec3 u = *this;
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

float Vec3::Length(){
    Vec3 u = *this;
    return sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2));
}
