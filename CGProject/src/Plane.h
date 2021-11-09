#pragma once

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include "Model.h"
#include "Lights.h"

class Plane : public BaseModel
{
public:
	enum Direction
	{
		NORTH = 0,
		WEST, SOUTH,
		EAST
	};
	Plane(Direction dir, float h);
	virtual ~Plane();
	bool loadModel(const char* File);
	void generatePlane();
	void update(float dtime);
	void draw(const BaseCamera& Cam);
	float getHeight() { return height; }
	Model* mod;
	
private:
	
	float height;
	Direction dir;
	void matrixscale(Matrix& m);
	int speed = 1;
	PointLight* pl;
};

#endif /* Plane_hpp */