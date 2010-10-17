/*
 * Vertex3D.h
 *
 *  Created on: 11 oct. 2010
 *      Author: Pierre
 */

#ifndef VERTEX3D_H_
#define VERTEX3D_H_

#include <psgl/psgl.h>

class Vertex3D {
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
public:
	Vertex3D();
	Vertex3D(float x, float y, float z);
	virtual ~Vertex3D();

	GLfloat getX() const;
	GLfloat getY() const;
	GLfloat getZ() const;
};

#endif /* VERTEX3D_H_ */
