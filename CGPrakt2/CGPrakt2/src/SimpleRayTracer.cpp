//
//  SimpleRayTracer.cpp
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 03.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "SimpleRayTracer.h"
#include "rgbimage.h"
#include <stdio.h>
#include <math.h>

#define EPSILON 1e-6

Camera::Camera( float zvalue, float planedist, float width, float height, unsigned int widthInPixel, unsigned int heightInPixel )
{
	this->zvalue = zvalue;
	this->planedist = planedist;
	this->width = width;
	this->height = height;
	this->heightInPixel = heightInPixel;
	this->widthInPixel = widthInPixel;
	this->height_prop = height / heightInPixel;
	this->width_prop = width / widthInPixel;
	// TODO: Add your code
}

Vector Camera::generateRay( unsigned int x, unsigned int y) const
{
	if (x > widthInPixel - 1) {
		x = widthInPixel - 1;
	}
	if (y > heightInPixel - 1) {
		y = heightInPixel - 1;
	}
	float n_X = 0;
	float n_Y = 0;
	int tmp;
	tmp = x - widthInPixel * 0.5;
	n_X = width_prop * tmp;
	tmp = heightInPixel*0.5 - y;
	n_Y = height_prop * tmp;
	Vector tmp_v(n_X, n_Y, zvalue + planedist); // Punkt der Bildebene
	Vector ray = tmp_v - Position();
	ray = ray.normalize();
	// TODO: Add your code
    return ray;
}

Vector Camera::Position() const
{
	// TODO: Add your code
	return Vector(0,0,zvalue);
}

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth)
{
	this->m_maxDepth = MaxDepth;
	// TODO: Add your code
}


void SimpleRayTracer::traceScene( const Scene& SceneModel, RGBImage& Image)
{
	Camera camera(-8, 1, 1, 0.75, 640, 480);
	for (int i = 0; i < 480; i++) {
		for (int j = 0; j < 640; j++) {
			Image.setPixelColor(j, i, Color(0, 0, 0));
			Vector s = camera.generateRay(j, i);
			Image.setPixelColor(j, i, trace(SceneModel, camera.Position(), s, 0));
		}
	}
	
}

Color SimpleRayTracer::localIllumination( const Vector& Surface, const Vector& Eye, const Vector& N, const PointLight& Light, const Material& Mtrl )
{
	Vector L = (Light.Position - Surface).normalize();
	float ndotl = L.dot(N);
	if (ndotl <= 0) {
		ndotl = 0;
	}
	Color D = Light.Intensity * Mtrl.getDiffuseCoeff(Surface) * ndotl;
	Vector E = (Eye - Surface).normalize();
	Vector R = N * 2 * (N.dot(L)) - L;
	float edotr = E.dot(R);
	if (edotr <= 0) {
		edotr = 0;
	}
	Color S = Light.Intensity * Mtrl.getSpecularCoeff(Surface) * (float)pow(edotr, Mtrl.getSpecularExp(Surface));

	return (D + S);

	//return Mtrl.getDiffuseCoeff(Surface);
}



Color SimpleRayTracer::trace( const Scene& SceneModel, const Vector& o, const Vector& d, int depth)
{
	Color f;
	float s = 0;
	Triangle tmp;
	Triangle intersected;
	float t = INFINITY;
	for (int i = 0; i < SceneModel.getTriangleCount(); i++) {
		tmp = SceneModel.getTriangle(i);
		if (o.triangleIntersection(d, tmp.A, tmp.B, tmp.C, s) == true) {
			if (fabs(s) < EPSILON || t < s) {
				continue;
			}
			intersected = tmp;
			t = s;
		}
	}
	if (t == INFINITY) {
		return f;
	}
	Vector p = o + d * t; //erster Auftreffpunkt
	PointLight tmpL;
	for (int i = 0; i < SceneModel.getLightCount(); i++) {
		tmpL = SceneModel.getLight(i);
		Vector dl = (p - tmpL.Position);
		Vector dln = dl.normalize();
		float s2 = 0;
		Triangle tmp2;
		float t2 = INFINITY;
		for (int i = 0; i < SceneModel.getTriangleCount(); i++) {
			tmp = SceneModel.getTriangle(i);
			if (tmpL.Position.triangleIntersection(dln, tmp.A, tmp.B, tmp.C, s2) == true) {
				if (fabs(s2) < EPSILON || t2 < s2) {
					continue;
				}
				tmp2 = tmp;
				t2 = s2;
			}
		}
		if (dl.length() > (tmpL.Position + dln * t2).length() || t2 == INFINITY) {
			return f;
		}
		if (intersected.A.X == tmp2.A.X && intersected.A.Y == tmp2.A.Y && intersected.A.Z == tmp2.A.Z &&
			intersected.B.X == tmp2.B.X && intersected.B.Y == tmp2.B.Y && intersected.B.Z == tmp2.B.Z &&
			intersected.C.X == tmp2.C.X && intersected.C.Y == tmp2.C.Y && intersected.C.Z == tmp2.C.Z &&
			intersected.pMtrl == tmp2.pMtrl) {
			f += localIllumination(p, o, intersected.calcNormal(p), tmpL, *intersected.pMtrl);
		}
	}
	if (depth == m_maxDepth) {
		f += intersected.pMtrl->getAmbientCoeff(p);
	}
	Vector r = d.reflection(intersected.calcNormal(p));
	if (depth < m_maxDepth) {
		f += trace(SceneModel, p, r, ++depth) * intersected.pMtrl->getReflectivity(p);
	}
	return f;
}

