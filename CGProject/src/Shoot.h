#pragma once

#include "Aabb.h"
#include "Lights.h"
#include "Plane.h"

class Shoot {

public:
	Shoot(Vector pos, Vector dir);
	~Shoot();
	bool checkCollision(Plane& p);
	void update(float dtime);
	Vector getPos() { return pos; }
protected:
	Vector pos;
	Vector dir;
};