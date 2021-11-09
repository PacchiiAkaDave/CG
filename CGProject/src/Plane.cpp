
#include <stdio.h>
#include "Model.h"
#include "stdlib.h"
#include "Plane.h"
#include "Lights.h"
#include "ShaderLightMapper.h"

//Richtung wird angegeben in Himmelsrichtung
// Hoehe des Flugzeugs wird uebergeben
Plane::Plane(Plane::Direction dir, float h) {
	this->dir = dir;
	this->height = h;

	//Flugzeug bekommt ein Pointlight
	this->pl = new PointLight();
	pl->color(Color(1.0f, 1.0f, 1.0f));
	pl->attenuation(Vector(0.05f, 0, 0.05f));
	ShaderLightMapper::instance().addLight(pl);
}

Plane::~Plane() {
	delete mod;
	delete pl;
}

//Model wird in einer seperaten Methode geladen
bool Plane::loadModel(const char* File) {
	if (File == nullptr) {
		return false;
	}
	mod = new Model(File,true);
	mod->shader(pShader);
	return true;
}

//Generierung der Anfangsposition des Fliegers
void Plane::generatePlane() {
	//Ermittlung der Position auf x- oder z-Ebene zufaellig
	float randpos = rand() % 200 - 100;
	//Ermittlung der Geschwindigkeit zufaellig
	speed = rand() % 3 + 1;
	Matrix TM, RM, TRM;
	if (dir == SOUTH) {
		// South z - 100
		TM.translation(randpos, height, -100);
		matrixscale(TM);
		mod->transform(TM);
		mod->boundingBox().transform(TM);
		//Position des Pointlights mituebergeben
		TM.m31 -= 1;
		pl->position(TM.translation());
	}
	else {
		//Damit Flugzeug nicht seitlich fliegt, muss dies 
		//abhaengig von Himmelsrichtung um Y-Achse rotiert werden
		if (dir == NORTH) {
			//North z + 100
			TM.translation(randpos, height, 100);
			RM.rotationY(3.14);
		}
		if (dir == WEST) {
			//West x + 100
			TM.translation(100, height, randpos);
			RM.rotationY(1.5f * 3.14);
		}
		if (dir == EAST) {
			// East x - 100
			TM.translation(-100, height, randpos);
			RM.rotationY(0.5f * 3.14);
		}
		
		matrixscale(TM);
		TRM = TM * RM;
		mod->transform(TRM);
		mod->boundingBox().transform(TRM);
		TRM.m31 -= 1;
		pl->position(TRM.translation());
	}
}

//Planeobjekt wird gescaled
void Plane::matrixscale(Matrix& m) {
	m.m00 = 5, m.m11 = 5, m.m22 = 5;
}


//Bewegung abhaengig von der Himmelsrichtung
void Plane::update(float dtime) {
	Matrix TM, RM, TRM;
	Vector pos = mod->transform().translation();
	if (dir == SOUTH) {
		//Bewegung in positiver Z-Richtung
		TM.translation(pos.X, height, pos.Z + (dtime*speed));
		matrixscale(TM);
		mod->transform(TM);
		mod->boundingBox().transform(TM);
		//Pointlight muss mitbewegt werden
		TM.m31 -= 1;
		pl->position(TM.translation());
		//Wenn Flugzeug bestimmte Grenze erreicht, wird dieser an Anfangsposition neu generiert
		if (pos.X <= -101 || pos.X >= 101 || pos.Z <= -101 || pos.Z >= 101) {
			generatePlane();
		}
	}
	else {
		if (dir == NORTH) {
			//Bewegung in negativer Z-Richtung
			TM.translation(pos.X, height, pos.Z - (dtime * speed));
			RM.rotationY(3.14);
		}
		if (dir == WEST) {
			//Bewegung in negativer X-Richtung
			TM.translation(pos.X - (dtime * speed), height, pos.Z);
			RM.rotationY(1.5f * 3.14);
		}
		if (dir == EAST) {
			//Bewegung in positiver X-Richtung
			TM.translation(pos.X + (dtime * speed), height, pos.Z);
			RM.rotationY(0.5f * 3.14);
		}
		
		matrixscale(TM);
		TRM = TM * RM;
		mod->transform(TRM);
		mod->boundingBox().transform(TRM);
		TRM.m31 -= 1;
		pl->position(TRM.translation());
		if (pos.X <= -101 || pos.X >= 101 || pos.Z <= -101 || pos.Z >= 101) {
			generatePlane();
		}
	}
}

void Plane::draw(const BaseCamera & Cam){
		mod->draw(Cam);
}