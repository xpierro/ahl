#include "World.hpp"

#include <iostream>
#include <fstream>

#include "VMFParser.hpp"

using namespace std;

void World::parseFile(string fileName) {
	ifstream fichier(fileName.data(), ios::in);  // on ouvre le fichier en lecture
	if(fichier) {  // si l'ouverture a réussi
		string desc;
        char c;
        while(fichier.get(c)) {
        	desc.push_back(c);
        }
        string solidDesc;
        //size_t findIndex = desc.find("{", desc.find("world", 0));
        //VMFParser::parseWorld(*VMFParser::getBloc(desc, 0,"world"));
        VMFParser::parseWorld(desc);
        VMFParser::parseEntity(desc);
        //string *worldDesc = VMFParser::getBloc(string bulk, size_t startPosition, string blocName);
        /*while((findIndex = desc.find("{", desc.find("solid", findIndex))) != string::npos) {
        	findIndex++;
        	int verifier = 1;
        	while(verifier != 0) {
        		if (desc[findIndex] == '{') {
        			verifier++;
        		} else if (desc[findIndex] == '}') {
        			verifier--;
        		}
        		solidDesc.push_back(desc[findIndex]);
        		findIndex++;
        	}
        	Solid s = VMFParser::parseSolid(solidDesc);

        	solidDesc.clear();
        	solids.push_back(s);
        }
*/
		fichier.close();  // on ferme le fichier
    } else {  // sinon
    	cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
}
