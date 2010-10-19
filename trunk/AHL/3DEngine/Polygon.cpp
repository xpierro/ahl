#include "Polygon.hpp"
#include "OGLWrapper.hpp"
#include "Plane.hpp"
#include <list>
#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>

Polygon::Polygon(vector<MathVector>& vertices, string texPath, struct axis uAxis, struct axis vAxis, MathVector planeNormal) {
	this->texPath = texPath;

	this->vertices = vertices;

	this->uAxis = uAxis;
	this->vAxis = vAxis;

	OGLWrapper::register_texture(texPath);

	buildTexCoord();

	orderVertices();

	if (getNormal().scalarP(planeNormal) < 0) {
		reverseOrder();
	}
	createDisplayList();
}


D_POSITION Polygon::classifyPolygon(Plane *p) {
	size_t nbFront = 0;
	size_t nbBehind = 0;
	size_t nbOn = 0;

	size_t nbVertices = vertices.size();
	for (size_t i = 0; i < nbVertices; i++) {
		switch(p->classifyPoint(vertices[i])) {
		case BEHIND : nbBehind++; break;
		case FRONT : nbFront++; break;
		case ON : nbOn++; break;
		default : break;
		}
	}

	//cout << "beh = " << nbBehind << " front = " << nbFront << "on = " << nbOn << endl;

	// Dans le cas d'un carré, dont deux points seraient front et deux autre ON, on n'a pas de spanning... tout est front...
	if (nbFront == nbVertices || (nbFront != 0 && nbBehind == 0)) {
		return FRONT;
	}

	if (nbBehind == nbVertices || (nbBehind != 0 && nbFront == 0)) {
		return BEHIND;
	}

	if (nbOn == nbVertices) {
		return ON;
	}

	return SPANNING;
}

//   BACK     FRONT   coupe le polygone selon le plan
pair<Polygon*, Polygon*> Polygon::splitPolygon(Plane *p) {
	MathVector normal = p->getNormal();
	size_t nbVertices = vertices.size();

	vector<MathVector> front;
	vector<MathVector> back;

	int verticesPositions[nbVertices];

	for (size_t i = 0; i < nbVertices; i++) {
		verticesPositions[i] = p->classifyPoint(vertices[i]);
	}

	for (size_t i = 0; i < nbVertices; i++) {
		int next = (i == nbVertices - 1) ? 0 : i + 1; // Parcours "circulaire"
		switch(verticesPositions[i]) {
		case FRONT :
				 front.push_back(vertices[i]);
				 break;
		case BEHIND :
				  back.push_back(vertices[i]);
				  break;
		case ON :
				  front.push_back(vertices[i]);
				  back.push_back(vertices[i]);
				  break;
		default : break; // Ne doit jamais arriver
		}

		// TODO : expliquer ça
		bool ignoreVertex = (verticesPositions[i] == ON && verticesPositions[next] != ON)
							|| (verticesPositions[next] == ON && verticesPositions[i] != ON);
		bool sameSide = verticesPositions[i] == verticesPositions[next];

		if (!ignoreVertex && !sameSide) {
			MathVector v((vertices[i] - vertices[next])); // next->i
			double denom = p->getNormal().scalarP(v);
			if (denom != 0) {
				double pos = -(p->getNormal().scalarP(vertices[i]) + p->getDistanceFromO()) / denom; // Pourcentage sur la droite
				MathVector vertex(vertices[i] + (v * pos));
				pos /= (vertices[next] - vertices[i]).getLength(); // TODO : vérifier pq pas v.getLength() ?
				//pos /= v.getLength();
				front.push_back(vertex);
				back.push_back(vertex);
			}
		}
	}

	Polygon *backP = new Polygon(back, texPath, uAxis, vAxis, getNormal());
	Polygon *frontP = new Polygon(front, texPath, uAxis, vAxis, getNormal());

	return pair<Polygon*, Polygon*>(backP, frontP);
}

/*
void Polygon::reverseOrder() {
	reverse(vertices.begin(), vertices.end());
}*/

void Polygon::reverseOrder() {
	vector<MathVector> ordered(vertices.size());
	reverse_copy(vertices.begin(), vertices.end(), ordered.begin());
	vertices = ordered;
}

void Polygon::buildTexCoord() {
	int nbOfVertices = vertices.size();

	pair<int, int> bounds = OGLWrapper::getBounds(texPath);
	int w = bounds.first;
	int h = bounds.second;

	struct texcoord tcs[nbOfVertices];

	for (size_t i = 0; i < vertices.size(); i++) {
		double tu = 0;
		double tv = 0;

		struct texcoord tc;

		MathVector v = vertices[i];

		tu = (MathVector(uAxis.x, uAxis.y, uAxis.z).scalarP(v) / uAxis.scale) + uAxis.shift;
		tv = (MathVector(vAxis.x, vAxis.y, vAxis.z).scalarP(v) / vAxis.scale) + vAxis.shift;

		if (w > 0) {
			tu /= (double) w;
		}
		if (h > 0) {
			tv /= (double) h;
		}

		if (tu < 0.001 && tu > - 0.001) {
			tu = 0;
		}

		if (tv < 0.001 && tv > - 0.001) {
			tv = 0;
		}

		tc.u = tu;
		tc.v = tv;
		tcs[i] = tc;
	}

	// affichage du résultat
	for (int i = 0; i < nbOfVertices; i++) {
		pair<MathVector, struct texcoord > pp(vertices[i], tcs[i]);
		texCoords.insert(pp); //OUILLE !
	}
}

const vector<MathVector>& Polygon::getVertices() {
	return vertices;
}

void Polygon::draw() {
	OGLWrapper::activate_texture(getTexPath());
	glCallList(dList);
}

void Polygon::createDisplayList() {
	dList = glGenLists(1);
	MathVector normal = getNormal();
	glNewList(dList, GL_COMPILE);
    glNormal3f(normal.getX(), normal.getY(), normal.getZ());

    /*
    glBegin(GL_LINE); {
    	glVertex3f(getCenter().getX(), getCenter().getY(), getCenter().getZ());
    	glVertex3f(normal.getX(), normal.getY(), normal.getZ());
    } glEnd();
	*/

    glBegin(GL_POLYGON); {
    	for(size_t i = 0; i < vertices.size(); i++) {
    		MathVector& vertex = vertices[i];
    		struct texcoord uv = texCoords[vertex];
    		glTexCoord2d(uv.u, uv.v);
        	glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
    	}
    }
    glEnd();
    glEndList();
}

void Polygon::createQuickDisplayList() {
	qdList = glGenLists(1);
	glNewList(qdList, GL_COMPILE);

    glBegin(GL_POLYGON); {
    	for(size_t i = 0; i < vertices.size(); i++) {
    		MathVector& vertex = vertices[i];
        	glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
    	}
    }
    glEnd();
    glEndList();
}


void Polygon::quickDraw() {
	glCallList(qdList);

}

MathVector Polygon::getNormal() {
	MathVector edge1 = (vertices[1]) - (vertices[0]);
	MathVector edge2 = (vertices[2]) - (vertices[1]);
	return edge1.vectP(edge2); // TODO : est-on sur du sens de rotation?
}

MathVector Polygon::getCenter() {
	MathVector center(0, 0, 0);

	for (size_t i = 0; i < vertices.size(); i++) {
		center += (vertices[i]);
	}

	center = center / vertices.size();
	return center;
}

double Polygon::angleBetweenVertices(MathVector d1, MathVector d2, MathVector center) {
	MathVector a = (d1 - center).getNormalized();
	MathVector b = (d2 - center).getNormalized();

	return a.scalarP(b);
}

void Polygon::orderVertices() {
	MathVector center = getCenter();
	MathVector normal = getNormal();

	for (size_t i = 0; i < vertices.size() - 2; i++) {
		MathVector a = (vertices[i] - getCenter()).getNormalized();
		Plane p(vertices[i], center, center + normal); // TODO : pointeur ?
		double smallestAngle = -1;
		int smallest = -1;
		for (size_t j = i + 1; j < vertices.size(); j++) {
			if (p.classifyPoint(vertices[j]) != BEHIND) {
				MathVector b = (vertices[j] - center).getNormalized();
				double angle = a.scalarP(b);
				if (angle > smallestAngle) { // TODO :  minimum > ?
					smallestAngle = angle;
					smallest = j;
				}
			}
		}
		if (smallest >= 0 && smallest < vertices.size() && (i + 1) < vertices.size()) {
			swap (vertices[smallest], vertices[i + 1]);
		}
	}
}
/*
void Polygon::orderVertices() {
	vector<MathVector> orderedPoly(vertices.size());

	// Pour simplifier, on va transformer le polynome en un tableau de point:
	vector<MathVector> poly_t = vertices;

	MathVector polyCenter = getCenter();

	// Pour chaque vecteur du tableau, on cherche le vecteur le plus proche
	// on commence par le premier vecteur de la liste, arbitrairement.
	int nbVertices = vertices.size();
	MathVector nextDot = poly_t[0];
	int k = 0;
	orderedPoly[k] = nextDot;
	poly_t.erase(poly_t.begin());
	nbVertices--;

	for (size_t j = 1; j < vertices.size(); j++) {
		double minAngle = -1;
		double currentAngle = 0;
		int minIndex = 0;
		for (int i = 0; i < nbVertices; i++) {
			currentAngle = angleBetweenVertices(nextDot, poly_t[i], polyCenter);
			if (currentAngle > minAngle) {
				minAngle = currentAngle;
				minIndex = i;
			}
		}
		k++;
		orderedPoly[k] = poly_t[minIndex];
		nextDot = poly_t[minIndex];
		poly_t.erase(poly_t.begin() + minIndex);
		nbVertices--;
	}
	vertices = orderedPoly;

}
*/
