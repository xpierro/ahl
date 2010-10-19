#include "Solid.hpp"
#include "OGLWrapper.hpp"
#include <string.h>

Solid::Solid(int id, vector<Side> sides) {
	this->id = id;
	this->sides = sides;

	calculatePolys();
}

MathVector* Solid::getIntersection(MathVector n1, MathVector n2, MathVector n3,
									double d1, double d2, double d3) {
	double denom = n1.scalarP(n2.vectP(n3));
	if (denom == 0) {
		return NULL;
	}

	MathVector *v = new MathVector((n2.vectP(n3) * -d1 + n3.vectP(n1)* -d2 + n1.vectP(n2) * -d3) * (1 / denom));

	return v;
}

MathVector* Solid::getIntersection(Plane p1, Plane p2, Plane p3) {
	MathVector n1 = p1.getNormal();
	MathVector n2 = p2.getNormal();
	MathVector n3 = p3.getNormal();

	double d1 = p1.getDistanceFromO();
	double d2 = p2.getDistanceFromO();
	double d3 = p3.getDistanceFromO();

	return getIntersection(n1, n2, n3, d1, d2, d3);
}

bool Solid::dotIsInPoly(MathVector *v, vector<MathVector> poly) {
	for (size_t i = 0; i < poly.size(); i++) {
		if (*v == poly[i]) {
			return true;
		}
	}
	return false;
}

bool dotIsInBrush(MathVector *v, vector<Side> sides) {
	for (size_t i = 0; i < sides.size(); i++) {
		Plane plane = sides[i].getPlane();
		if(plane.classifyPoint(v) == FRONT) {
			return false;
		}
	}
	return true;
}

// A vérifier !
void Solid::calculatePolys() {
	vector<MathVector> polys[sides.size()];
	for(size_t i = 0; i < sides.size(); i++) {
		for(size_t j = 0; j < sides.size(); j++) {
			for (size_t k = 0; k < sides.size(); k++) {
				if (i != j && j != k) {

					MathVector *v;
					Plane iPlane = sides[i].getPlane();
					Plane jPlane = sides[j].getPlane();
					Plane kPlane = sides[k].getPlane();

					v = getIntersection(iPlane, jPlane, kPlane);
					if (v != NULL && !dotIsInPoly(v, polys[i]) && !dotIsInPoly(v, polys[j]) && !dotIsInPoly(v, polys[k]) && dotIsInBrush(v, sides)) {
						polys[i].push_back(*v);
						polys[j].push_back(*v);
						polys[k].push_back(*v);
					}
					delete v;
				}
			}
		}
	}
	for (size_t i = 0; i < sides.size(); i++) {
		if (polys[i].size() >= 3) {
			//if (sides[i].getMaterialPath().compare(0, 5, "tools")) {
				Polygon p(polys[i], sides[i].getMaterialPath(), sides[i].getUAxis(), sides[i].getVAxis(), sides[i].getPlane().getNormal());
				OGLWrapper::registerPolygon(p);
			//}
		}
	}
}
