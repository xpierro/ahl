#ifndef VMFPARSER_HPP_
#define VMFPARSER_HPP_

#include <string>
#include "World.hpp"
#include "Solid.hpp"
#include "Side.hpp"
#include "Entity.h"

using namespace std;

// Cette classe va se charger de creer tous les objets nécessaires modélisant
// la MAP.

class VMFParser {
private :
	World world;
	//VersionInfo vinfo;
	//ViewSettings vsetting;
	//Camera camera;

	static Entity getEntity(string entityDesc);
public :
	static void parseGlobal(string vmf) {parseWorld(vmf);}

	static World parseWorld(string worldDesc);
	static Solid parseSolid(string solidDesc);
	static Side parseSide(string sideDesc);

	static vector<Entity> parseEntity(string entityDesc);

	static pair<string, string> getKeyValue(string keyValue);
	static string* getBloc(string bulk, size_t startPosition, string blocName);
};

#endif /* VMFPARSER_HPP_ */
