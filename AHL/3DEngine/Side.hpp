#ifndef SIDE_HPP_
#define SIDE_HPP_

#include <iostream>
#include <string>
#include "Structs.hpp"
#include "MathVector.hpp"
#include "Plane.hpp"

using namespace std;

class Side {
    private :
		int id;
		Plane plane;
		string material_path;
		struct axis uaxis;
		struct axis vaxis;
		float rotation;
		int lightMapScale;
		int smoothingGroups;
		//DispInfo dispInfo; Pas obligatoire

	public :
		Side(int id, Plane plane, string material_path, struct axis uaxis,
				struct axis vaxis, float rotation, int lms, int sg) {
			this->id = id;
			this->plane = plane;
			this->material_path = material_path;
			this->uaxis = uaxis;
			this->vaxis = vaxis;
			this->rotation = rotation;
			this->lightMapScale = lms;
			this->smoothingGroups = sg;
		}
		Side() {};
		Plane getPlane() {return plane;}
		string getMaterialPath() {return material_path;}
		struct axis getUAxis() {return uaxis;}
		struct axis getVAxis() {return vaxis;}

	private :
		MathVector getNPlaneVertex(int n); // Retourne le nieme point du triangle !
};

#endif /* SIDE_HPP_ */
