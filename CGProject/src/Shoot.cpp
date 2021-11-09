
#include "Shoot.h"
#include "Plane.h"
#include "ShaderLightMapper.h"

//Usprung und Richtung des Schusses
Shoot::Shoot(Vector pos, Vector dir) {
	this->pos = pos;
	this->dir = dir.normalize();
}

Shoot::~Shoot() {
}

//Position wird beim in Richtung vom Directionvektor für jeden Frame addiert
void Shoot::update(float dtime) {

	pos += dir * dtime;
	
}


//Kollisionstest
bool Shoot::checkCollision(Plane& p) {
	//Ort und Groeße der BoundingBox wird ermittelt 
	Vector tmp = p.mod->transform().translation();
	Vector size = p.mod->boundingBox().size();

	//Kollision auf X-,Y- und Z-Ebene wird ueberprueft
	bool collisionX = tmp.X + size.X >= pos.X &&
		pos.X + size.X >= tmp.X;
	
	bool collisionY = tmp.Y + size.Y >= pos.Y &&
		pos.Y + size.Y >= tmp.Y;
	
	bool collisionZ = tmp.Z + size.Z >= pos.Z &&
		pos.Z + size.Z >= tmp.Z;

	//wenn alles true, dann gab es ein Treffer
	return collisionX && collisionY && collisionZ;
	
	
	
}