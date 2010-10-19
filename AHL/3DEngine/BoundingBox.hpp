#ifndef _B_BOX_H_
#define _B_BOX_H_

#include <vector>
#include "MathVector.hpp"
#include "Polygon.hpp"

class BoundingBox {
private :
	std::vector<MathVector*> vertices; // Tous les points du cube, dans un ordre aléatoire
	double minx, maxx, miny, maxy, minz, maxz;
	std::vector<Polygon*> faces;
	GLuint queryId;
public :
	BoundingBox(double&, double&, double&, double&, double&, double&);
	const std::vector<MathVector*>& getVertices();
	MathVector getNVertex(MathVector& planeNormal);
	MathVector getPVertex(MathVector& planeNormal);
	bool pointIsInBox(MathVector& v) {
		// Retourne faux le plus vite possible, evaluation paresseuse
		return !(v.getX() < minx || v.getX() > maxx || v.getY() < miny || v.getY() > maxy || v.getZ() < minz || v.getZ() > maxz);
	}
	bool isOccluded(); // Dit si la boite est visible

};

#endif
