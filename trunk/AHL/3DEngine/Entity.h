#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>
#include <string>
#include <iostream>
#include "MathVector.hpp"

using namespace std;

class Entity {
private:
	int id;
	string classname;
	MathVector origin;
	vector<pair<string, string> > properties;
public:
	Entity(int id, string classname, MathVector origin, vector<pair<string, string> > properties);

	int getId() { return id; }
	string getClassname() { return classname; }
	MathVector getOrigin() { return origin; }
	vector<pair<string, string > > getProperties() { return properties; }

	void print();
};

#endif /* ENTITY_H_ */
