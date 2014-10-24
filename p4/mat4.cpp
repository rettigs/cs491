const float D2R = (float)M_PI / 180.f;

Mat4::Mat4( )
{
	SetIdentity( );
}


Mat4::Mat4( float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p )
{
        this->m[0][0] = a;      this->m[0][1] = b;      this->m[0][2] = c;      this->m[0][3] = d;
        this->m[1][0] = e;      this->m[1][1] = f;      this->m[1][2] = g;      this->m[1][3] = h;
        this->m[2][0] = i;      this->m[2][1] = j;      this->m[2][2] = k;      this->m[2][3] = l;
        this->m[3][0] = m;      this->m[3][1] = n;      this->m[3][2] = o;      this->m[3][3] = p;
}

Mat4&
Mat4::operator=( const Mat4& that )
{
	// "this" is a pointer to this class's mat4  (this->name)
	// "that" is a reference to the other matrix (that.name)

        this->m[0][0] = that.m[0][0]; this->m[0][1] = that.m[0][1]; this->m[0][2] = that.m[0][2]; this->m[0][3] = that.m[0][3];
        this->m[1][0] = that.m[1][0]; this->m[1][1] = that.m[1][1]; this->m[1][2] = that.m[1][2]; this->m[1][3] = that.m[1][3];
        this->m[2][0] = that.m[2][0]; this->m[2][1] = that.m[2][1]; this->m[2][2] = that.m[2][2]; this->m[2][3] = that.m[2][3];
        this->m[3][0] = that.m[3][0]; this->m[3][1] = that.m[3][1]; this->m[3][2] = that.m[3][2]; this->m[3][3] = that.m[3][3];

	return *this;
}

Mat4
Mat4::operator*( Mat4& that )
{
	// "this" is a pointer to this class's mat4  (this->name)
	// "that" is a reference to the other matrix (that.name)

	Mat4 result;
        result.m[0][0] = this->m[0][0] * that.m[0][0]  +  this->m[0][1] * that.m[1][0]  +  this->m[0][2] * that.m[2][0]  +  this->m[0][3] * that.m[3][0];
        result.m[0][1] = this->m[0][0] * that.m[0][1]  +  this->m[0][1] * that.m[1][1]  +  this->m[0][2] * that.m[2][1]  +  this->m[0][3] * that.m[3][1];
        result.m[0][2] = this->m[0][0] * that.m[0][2]  +  this->m[0][1] * that.m[1][2]  +  this->m[0][2] * that.m[2][2]  +  this->m[0][3] * that.m[3][2];
        result.m[0][3] = this->m[0][0] * that.m[0][3]  +  this->m[0][1] * that.m[1][3]  +  this->m[0][2] * that.m[2][3]  +  this->m[0][3] * that.m[3][3];

        result.m[1][0] = this->m[1][0] * that.m[0][0]  +  this->m[1][1] * that.m[1][0]  +  this->m[1][2] * that.m[2][0]  +  this->m[1][3] * that.m[3][0];
        result.m[1][1] = this->m[1][0] * that.m[0][1]  +  this->m[1][1] * that.m[1][1]  +  this->m[1][2] * that.m[2][1]  +  this->m[1][3] * that.m[3][1];
        result.m[1][2] = this->m[1][0] * that.m[0][2]  +  this->m[1][1] * that.m[1][2]  +  this->m[1][2] * that.m[2][2]  +  this->m[1][3] * that.m[3][2];
        result.m[1][3] = this->m[1][0] * that.m[0][3]  +  this->m[1][1] * that.m[1][3]  +  this->m[1][2] * that.m[2][3]  +  this->m[1][3] * that.m[3][3];

        result.m[2][0] = this->m[2][0] * that.m[0][0]  +  this->m[2][1] * that.m[1][0]  +  this->m[2][2] * that.m[2][0]  +  this->m[2][3] * that.m[3][0];
        result.m[2][1] = this->m[2][0] * that.m[0][1]  +  this->m[2][1] * that.m[1][1]  +  this->m[2][2] * that.m[2][1]  +  this->m[2][3] * that.m[3][1];
        result.m[2][2] = this->m[2][0] * that.m[0][2]  +  this->m[2][1] * that.m[1][2]  +  this->m[2][2] * that.m[2][2]  +  this->m[2][3] * that.m[3][2];
        result.m[2][3] = this->m[2][0] * that.m[0][3]  +  this->m[2][1] * that.m[1][3]  +  this->m[2][2] * that.m[2][3]  +  this->m[2][3] * that.m[3][3];

        result.m[3][0] = this->m[3][0] * that.m[0][0]  +  this->m[3][1] * that.m[1][0]  +  this->m[3][2] * that.m[2][0]  +  this->m[3][3] * that.m[3][0];
        result.m[3][1] = this->m[3][0] * that.m[0][1]  +  this->m[3][1] * that.m[1][1]  +  this->m[3][2] * that.m[2][1]  +  this->m[3][3] * that.m[3][1];
        result.m[3][2] = this->m[3][0] * that.m[0][2]  +  this->m[3][1] * that.m[1][2]  +  this->m[3][2] * that.m[2][2]  +  this->m[3][3] * that.m[3][2];
        result.m[3][3] = this->m[3][0] * that.m[0][3]  +  this->m[3][1] * that.m[1][3]  +  this->m[3][2] * that.m[2][3]  +  this->m[3][3] * that.m[3][3];

	return result;
}

Vec3
Mat4::operator*( Vec3& that )
{
	// "this" is a pointer to this class's mat4  (this->name)
	// "that" is a reference to the vector (that.name)

	Vec3 result;
        result.v0 = this->m[0][0]*that.v0 + this->m[0][1]*that.v1 + this->m[0][2]*that.v2 + this->m[0][3];
        result.v1 = this->m[1][0]*that.v0 + this->m[1][1]*that.v1 + this->m[1][2]*that.v2 + this->m[1][3];
        result.v2 = this->m[2][0]*that.v0 + this->m[2][1]*that.v1 + this->m[2][2]*that.v2 + this->m[2][3];
	return result;
}

void
Mat4::Export( float out[4][4] )
{
	for( int i = 0; i < 4; i++ )
		for( int j = 0; j < 4; j++ )
			out[i][j] = m[i][j];
}

void
Mat4::Print( char *str, FILE *fp )
{
	// "this" is a pointer to this class's mat4  (this->name)

	fprintf( fp, "%s \n", str );
	fprintf( fp, "\t%8.3f,  %8.3f,  %8.3f,  %8.3f,\n", this->m[0][0], this->m[0][1], this->m[0][2], this->m[0][3] );
	fprintf( fp, "\t%8.3f,  %8.3f,  %8.3f,  %8.3f,\n", this->m[1][0], this->m[1][1], this->m[1][2], this->m[1][3] );
	fprintf( fp, "\t%8.3f,  %8.3f,  %8.3f,  %8.3f,\n", this->m[2][0], this->m[2][1], this->m[2][2], this->m[2][3] );
	fprintf( fp, "\t%8.3f,  %8.3f,  %8.3f,  %8.3f\n", this->m[3][0], this->m[3][1], this->m[3][2], this->m[3][3] );
}

void
Mat4::SetIdentity( )
{
	// "this" is a pointer to this class's mat4  (this->name)

        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.;
        m[0][1] = m[0][2] = m[0][3] = 0.;
        m[1][0] = m[1][2] = m[1][3] = 0.;
        m[2][0] = m[2][1] = m[2][3] = 0.;
        m[3][0] = m[3][1] = m[3][2] = 0.;
}

void
Mat4::SetRotateX( float deg )
{
	// "this" is a pointer to this class's mat4  (this->name)

	SetIdentity( );
	float rad = D2R * deg;
	m[1][1] = m[2][2] = cos(rad);
	m[1][2] = -sin(rad);
	m[2][1] =  sin(rad);
}

void
Mat4::SetRotateY( float deg )
{
	// "this" is a pointer to this class's mat4  (this->name)

	SetIdentity( );
	float rad = D2R * deg;
	m[0][0] = m[2][2] = cos(rad);
	m[0][2] =  sin(rad);
	m[2][0] = -sin(rad);
}

void
Mat4::SetRotateZ( float deg )
{
	// "this" is a pointer to this class's mat4  (this->name)

	SetIdentity( );
	float rad = D2R * deg;
	m[0][0] = m[1][1] = cos(rad);
	m[0][1] = -sin(rad);
	m[1][0] =  sin(rad);
}



void
Mat4::SetScale( float sx, float sy, float sz )
{
	// "this" is a pointer to this class's mat4  (this->name)

	SetIdentity( );
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

void
Mat4::SetTranslate( float tx, float ty, float tz )
{
	// "this" is a pointer to this class's mat4  (this->name)

	SetIdentity( );
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
}


void
Mat4::SetTranspose( )
{
	// "this" is a pointer to this class's mat4  (this->name)

	Mat4 tmp = *this;
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			this->m[i][j] = tmp.m[j][i];
		}
	}
}

