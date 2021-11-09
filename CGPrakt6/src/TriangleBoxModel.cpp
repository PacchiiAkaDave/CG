//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"



TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{
    float x = Width/2.0f;
    float y = Height/2.0f;
    float z = Depth/2.0f;
    Vector a(-x, -y, -z);
    Vector b(-x, y, -z);
    Vector c(-x, -y, z);
    Vector d(-x, y, z);
    Vector e(x, -y, -z);
    Vector f(x, y, -z);
    Vector g(x, -y, z);
    Vector h(x, y, z);

    VB.begin();
    
    FaceArea(VB, e, f, b, a, Vector(0, 0, -1)); //left
    FaceArea(VB, c, d, h, g, Vector(0, 0, 1)); //right
    FaceArea(VB, e, a, c, g, Vector(0, -1, 0)); //unten
    FaceArea(VB, d, b, f, h, Vector(0, 1, 0)); //oben
    FaceArea(VB, a, b, d, c, Vector(-1, 0, 0)); //hinten
    FaceArea(VB, g, h, f, e, Vector(1, 0, 0)); //vorne

    VB.end();

    // 2. setup index buffer
    IB.begin();
    
    for (int i = 0; i < 6; i++) {
        //erster Triangle
        IB.addIndex(0 + (4*i));
        IB.addIndex(3 + (4*i));
        IB.addIndex(2 + (4*i));

        //zweiter Triangle
        IB.addIndex(0 + (4*i));
        IB.addIndex(2 + (4*i));
        IB.addIndex(1 + (4*i));
    }

    IB.end();
	/// TODO: Add your code (Exercise 2)
}

void TriangleBoxModel::FaceArea(VertexBuffer& VB, Vector a, Vector b, Vector c, Vector d, Vector n) {
    VB.addNormal(n);
    VB.addTexcoord0(0, 1);
    VB.addVertex(a);
    VB.addTexcoord0(0, 0);
    VB.addVertex(b);
    VB.addTexcoord0(1, 0);
    VB.addVertex(c);
    VB.addTexcoord0(1, 1);
    VB.addVertex(d);
}

void TriangleBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);
    VB.activate();
    IB.activate();

    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

    IB.deactivate();
    VB.deactivate();
	// TODO: Add your code (Exercise 2)
}
