#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <list>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <utility>

#include "MathVector.hpp"
#include "Structs.hpp"
#include "Plane.hpp"

using namespace std;

class Polygon {
private :
	vector<MathVector> vertices;
	map<MathVector, struct texcoord, cmp_vect> texCoords;


	string texPath;
	struct axis uAxis;
	struct axis vAxis;

	GLuint dList;
	GLuint qdList;

public :
	Polygon() {
		//vertices = vector<MathVector*>();
		//texCoords = map<MathVector, struct texcoord, cmp_vect>();
	}

	Polygon(vector<MathVector>& vertices, MathVector planeNormal) { // Créé un polygone non texturé (occlusion culling)
		this->vertices = vertices;
		orderVertices();
		if (getNormal().scalarP(planeNormal) < 0) {
			reverseOrder();
		}
		createQuickDisplayList();
	}

	Polygon(const Polygon& p) {
		vertices = p.vertices;
		texCoords = p.texCoords;
		texPath = p.texPath;
		uAxis = p.uAxis;
		vAxis = p.vAxis;
		dList = p.dList;
	}


	Polygon(vector<MathVector>&, string texPath, struct axis uaxis, struct axis vaxis, MathVector planeNormal);
	string getTexPath() {return texPath;}
	MathVector getNormal();
	void reverseOrder();
	void orderVertices();
	MathVector getCenter();
	double angleBetweenVertices(MathVector d1, MathVector d2, MathVector center);
	const vector<MathVector>& getVertices();
	D_POSITION classifyPolygon(Plane *p);
	pair<Polygon*, Polygon*> splitPolygon(Plane *p);
	void draw();
	void quickDraw();

private :
	void buildTexCoord();
	void createDisplayList();
	void createQuickDisplayList();
};

#endif /* POLYGON_HPP_ */
