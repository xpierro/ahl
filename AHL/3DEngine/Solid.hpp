#ifndef SOLID_HPP_
#define SOLID_HPP_

#include <list>
#include <string>
#include <vector>
#include <iostream>
#include "Side.hpp"
#include "Plane.hpp"

using namespace std;

class Solid {
    private :
        int id;
        vector<Side> sides; // Tableau de sides

        //Editor *editors; // ??
    public :
        Solid(int id, vector<Side> sides);
        Solid() {}

    private :
        void calculatePolys();

        static MathVector* getIntersection(MathVector n1, MathVector n2, MathVector n3,
									double d1, double d2, double d3);
        static MathVector* getIntersection(Plane p1, Plane p2, Plane p3);

        static bool dotIsInPoly(MathVector *v, vector<MathVector> poly);
};


#endif
