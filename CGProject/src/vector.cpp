#include "vector.h"
#include <assert.h>
#include <math.h>

#define EPSILON 1e-6

Vector::Vector( float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
	// TODO: add your code
}

Vector::Vector()
{	
	// TODO: add your code
}

float Vector::dot(const Vector& v) const
{	
	// TODO: add your code
	return v.X * X + v.Y * Y + v.Z * Z;
}

Vector Vector::cross(const Vector& v) const
{
	float n_X = Y * v.Z - Z * v.Y;
	float n_Y = Z * v.X - X * v.Z;
	float n_Z = X * v.Y - Y * v.X;
	// TODO: add your code
	return Vector(n_X,n_Y,n_Z);
}



Vector Vector::operator+(const Vector& v) const
{
	float n_X = X + v.X;
	float n_Y = Y + v.Y;
	float n_Z = Z + v.Z;
	// TODO: add your code
	return Vector(n_X,n_Y,n_Z);
}

Vector Vector::operator-(const Vector& v) const
{
	float n_X = X - v.X;
	float n_Y = Y - v.Y;
	float n_Z = Z - v.Z;
	// TODO: add your code
	return Vector(n_X,n_Y,n_Z);
}

Vector Vector::operator*(float c) const
{
	float n_X = X * c;
	float n_Y = Y * c;
	float n_Z = Z * c;
	// TODO: add your code
	return Vector(n_X,n_Y,n_Z);
}

Vector Vector::operator-() const
{
	float n_X = X * -1;
	float n_Y = Y * -1;
	float n_Z = Z * -1;
	// TODO: add your code
	return Vector(n_X,n_Y,n_Z);
}

Vector& Vector::operator+=(const Vector& v)
{	
	X += v.X;
	Y += v.Y;
	Z += v.Z;
	// TODO: add your code
	return *this;
}



Vector& Vector::normalize()
{	
	float l = length();
	X = X * (1 / l);
	Y = Y * (1 / l);
	Z = Z * (1 / l);
	return *this;
}

float Vector::length() const
{	
	float length = sqrt(lengthSquared());
	return length; 
}

float Vector::lengthSquared() const
{
	// TODO: add your code
	return X*X+Y*Y+Z*Z; 
}

Vector Vector::reflection( const Vector& normal) const
{
	float s = 2 * this->dot(normal);
	// TODO: add your code
	return *this - normal * s; 
}

bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
	Vector ab = b - a;
	Vector bc = c - b;
	Vector n = ab.cross(bc);
	Vector tmp = n;
	Vector nor_n = tmp.normalize();

	float ndotd = nor_n.dot(d);
	if (fabs(ndotd) < EPSILON) {
		return false; //Dreieck und Strahl sind parallel
	}
	float dist = nor_n.dot(c);
	s = (dist - nor_n.dot(*this)) / ndotd;
	if (s < 0) {
		return false; //Dreieck ist dahinter
	}
	Vector ray = *this + d * s;

	//inside-outside test
	float area = n.length()/2;
	float area1 = ((a - ray).cross(b - ray)).length()/2;
	float area2 = ((a - ray).cross(c - ray)).length()/2;
	float area3 = ((c - ray).cross(b - ray)).length()/2;

	if (area + EPSILON >= area1 + area2 + area3) {
		return true;
	}


	return false;
}
