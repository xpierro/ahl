#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "MathVector.hpp"

enum D_POSITION {
	FRONT,
	BEHIND,
	ON,
	SPANNING, // Utilisé pour les polygons, si traverse.
	IN,
	OUT
};

class Plane {
private :
	MathVector A;
	MathVector B;
	MathVector C;

public :
	Plane(MathVector, MathVector, MathVector);
	Plane() {}
	MathVector getNormal();
	double getDistanceFromO();
	MathVector getNPlaneVertex(int n);
	D_POSITION classifyPoint(const MathVector& d);
};

#endif /* PLANE_HPP_ */
