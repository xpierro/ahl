#include "Plane.hpp"
#include "Polygon.hpp"
#include <iostream>
#include <math.h>
#include <sys/types.h>

const double EPSILON = 0.001;

using namespace std;

Plane::Plane(MathVector A, MathVector B, MathVector C) {
	this->A = A;
	this->B = B;
	this->C = C;
}

MathVector Plane::getNormal() {
	MathVector AB(A, B);
	MathVector AC(A, C);
	return AB.vectP(AC).getNormalized(); // ORDRE TRIGO
}

D_POSITION Plane::classifyPoint(const MathVector& d) {
	double classification = getNormal().scalarP(d) + getDistanceFromO();
	// http://fr.wikipedia.org/wiki/Distance_d%27un_point_%C3%A0_un_plan
	// DEFINITION : |n . vertex + d| / ||n|| or ici la normale est normalisée, donc getNormal().getLength() == 1
	if (classification < -EPSILON) {
		return BEHIND;
	} else if (classification > EPSILON) {
		return FRONT;
	} else {
		return ON;
	}
}

double Plane::getDistanceFromO() {
	return -(A.scalarP(getNormal()));
}

MathVector Plane::getNPlaneVertex(int n) {
	MathVector v;
	switch(n) {
	case 1 : v = A; break;
	case 2 : v = B; break;
	case 3 : v = C; break;
	}
	return v;
}
