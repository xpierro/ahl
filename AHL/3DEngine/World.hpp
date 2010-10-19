
#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <string>
#include <list>
#include "Solid.hpp"

using namespace std;

class World {
    private :
        int id;
        int mapVersion;
        int hammerId;

        string name;
        string comment;
        string skyName;
        vector<Solid> solids; // Tous les solides de la map

    public :
        World(string fileName) {parseFile(fileName);}
        World() {}
        World(vector<Solid> solids) {this->solids = solids;}
        void parseFile(string fileName);
};


#endif /* WORLD_HPP_ */
