/*
 * Shape.h
 *
 *  Created on: 11 oct. 2010
 *      Author: Pierre
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "../Vertex3D.h"

#include <list>

using namespace std;

namespace PS3 {

class Shape {
protected:
	list<Vertex3D> *vertices; //!\ Vertex ordonnés
public:
	Shape();
	Shape(const Shape&);
	Shape(const list<Vertex3D>*);

	virtual ~Shape();

	const list<Vertex3D>* getVertices() const;
	void setVertices(const list<Vertex3D>*);

	/**
	 * Dessine l'objet graphique, par défaut lie les vertex un a un en fermant
	 * la forme, mais peut être surchargé pour dessiner un cercle par exemple.
	 */
	virtual void draw() const;
};

}

#endif /* SHAPE_H_ */
