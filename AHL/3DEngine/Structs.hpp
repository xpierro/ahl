/*
 * Structs.hpp
 *
 *  Created on: 19 mars 2010
 *      Author: pierre
 */

#include "MathVector.hpp"
#include "GL/gl.h"

#ifndef STRUCTS_HPP_
#define STRUCTS_HPP_

struct texcoord {
    double u;
    double v;
};

struct axis {
	double x;
	double y;
	double z;
    int shift;
    double scale;
};

typedef struct {
    GLfloat shininess;
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
} material;

#endif /* STRUCTS_HPP_ */
