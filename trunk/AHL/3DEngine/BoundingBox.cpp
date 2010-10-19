#include "BoundingBox.hpp"
#include <vector>
#include "OGLWrapper.hpp"


using namespace std;

void pushVertice(double& x, double& y, double& z, vector<MathVector*>& vertices) {
	vertices.push_back(new MathVector(x, y, z));
}

void pushFace(MathVector& v1, MathVector& v2, MathVector& v3, MathVector& v4, MathVector planeNormal, vector<Polygon*>& faces) {
	vector<MathVector> vertices(4);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
	faces.push_back(new Polygon(vertices, planeNormal));
}

BoundingBox::BoundingBox(double& minx, double& maxx, double& miny, double& maxy, double& minz, double& maxz) {
	MathVector A(minx, miny, minz);
	MathVector B(maxx, miny, minz);
	MathVector C(minx, maxy, minz);
	MathVector D(maxx, maxy, minz);
	MathVector E(minx, miny, maxz);
	MathVector F(maxx, miny, maxz);
	MathVector G(minx, maxy, maxz);
	MathVector H(maxx, maxy, maxz);
	// FRONT
	pushFace(A, B, C, D, MathVector(0, 0, -1), faces);
	// BACK
	pushFace(E, F, G, H, MathVector(0, 0, 1), faces);
	// UP
	pushFace(C, D, G, H, MathVector(0, 1, 0), faces);
	// DOWN
	pushFace(A, B, E, F, MathVector(0, -1, 0), faces);
	// LEFT
	pushFace(A, C, E, G, MathVector(-1, 0, 0), faces);
	// RIGHT
	pushFace(B, D, F, H, MathVector(1, 0, 0), faces);

	pushVertice(minx, miny, minz, vertices);
	pushVertice(maxx, miny, minz, vertices);
	pushVertice(minx, maxy, minz, vertices);
	pushVertice(maxx, maxy, minz, vertices);
	pushVertice(minx, miny, maxz, vertices);
	pushVertice(maxx, miny, maxz, vertices);
	pushVertice(minx, maxy, maxz, vertices);
	pushVertice(maxx, maxy, maxz, vertices);
	this->minx = minx;
	this->maxx = maxx;
	this->miny = miny;
	this->maxy = maxy;
	this->minz = minz;
	this->maxz = maxz;
	OGLWrapper::glGenQueriesARB(1, &queryId);
}

const vector<MathVector*>& BoundingBox::getVertices() {
	return vertices;
}

// Ces points serviront a simplifier le test de bounding box

// Vertex negatif
MathVector BoundingBox::getNVertex(MathVector& planeNormal) {
	double x = maxx;
	double y = maxy;
	double z = maxz;
	if (planeNormal.getX() >= 0) x = minx;
	if (planeNormal.getY() >=0)  y = miny;
	if (planeNormal.getZ() >= 0) z = minz;
	return MathVector(x, y, z);
}

// Vertex positif
MathVector BoundingBox::getPVertex(MathVector& planeNormal) {
	double x = minx;
	double y = miny;
	double z = minz;
	if (planeNormal.getX() >= 0) x = maxx;
	if (planeNormal.getY() >=0) y = maxy;
	if (planeNormal.getZ() >= 0) z = maxz;
	return MathVector(x, y, z);
}

// BEAUCOUP trop lourd!!!
/**
 * Ce qu'il faudrait c'est :
 * - Pour chaque bounding box du frustum (aie) faire le test, puis ne dessiner que les non occludées.
 * - Pour chaque poly, viable ?
 * TODO : HERE!!!!!!!
 */
bool BoundingBox::isOccluded() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_FLAT);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_NORMALIZE);
	glDepthMask(GL_FALSE);
	glColorMask(0, 0, 0, 0);

	GLint passingSample; // Le nombre de face dessiné, ici 1 ou 0 :)
	OGLWrapper::glBeginQueryARB(GL_SAMPLES_PASSED, queryId);
	for(size_t i = 0; i < faces.size(); i++) {
		faces[i]->quickDraw();
	}
	OGLWrapper::glEndQueryARB(GL_SAMPLES_PASSED);
	OGLWrapper::glGetQueryObjectivARB(queryId, GL_QUERY_RESULT, &passingSample);
	glPopAttrib();
	return passingSample == 0;
}
