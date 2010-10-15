/*
 * Vertex3D.cpp
 *
 *  Created on: 11 oct. 2010
 *      Author: Pierre
 */

#include "Vertex3D.h"

Vertex3D::Vertex3D() {
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

Vertex3D::Vertex3D(float xp, float yp, float zp) {
	x = xp;
	y = yp;
	z = zp;
}

Vertex3D::~Vertex3D() {
	// TODO Auto-generated destructor stub
}

GLfloat Vertex3D::getX() const {
	return x;
}

GLfloat Vertex3D::getY() const {
	return y;
}

GLfloat Vertex3D::getZ() const {
	return z;
}
