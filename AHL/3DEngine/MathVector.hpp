/*
 * MathVector.hpp
 *
 *  Created on: 20 mars 2010
 *      Author: pierre
 */

#ifndef MATHVECTOR_HPP_
#define MATHVECTOR_HPP_

#include <functional>
#include "Structs.hpp"
#include <iostream>

using namespace std;


class MathVector {
private :
	double x;
	double y;
	double z;
public :
	MathVector(double x, double y, double z);
	MathVector(MathVector d1, MathVector d2);
	MathVector() {}
	MathVector(const MathVector& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	MathVector(const MathVector *v) {
		x = v->x;
		y = v->y;
		z = v->z;
	}

	double getX() const {return x;}
	double getY() const {return y;}
	double getZ() const {return z;}

	double scalarP(MathVector v);
	MathVector vectP(MathVector v);
	MathVector operator*(double scalar);
	MathVector operator+(MathVector v);
	void operator+=(MathVector v);
	void operator-=(MathVector v);
	MathVector operator-(MathVector v);
	MathVector operator/(double scalar);
	bool operator==(MathVector v);
	bool operator!=(MathVector v) {
		return !(*this == v);
	}
	MathVector rotate(double angle, MathVector around);

	double getLength();
	MathVector getNormalized();

	void display() {
		cout << "[" << x << "," << y << "," << z << "]" << endl;
	}
};

struct cmp_vect :
   public std::binary_function<MathVector, MathVector, bool> {
	   bool operator()(const MathVector &v1, const MathVector &v2) const {
		   double comp_x = v1.getX() - v2.getX();
		   double comp_y = v1.getY() - v2.getY();
		   double comp_z = v1.getZ() - v2.getZ();

		   if (comp_x < 0) return true;
		   if (comp_x > 0) return false;
		   if (comp_y < 0) return true;
		   if (comp_y > 0) return false;
		   if (comp_z < 0) return true;

		   return false;
	   }
   };
#endif /* MATHVECTOR_HPP_ */
