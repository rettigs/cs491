#include "vec3.h"
#include "mat4.h"

    // ** 3A: **

Mat4::Mat4()
{
    SetIdentity();
}

Mat4::Mat4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float n, float o, float p, float q)
{
    m[0][0] = a;
    m[0][1] = b;
    m[0][2] = c;
    m[0][3] = d;
    m[1][0] = e;
    m[1][1] = f;
    m[1][2] = g;
    m[1][3] = h;
    m[2][0] = i;
    m[2][1] = j;
    m[2][2] = k;
    m[2][3] = l;
    m[3][0] = n;
    m[3][1] = o;
    m[3][2] = p;
    m[3][3] = q;
}

Mat4&
Mat4::operator=(const Mat4& that)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            this->m[i][j] = that.m[i][j];
        }
    }
    return *this;
}

Mat4
Mat4::operator*(Mat4& that)
{
    Mat4 c = Mat4();
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            float sum = 0;
            for(int k = 0; k < 4; k++){
                sum += this->m[i][k] * that.m[k][j];
            }
            c.m[i][j] = sum;
        }
    }
    return c;
}

Vec3
Mat4::operator*(Vec3& that)
{
    float c[4];
    float v[] = {that.x, that.y, that.z, 1.};
    for(int i = 0; i < 4; i++){
        float sum = 0;
        for(int k = 0; k < 4; k++){
            sum += this->m[i][k] * v[k];
        }
        c[i] = sum;
    }
    return Vec3(c[0], c[1], c[2]);
}

void
Mat4::Print(char *str, FILE *fp)
{
    fprintf(fp, "%s \n", str);
    fprintf(fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[0][0], this->m[0][1], this->m[0][2], this->m[0][3]);
    fprintf(fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[1][0], this->m[1][1], this->m[1][2], this->m[1][3]);
    fprintf(fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[2][0], this->m[2][1], this->m[2][2], this->m[2][3]);
    fprintf(fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[3][0], this->m[3][1], this->m[3][2], this->m[3][3]);
}

void
Mat4::SetIdentity()
{
    this->m[0][0] = 1;
    this->m[0][1] = 0;
    this->m[0][2] = 0;
    this->m[0][3] = 0;
    this->m[1][0] = 0;
    this->m[1][1] = 1;
    this->m[1][2] = 0;
    this->m[1][3] = 0;
    this->m[2][0] = 0;
    this->m[2][1] = 0;
    this->m[2][2] = 1;
    this->m[2][3] = 0;
    this->m[3][0] = 0;
    this->m[3][1] = 0;
    this->m[3][2] = 0;
    this->m[3][3] = 1;
}

    // ** 3B: **

void
Mat4::SetRotateX(float deg)
{
    // "this" is a pointer to this class's mat4  (this->name)

    //this->m ???
}

void
Mat4::SetRotateY(float deg)
{
    // "this" is a pointer to this class's mat4  (this->name)

    //this->m ???
}

void
Mat4::SetRotateZ(float deg)
{
    // "this" is a pointer to this class's mat4  (this->name)

    //this->m ???
}



void
Mat4::SetScale(float sx, float sy, float sz)
{
    // "this" is a pointer to this class's mat4  (this->name)

    //this->m ???
}

void
Mat4::SetTranslate(float tx, float ty, float tz)
{
    // "this" is a pointer to this class's mat4  (this->name)

    //this->m ???
}
