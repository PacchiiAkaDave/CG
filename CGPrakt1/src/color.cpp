#include "color.h"
#include <assert.h>

Color::Color()
{
	R = 0;
	G = 0;
	B = 0;
  // TODO: add your code
}

Color::Color( float r, float g, float b)
{
	R = r;
	G = g;
	B = b;
	// TODO: add your code
}

Color Color::operator*(const Color& c) const
{
	float n_R = R * c.R;
	float n_G = G * c.G;
	float n_B = B * c.B;
	// TODO: add your code
    return Color(n_R,n_G,n_B);
}

Color Color::operator*(const float Factor) const
{
	float n_R = R * Factor;
	float n_G = G * Factor;
	float n_B = B * Factor;
	// TODO: add your code
	return Color(n_R,n_G,n_B);
}

Color Color::operator+(const Color& c) const
{
	float n_R = R + c.R;
	float n_G = G + c.G;
	float n_B = B + c.B;
	// TODO: add your code
	return Color(n_R,n_G,n_B); // dummy (remove)
}

Color& Color::operator+=(const Color& c)
{
	R += c.R;
	G += c.G;
	B += c.B;
	// TODO: add your code
	return *this; // dummy (remove)
}