#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>

#ifndef VEC3_H
#define VEC3_H

class Vec3{
    protected:
        float x, y, z;

    public:
        Vec3(float=0, float=0, float=0);
        Vec3&   operator=(const Vec3&);
        Vec3    operator+(const Vec3&);
        Vec3    operator-(const Vec3&);
        Vec3    operator-();
        Vec3    Cross(Vec3&);
        float   Dot(Vec3&);
        float   Length();
        void    Print(char * = "", FILE * = stderr);
        Vec3    Unit();

    friend class Mat4;
};


inline float SQR(float f){
    return f * f;
}

#endif
