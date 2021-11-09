//
//  Tank.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 16.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Tank.h"


Tank::Tank()
{
    angle_corr = 0.0f;
}

Tank::~Tank()
{
    delete bot, top;
}

bool Tank::loadModels(const char* ChassisFile, const char* CannonFile)
{
    if (ChassisFile == nullptr || CannonFile == nullptr) {
        return false;
    }
    bot = new Model(ChassisFile, false);
    bot->shader(pShader);
    top = new Model(CannonFile, false);
    top->shader(pShader);
    return true;
}

void Tank::steer( float ForwardBackward, float LeftRight)
{
    dir.X = ForwardBackward;
    dir.Y = LeftRight;
}

void Tank::aim(const Vector& Target )
{
    Matrix pos = top->transform();

    float Z = -Target.Z + pos.m23;
    float X = -Target.X + pos.m03;

    aim_angle = atan2f(Z, -X);

    
}

void Tank::update(float dtime)
{
    Matrix BottomTM;
    BottomTM.translation(dir.X * dtime , 0, 0);
    Matrix BottomRM;
    BottomRM.rotationY(dir.Y*dtime);
    Matrix BottomRTM;
    BottomRTM = BottomTM * BottomRM;
    bot->transform(bot->transform()*BottomRTM); //Für den Fall als Variable speichern

    angle_corr -= dir.Y * dtime;
    Matrix TopRM;
    TopRM.rotationY(aim_angle + angle_corr);
    top->transform(bot->transform() * TopRM);

    
}

void Tank::draw(const BaseCamera& Cam)
{
    bot->draw(Cam);
    top->draw(Cam);
}
