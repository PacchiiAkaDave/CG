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
#include "scene.h"


#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), xpos(0), ypos(0)
{
    BaseModel* pModel;
    ConstantShader* pConstShader;
    PhongShader* pPhongShader;
    
    // create LineGrid model with constant color shader
    pModel = new LinePlaneModel(10, 10, 10, 10);
    pConstShader = new ConstantShader();
	pConstShader->color( Color(1,1,1));
    pModel->shader(pConstShader, true);
    Models.push_back( pModel );

    // Exercise 1
    // TODO Load pTankTop & pTankBot
    /*pTankBot = new Model(ASSET_DIRECTORY "tank_bottom.dae");
    pPhongShader = new PhongShader();
    pTankBot->shader(pPhongShader, true);
    Models.push_back(pTankBot);
    pTankTop = new Model(ASSET_DIRECTORY "tank_top.dae");
    pPhongShader = new PhongShader();
    pTankTop->shader(pPhongShader, true); 
    Models.push_back(pTankTop);*/

    // Exercise 2
    
    pPhongShader = new PhongShader();
    pTank = new Tank();
    pTank->shader(pPhongShader, true);
    pTank->loadModels(ASSET_DIRECTORY "tank_bottom.dae", ASSET_DIRECTORY "tank_top.dae");
    Models.push_back( pTank );
    
    
    // Exercise 3
    /*
    Scene* pScene = new Scene();
    pScene->shader(new PhongShader(), true);
    pScene->addSceneFile(ASSET_DIRECTORY "scene.osh");
    Models.push_back(pScene);
    */
    

    
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


void Application::update(float frame_t)
{
    // Exercise 1
    
    /*Matrix TM;
    TM.translation(0, 0, 4);
    Matrix RM;
    RM.rotationY(0.5f * 3.14f * frame_t);
    Matrix RTM;
    RTM = RM * TM;
    pTankBot->transform(RTM);
    pTankTop->transform(RTM * RM.invert());*/
    
    
    // Exercise 2
    

    
    glfwGetCursorPos(pWindow, &xpos, &ypos);

    int width = 0;
    int height = 0;
    glfwGetWindowSize(pWindow, &width, &height);
    width *= 0.5;
    height *= 0.5;
    xpos = (xpos - width) / width;
    ypos = (-(ypos - height) / height); //normalisierte Cursorpos
    Vector pos;
    Vector dir = calc3DRay(xpos, ypos, pos);
    float s = 0;
    pos.triangleIntersection(dir, Vector(1, 0, 0), Vector(0, 0, 1), Vector(1, 0, 1), s);
    Vector dest = pos + dir * s;
    if (s < 0) {
        dest = Vector(0, 0, 0);
    }
    //std::cout << dest.X << ":" << dest.Y << ":" << dest.Z << std::endl;
    if(dest.X == 0 && dest.Z == 0){
    }
    else {
        pTank->aim(dest);
    }
    float fb = 0;
    float lr = 0;
    if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS) {
        fb += 1.0f;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
        fb -= 1.0f;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
        lr += 1.0f;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        lr -= 1.0f;
    }
    pTank->steer(fb, lr);
    pTank->update(frame_t);
    Cam.update();
}

Vector Application::calc3DRay( float x, float y, Vector& Pos)
{
    Matrix projM = Cam.getProjectionMatrix();
    projM = projM.invert();
    Vector ray = projM * Vector(x, y, 0);
    
    //std::cout << ray.X << ":" << ray.Y << ":" << ray.Z << std::endl;
    Matrix viewM = Cam.getViewMatrix();
    Vector ray3D = viewM.invert().transformVec3x3(ray);
    Pos = Cam.position();
    // Pos:Ray Origin
    // return:Ray Direction
    return ray3D.normalize();
}

void Application::draw()
{
    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. setup shaders and draw models
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
    
    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;
    
    Models.clear();
}