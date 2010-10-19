#include "BSPTree.hpp"
#include "Polygon.hpp"
#include "MathVector.hpp"
#include "Plane.hpp"
#include <iostream>
#include "OGLWrapper.hpp"
#include "BoundingBox.hpp"
#include "ProgressBar.hpp"

using namespace std;

Polygon* getFromList(vector<Polygon*>& polygons) {
	if (polygons.size() == 0) {
		return NULL;
	}
	int index = 0;
	Polygon *p = polygons[index];
	polygons.erase(polygons.begin() + index);
	return p;
}


/**
 * La fonction la plus importante, on doit ici trouver le meilleur polygone pour équilibrer
 */
Polygon* selectBestPlane(vector<Polygon*>& polygons) {
	if (polygons.size() == 0) {
			return NULL;
	}
	int index = polygons.size() / 2;
	Polygon *p = polygons[index];
	polygons.erase(polygons.begin() + index);
	return p;
}

Plane* polyToPlane(Polygon *p) {
	vector<MathVector> vertices = p->getVertices();
    return new Plane(vertices[0], vertices[1], vertices[2]);
}

// TODO : est-ce si parfait ?
BoundingBox* buildBox(const vector<Polygon*>& polygons) {
	double minx = polygons[0]->getVertices()[0].getX();
	double miny = polygons[0]->getVertices()[0].getY();
	double minz = polygons[0]->getVertices()[0].getZ();
	double maxx = minx;
	double maxy = miny;
	double maxz = minz;

	for (size_t i = 0; i < polygons.size(); i++) {
		vector<MathVector> vertices = polygons[i]->getVertices();
		for (size_t j = 0; j < vertices.size(); j++) {
			MathVector &v = vertices[j];
			double x = v.getX();
			double y = v.getY();
			double z = v.getZ();
			if (x < minx) minx = x;
			if (x > maxx) maxx = x;
			if (y < miny) miny = y;
			if (y > maxy) maxy = y;
			if (z < minz) minz = z;
			if (z > maxz) maxz = z;
		}
	}
	return new BoundingBox(minx, maxx, miny, maxy, minz, maxz);
}

int BSPTree::idCounter = 0;

BSPTree* BSPTree::buildBSPTree(vector<Polygon*> polygons) {
	string type = "bsp";
	ProgressBar::registerType(type);

	if (polygons.size() == 0) {
		return NULL;
	}
	BSPTree *tree = new BSPTree();
	tree->id = idCounter;
	idCounter++;
	tree->bBox = buildBox(polygons);
	Polygon *root = selectBestPlane(polygons);
	tree->partition = polyToPlane(root);
	tree->polygons.push_back(root);

	// TODO : mauvais, limiter ainsi l'arbre casse le principe du BSP et oblige à avoir un Z-Buffer car on ne sait plus dans quel ordre dessiner :'(
	// MAIS accélère tellement le rendering qu'on va laisser comme ca :D
	if (polygons.size() <= 500) {
		tree->front = NULL;
		tree->back = NULL;

		for(size_t i = 0; i < polygons.size(); i++) {
			tree->polygons.push_back(polygons[i]);
		}
		return tree;
	}

	vector<Polygon*> frontPolys;
	vector<Polygon*> behindPolys;
	Polygon *poly;
	while((poly = getFromList(polygons)) != NULL ) {
		int classification = poly->classifyPolygon(tree->partition);
		switch (classification) {
		case ON :
			tree->polygons.push_back(poly);
			break;
		case FRONT :
			frontPolys.push_back(poly);
			break;
		case BEHIND :
			behindPolys.push_back(poly);
			break;
		case SPANNING :
			pair<Polygon*, Polygon*> bf = poly->splitPolygon(tree->partition);
			behindPolys.push_back(bf.first);
			frontPolys.push_back(bf.second);
			break;
		}
	}
	if (frontPolys.size() != 0) {
	      tree->front = buildBSPTree(frontPolys);
	} else {
		tree->front = NULL;
	}
	if (behindPolys.size() != 0) {
	      tree->back = buildBSPTree(behindPolys);
	} else {
		tree->back = NULL;
	}
	return tree;
}

void drawPolygons(vector<Polygon*> &v) {
	for (size_t i = 0; i < v.size(); i++) {
		v[i]->draw();
	}
}

void drawTree(BSPTree *t, MathVector& eye, MathVector& visee, bool b) {
	if (t) {
		t->draw(eye, visee, b);
	}
}

void BSPTree::draw(MathVector& eye, MathVector& visee, bool checkFrustum) {
	if (checkFrustum) {
		if (!bBox->pointIsInBox(eye)) { // Si l'oeil est dans la bounding box, alors on doit afficher, pas besoin de tester
			switch (OGLWrapper::boxIsInFrustum(bBox)) {
			case IN : // Ici, occlusion culling! 1. la boite est dans le frustum, mais est-elle caché ? Si oui, alors return
				if(bBox->isOccluded()) return;
				checkFrustum = false; break; // On ne doit plus reverifier, tous les fils sont visibles dans le frustum
			case OUT : return; // La bounding box est invisible, aucun interet de continuer
			default : break; // On ne fait rien, puisqu'une partie est encore visible
			}
		}
	} else if(bBox->isOccluded()) return;

	// N.B.: Normalement ici on a cullé le maximum possible, mais à quel prix ?
	// Le PVS, potentially visible set étant trop difficile, l'occlusion culling materiel etant trop lent, que reste-il ?
	// Solutions : les VBO, diviser le monde en solide et non en polygones (la plus grosse erreur de conception, difficile à corriger maintenant)
	//             utilise un scene graph (est-ce si efficace?),

	double result = partition->classifyPoint(eye);
	if (result == BEHIND) { // OCCLUSION CULLING INVERSION
		drawTree(back, eye, visee, checkFrustum);
	    drawPolygons(polygons);
	    drawTree(front, eye, visee, checkFrustum);
	} else if (result == FRONT) {
		drawTree(front, eye, visee, checkFrustum);
		drawPolygons(polygons);
		drawTree(back, eye, visee, checkFrustum);
	} else {
		drawTree(front, eye, visee, checkFrustum);
		drawTree(back, eye, visee, checkFrustum);
	}
}
