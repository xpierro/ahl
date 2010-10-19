#include "VMFParser.hpp"
#include "MathVector.hpp"
#include "ProgressBar.hpp"
#include "OGLWrapper.hpp"
#include <stdio.h>
#include <iostream>
#include <map>

using namespace std;

string* VMFParser::getBloc(string bulk, size_t startPosition, string blocName) {
	startPosition = bulk.find(blocName, startPosition);
	if (startPosition == string::npos) {
		return NULL;
	}

	string* bloc = new string();
	size_t index = startPosition;
	while(bulk[index] != '{' && index < bulk.size()) {
		bloc->push_back(bulk[index]);
		index += 1;
	}
	bloc->push_back('{');
	index += 1;

	int verifier = 1;
	while(verifier != 0 && index < bulk.size()) {
		if (bulk[index] == '{') {
			verifier += 1;
		} else if (bulk[index] == '}') {
			verifier -= 1;
		}
		bloc->push_back(bulk[index]);
		index += 1;
	}
	return bloc; // Aucun controle d'intégrité possible :'(
}

bool isSeparator(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}

string trim(string s) {
	string ss;
	size_t begin = 0;
	while(isSeparator(s[begin])) {
		begin++;
	}
	size_t end = s.size() - 1;
	while(isSeparator(s[end])) {
		end--;
	}
	for (size_t i = begin; i <= end; i++) {
		ss.push_back(s[i]);
	}
	return ss;
}

string toLower(string uppered) {
	for (size_t i = 0; i < uppered.size(); i++) {
		if (uppered[i] >= 'A' && uppered[i] <= 'Z') {
			uppered[i] += 'a' - 'A';
		}
	}
	return uppered;
}

Side VMFParser::parseSide(string sideDesc) {
	int id;
	Plane plane;
	string material_path;
	struct axis uaxis;
	struct axis vaxis;
	float rotation;
	int lightMapScale;
	int smoothingGroups;

	size_t z = sideDesc.find("\"", 0);
	string currentLine;
	for (size_t i = z; i < sideDesc.size(); i++) {
		if (sideDesc[i] == '\n') {
			currentLine = trim(currentLine);
			pair<string, string> kv = getKeyValue(currentLine);
			string key = kv.first;
			string value = kv.second;
			if (!key.compare("id")) {
				sscanf(value.data(), "%d", &id);
			} else if (!key.compare("plane")) {
				MathVector A, B, C;
				double ax, ay, az, bx, by, bz, cx, cy, cz;
				sscanf(value.data(), "(%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf)",
						&ax, &ay, &az, &bx, &by, &bz, &cx, &cy, &cz);
				C = MathVector((int) ax, (int) az, (int) -ay); // Conversion indispensable, car gros manque de précision + inversion necessaire
				B = MathVector((int) bx, (int) bz, (int) -by);
				A = MathVector((int) cx, (int) cz, (int) -cy);
				plane = Plane(A, B, C);
			} else if (!key.compare("material")) {
				material_path = toLower(value);
			} else if (!key.compare("uaxis")) {
				double x, y, z, scale, shift;
				sscanf(value.data(), "[%lf %lf %lf %lf] %lf",
						&x, &y, &z, &shift, &scale);
				uaxis.x = x;
				uaxis.y = z;
				uaxis.z = -y;
				uaxis.scale = scale;
				uaxis.shift = shift;
			} else if (!key.compare("vaxis")) {
				double x, y, z, scale, shift;
				sscanf(value.data(), "[%lf %lf %lf %lf] %lf",
						&x, &y, &z, &shift, &scale);
				vaxis.x = x;
				vaxis.y = z;
				vaxis.z = -y;
				vaxis.scale = scale;
				vaxis.shift = shift;
			} else if (!key.compare("rotation")) {
				sscanf(value.data(), "%f", &rotation);
			} else if (!key.compare("lightmapscale")) {
				sscanf(value.data(), "%d", &lightMapScale);
			} else if (!key.compare("smoothing_groups")) {
				sscanf(value.data(), "%d", &smoothingGroups);
			}
			currentLine.clear();
		} else {
			currentLine.push_back(sideDesc[i]);
		}
	}
	return Side(id, plane, material_path, uaxis, vaxis, rotation,
			lightMapScale, smoothingGroups);

}

Solid VMFParser::parseSolid(string solidDesc) {
	vector<Side> sides;

	int index = solidDesc.find('\"', 0);
	int end = solidDesc.find('\n', index);
	string id_s = trim(solidDesc.substr(index, end - index));
	int id;
	sscanf(getKeyValue(id_s).second.data(), "%d", &id);
	int i = end + 1;
	string *side = getBloc(solidDesc, i, "side");
	while(side != NULL) {
		sides.push_back(parseSide(*side));
		i += side->size();
		delete side;
		side = getBloc(solidDesc, i, "side");
	}
	return Solid(id, sides);
}

Entity VMFParser::getEntity(string entityDesc) {
	int id, u, v, t;
	char classname[128] = { '\0' };
	MathVector origin;

	vector<pair<string, string> > properties;

	size_t z = entityDesc.find("\"", 0);
	string currentLine;
	map<string, string> keyValues;

	for (size_t i = z; i < entityDesc.size(); i++) {
		if (entityDesc[i] == '\n') {
			currentLine = trim(currentLine);

			pair<string, string> kv = getKeyValue(currentLine);
			string key = kv.first;
			string value = kv.second;

			if (!key.compare("id")) {
				sscanf(value.data(), "%d", &id);
			} else if (!key.compare("classname")) {
				sscanf(value.data(), "%s", classname);
			} else if (!key.compare("origin")) {
				sscanf(value.data(), "%d %d %d", &u, &v, &t);

				origin = MathVector(u, t, -v);
			} else if (key.compare("")) {
				char prop[128] = { '\0' };
				char content[128] = { '\0' };

				sscanf(key.data(), "%s", prop);
				sscanf(value.data(), "%d %d %d", &u, &v, &t);

				sprintf(content, "%d %d %d", u, v, t);
				keyValues.insert(kv);
				properties.push_back(pair<string, string> (string(prop), string(content)));
			}

			currentLine.clear();
		} else {
			currentLine.push_back(entityDesc[i]);
		}
	}

	string class_s = string(classname);
	cout << "class = " << class_s << endl;
	if (!class_s.compare("info_player_start")) {
		string angle = keyValues["angles"];
		int pitch, yawn, roll;
		sscanf(angle.data(), "%d %d %d", &pitch, &yawn, &roll);
		MathVector visee = MathVector(origin.getX(), origin.getY(), origin.getZ() - 30);

		OGLWrapper::setEye(origin);
		OGLWrapper::setVisee(visee);
	}
	return Entity(id, string(classname), origin, properties);
}

vector<Entity> VMFParser::parseEntity(string entityDesc) {
	vector<Entity> entities;

	string *s = getBloc(entityDesc, 0, "entity");

	if (s != NULL) {
		Entity e = getEntity(*s);
		e.print();

		entities.push_back(e);
	}

	return entities;
}

World VMFParser::parseWorld(string worldDesc) {
	vector<Solid> solids;

	int index_find = worldDesc.find("solid", 0);
	string *solid = getBloc(worldDesc, index_find, "solid");

	string type = "world";

	int currentSize = 0;
	while (solid != NULL) {
		currentSize += solid->size();
		ProgressBar::updateType(type, (double) currentSize / (double) worldDesc.size());
		ProgressBar::draw();
		Solid s = parseSolid(*solid);
		solids.push_back(s);
		index_find += solid->size();

		solid = getBloc(worldDesc, index_find, "solid");
	}

	return World(solids);
}

pair<string, string> VMFParser::getKeyValue(string keyValue) {
	char key[keyValue.size()]; // obligé, trop pratique sscanf!
	char value[keyValue.size()];

	// On veut parser : "clef" "valeur"
	sscanf(keyValue.data(), "\"%[^\"]\" \"%[^\"]\"", key, value); // miam
	return pair<string, string>(string(key), string(value));
}
