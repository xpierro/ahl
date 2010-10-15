/*
 * Shape.cpp
 *
 *  Created on: 11 oct. 2010
 *      Author: Pierre
 */

#include "Shape.h"

namespace PS3 {

Shape::Shape() {
	vertices = NULL;
}

Shape::Shape(const Shape& s) {
	vertices = new list<Vertex3D>();
	setVertices(s.vertices);
}

Shape::Shape(const list<Vertex3D>* verticesp) {
	setVertices(verticesp);
}

Shape::~Shape() {
	delete vertices;
}

const list<Vertex3D>* Shape::getVertices() const {
	return vertices;
}

void Shape::setVertices(const list<Vertex3D>* verticesp) {
	delete vertices;
	vertices = new list<Vertex3D>();
	list<Vertex3D>::const_iterator it = verticesp->begin();
	while(it != verticesp->end()) {
		vertices->push_back(*it);
		++it;
	}
}

void Shape::draw() const {

}

}
