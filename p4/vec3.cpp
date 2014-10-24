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
        result.v0 = this->v0 + that.v0;
        result.v1 = this->v1 + that.v1;
        result.v2 = this->v2 + that.v2;
	return result;
}

Vec3
Vec3::operator-( const Vec3& that )
{
	// "this" is a pointer to this class's vec3  (this->name)
	// "that" is a reference to the other vector (that.name)
	Vec3 result;
        result.v0 = this->v0 - that.v0;
        result.v1 = this->v1 - that.v1;
        result.v2 = this->v2 - that.v2;
	return result;
}

Vec3
Vec3::operator-( )
{
	// "this" is a pointer to this class's vec3  (this->name)
	// there is no "that" here
	Vec3 result;
        result.v0 = -this->v0;
        result.v1 = -this->v1;
        result.v2 = -this->v2;
	return result;
}

Vec3
Vec3::Cross( Vec3& that )
{
	// "this" is a pointer to this class's vec3  (this->name)
	// "that" is a reference to the other vector (that.name)
	Vec3 result;
        result.v0 = this->v1*that.v2 - that.v1*this->v2;
        result.v1 = this->v2*that.v0 - that.v2*this->v0;
        result.v2 = this->v0*that.v1 - that.v0*this->v1;
	return result;
}

float
Vec3::Dot( Vec3& that )
{
	// "this" is a pointer to this class's vec3  (this->name)
	// "that" is a reference to the other vector (that.name)
	float d = this->v0*that.v0 + this->v1*that.v1 + this->v2*that.v2;
	return d;
}

void
Vec3::Export( float* out )
{
	out[0] = v0;
	out[1] = v1;
	out[2] = v2;
}

float
Vec3::Length( )
{
	// "this" is a pointer to this class's vec3  (this->name)
	float len = sqrt( SQR(this->v0) + SQR(this->v1) + SQR(this->v2) );
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
	float len = this->Length( );
	Vec3 result;
	result.v0 = this->v0 / len;;
	result.v1 = this->v1 / len;;
	result.v2 = this->v2 / len;;
	return result;
}