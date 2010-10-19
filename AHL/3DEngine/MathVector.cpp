#include "MathVector.hpp"
#include "Structs.hpp"
#include <math.h>

MathVector MathVector::rotate(double angle, MathVector around) {
	MathVector me = *this;
	return me * cos(angle) + around * ((1 - cos(angle)) * me.scalarP(around)) + around.vectP(me) * sin(angle);
}

MathVector::MathVector(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

MathVector::MathVector(MathVector d1, MathVector d2) {
	x = d2.x - d1.x;
	y = d2.y - d1.y;
	z = d2.z - d1.z;
}

double MathVector::scalarP(MathVector v) {
	return x * v.x + y * v.y + z * v.z;
}

MathVector MathVector::vectP(MathVector v) {
	/*
	 * Ux = Vy * Wz - Wy * Vz;
	 * Uy = Vz * Wx - Wz * Vx;
	 * Uz = Vx * Wy - Wx * Vy;
	 */
	MathVector vv(y * v.z - v.y * z,
				  z * v.x - v.z * x,
				  x * v.y - v.x * y);
	return vv;
}

MathVector MathVector::operator*(double scalar) {
	return MathVector(x * scalar, y * scalar, z * scalar);
}

MathVector MathVector::operator/(double scalar) {
	return MathVector(x / scalar, y / scalar, z / scalar);
}

MathVector MathVector::operator+(MathVector v) {
	return MathVector(x + v.x, y + v.y, z + v.z);
}

void MathVector::operator+=(MathVector v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void MathVector::operator-=(MathVector v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

MathVector MathVector::operator-(MathVector v) {
	return MathVector(x - v.x, y - v.y, z - v.z);
}

bool MathVector::operator==(MathVector v) {
	return x == v.x && y == v.y && z == v.z;
}

double MathVector::getLength() {
	return sqrt(x * x + y * y + z * z);
}

MathVector MathVector::getNormalized() {
	double length = getLength();
	return MathVector(x / length, y / length, z / length);
}
