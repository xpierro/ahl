#include "Entity.h"

Entity::Entity(int id, string classname, MathVector origin, vector<pair<string, string > > properties) {
	this->id = id;
	this->classname = classname;
	this->origin = origin;
	this->properties = properties;
}

void Entity::print() {
	cout << "Entity: " << id << endl;
	cout << "classname " << classname << endl;
	cout << "origin " << origin.getX() << " " << origin.getY() << " " << origin.getZ() << endl;

	for (size_t i = 0; i < properties.size(); i++) {
		cout << properties[i].first << " " << properties[i].second << endl;
	}
}
