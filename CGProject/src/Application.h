//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "ShadowMapGenerator.h"
#include <vector>
#include "Plane.h"
#include "Player.h"
#include "Shoot.h"

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();
protected:
	void createScene();
    void getInputMovement(float& fw, float& rotY, float rotX);
    Vector calc3DRay(float x, float y, Vector& Pos);
    Camera Cam;
    ModelList Models;
    GLFWwindow* pWindow;
	BaseModel* pModel;
	ShadowMapGenerator ShadowGenerator;
    std::vector<Plane*> planes;
    Plane* pPlane;
    double xpos, ypos;
    Matrix perspective, RTM, TM , RyM;
    Vector Dir;
    BaseModel* player;
    float fw;
    float rotY;
    Shoot shoot = Shoot(Vector(0,0,0),Vector(0,0,0));
    Model* bullet;
    
    
};

#endif /* Application_hpp */
