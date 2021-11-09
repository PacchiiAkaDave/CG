//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "ShaderLightmapper.h"
#include <vector>
#include "Shoot.h"


#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif



Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048),xpos(0),ypos(0)
{
	player = new BaseModel();
	Dir = Vector(0, 3, 0);
	createScene();
	


}
void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Methode zur Richtung des Schusses zum Mauszeiger
Vector Application::calc3DRay(float x, float y, Vector& Pos)
{
	Matrix projM = Cam.getProjectionMatrix();
	projM = projM.invert();
	Vector ray = projM * Vector(x, y, 0);
	Matrix viewM = Cam.getViewMatrix();
	Vector ray3D = viewM.invert().transformVec3x3(ray);
	Pos = Cam.position();
	// Pos:Ray Origin
	// return:Ray Direction
	return ray3D.normalize();
}

//Hilfsmethode zur Steuerung
void Application::getInputMovement(float& fw, float& rotY, float dtime) {
	//Vorwaerts
	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
		fw += 7.0f;
	}
	//Rueckwaerts
	else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
		fw -= 7.0f;
	}
	//nach links drehen
	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) {
		rotY += 1.0f;
	}
	//nach rechts drehen
	else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
		rotY -= 1.0f;
	}
	
	//Im Folgendem Bearbeitung des "Rotationsvektors"

	//Nach unten schauen (bis zu einer Grenze)
	if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS) {
		if (Dir.Y > 2.5f) {
			Dir.Y -= dtime;
		}
	}
	//Nach oben schauen (bis zu einer Grenze)
	else if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS) {
		if (Dir.Y < 5.0f) {
			Dir.Y += dtime;
		}
	}
}

void Application::update(float dtime)
{
	//Ermittlung der Cursorposition im Fenster
	glfwGetCursorPos(pWindow, &xpos, &ypos);
	
	int width = 0;
	int height = 0;
	//Ermittlung der Fenstergroeße
	glfwGetWindowSize(pWindow, &width, &height);
	width *= 0.5;
	height *= 0.5;
	xpos = (xpos - width) / width;
	ypos = (-(ypos - height) / height); //normalisierte Cursorpos
	//Startpunkt des Schusses
	Vector pos;
	//Richtung des Schusses
	Vector dir = calc3DRay(xpos, ypos, pos);
	
	//Matrix, die Ort des Schusses beschreibt
	Matrix bulletpos;
	//Beim Nichtdruecken der linken Maustaste,
	//werden Position(entspricht Kameraposition) und Schussrichtung neu generiert
	if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		
		shoot = Shoot(pos, dir);
	}
	
	
	//Beim Druecken der linken Maustaste wird Schuss abgegeben mit den oben ermittelten Werten
	//Flugbahn wird abgebrochen, wenn Taste losgelassen wird
	if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		shoot.update(dtime * 300); //Parameter mit 300 multipliziert um Schuss zu verschnellern
		for (std::vector<Plane*>::iterator it = planes.begin(); it != planes.end(); ++it) {
			
			pPlane = *it;
			//Bei einem Treffer verschwindet das Flugzeug und Position wird neu generiert
			if (shoot.checkCollision(*pPlane) == true) {
				pPlane->generatePlane();
				
			}
		}
	}
	//Ortsangabe für die Matrix mit Hilfsmethode der Shoot-Klasse
	bulletpos.translation(shoot.getPos());
	//Schussmodel bekommt die bulletpos-Matrix um Bewegung zu modellieren
	bullet->transform(bulletpos);
	for (std::vector<Plane*>::iterator it = planes.begin(); it != planes.end(); ++it) {
		pPlane = *it;
		//Bewegung jedes einzelnen Plane-objekts
		pPlane->update(dtime*10);
	}
	

	fw = 0.0f;
	rotY = 0.0f;
	//Steuerungsannahme
	getInputMovement(fw, rotY , dtime);

	//Bewegung nach vorde und nach hinten mit einer Translationmatrix
	TM.translation(0, 0, fw * dtime);
	//Rotation um y-Achse um nach links und rechts zu drehen
	RyM.rotationY(rotY * dtime);
	//Erst rotiert, dann nach vorne und nach hinten
	player->transform(player->transform()  * TM * RyM);
	player->transform(player->transform());

	Matrix tmp = player->transform();
	
	//1.Parameter Punkt der von Kamera "anvisiert" wird
	//2.Parameter Up-Vektor -> orthogonal zur Ebene , wo sich die Kamera befindet
	//3.Parameter Position der Kamera (manuell um 3 Einheiten in y-Richtung verschoben
	perspective.lookAt(tmp.translation()+Dir,tmp.up(),Vector(0,3,0)+tmp.translation() - tmp.forward() + tmp.up() * 0.5f);

	//Aenderung der Viewmatrix der Kamera mit der lookAt-Matrix 
	Cam.setViewMatrix(perspective);

	//Position muss manuell gesetzt, werden da wir auch manuell die ViewMatrix aendern
	//Wichtig fuer Ursprung des Schusses 
	Cam.setPosition(Vector(0, 3, 0) + tmp.translation() - tmp.forward() + tmp.up() * 0.5f);
	
}

void Application::draw()
{
	ShadowGenerator.generate(Models);
	
    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
    // 2. setup shaders and draw models
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
	ShaderLightMapper::instance().deactivate();
	
    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
	delete player;
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;
    
    Models.clear();

	planes.clear();
}

void Application::createScene()
{
	//Matrix m,n,tmp;
	
	
	
	PhongShader* pPhongShader = new PhongShader();
	

	//Erstellung unserer Skybox
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	pModel->shader(new PhongShader(), true);
	pModel->shadowCaster(false);
	Models.push_back(pModel);

	//Patronenmodel
	bullet = new Model(ASSET_DIRECTORY "bullet/bullet.obj", false);
	bullet->shader(new PhongShader(), true);
	bullet->shadowCaster(false);
	Models.push_back(bullet);
	
	//Hauptlichtquelle (kommt von Sonne der Skybox)
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(-1, -0.6, 1));
	dl->color(Color(1, 1, 1));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	

	//Erstellung der Planemodelle
	pPlane = new Plane(Plane::EAST, 30);
	pPlane->shader(pPhongShader, true);
	pPlane->loadModel(ASSET_DIRECTORY "plane/Plane.obj");
	Models.push_back(pPlane);
	planes.push_back(pPlane);
	

	pPlane = new Plane(Plane::SOUTH, 40);
	pPlane->shader(pPhongShader, true);
	pPlane->loadModel(ASSET_DIRECTORY "plane/Plane.obj");
	Models.push_back(pPlane);
	planes.push_back(pPlane);

	pPlane = new Plane(Plane::NORTH, 50);
	pPlane->shader(pPhongShader, true);
	pPlane->loadModel(ASSET_DIRECTORY "plane/Plane.obj");
	Models.push_back(pPlane);
	planes.push_back(pPlane);

	pPlane = new Plane(Plane::WEST, 60);
	pPlane->shader(pPhongShader, true);
	pPlane->loadModel(ASSET_DIRECTORY "plane/Plane.obj");
	Models.push_back(pPlane);
	planes.push_back(pPlane);

	pPlane = new Plane(Plane::EAST, 70);
	pPlane->shader(pPhongShader, true);
	pPlane->loadModel(ASSET_DIRECTORY "plane/Plane.obj");
	Models.push_back(pPlane);
	planes.push_back(pPlane);

	

	pPlane = new Plane(Plane::NORTH, 80);
	pPlane->shader(pPhongShader, true);
	pPlane->loadModel(ASSET_DIRECTORY "plane/Plane.obj");
	Models.push_back(pPlane);
	planes.push_back(pPlane);

	pPlane = new Plane(Plane::WEST, 90);
	pPlane->shader(pPhongShader, true);
	pPlane->loadModel(ASSET_DIRECTORY "plane/Plane.obj");
	Models.push_back(pPlane);
	planes.push_back(pPlane);

	pPlane = new Plane(Plane::SOUTH, 100);
	pPlane->shader(pPhongShader, true);
	pPlane->loadModel(ASSET_DIRECTORY "plane/Plane.obj");
	Models.push_back(pPlane);
	planes.push_back(pPlane);

	
	//Position, jedes einzelnen Planes generiert
	for (std::vector<Plane*>::iterator it = planes.begin(); it != planes.end(); ++it) {
		pPlane = *it;
		pPlane->generatePlane();
	}


	//Boden
	pModel = new TrianglePlaneModel(200, 200, 1, 1);
	pPhongShader = new PhongShader();
	pPhongShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "sand2.jpg"));
	pModel->shader(pPhongShader, true);
	pModel->shadowCaster(true);
	Models.push_back(pModel);

	
	

	
	
}

