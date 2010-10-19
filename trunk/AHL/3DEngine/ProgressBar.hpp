#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <math.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL/SDL_image.h>
#include <map>
#include <vector>
#include <string>

using namespace std;

class ProgressBar {
private:
	static TTF_Font *police;
	static GLuint texId;
	// Coordonnées des types
	static float baseCoordX;
	static float baseCoordY;
	static const float spaceBetweenTypes;
	static map<string, pair<float, float> > mTypesCoords;
	// Enregistrement interne
	static vector<string> vTypes;
	// Test o/
	static map<string, double> mTypesPercents;
	// Hihi
	static void drawSpecific(const char *type, const char *spec, double percent, float x, float y);
	static void drawGlobal(double percent);
	// Colors
	static const float tBorder[3];
	static const float tWhite[3];
	// Espacement barre principale
	static const float fBar;
	static const float fBorder;
	// JAVA CONTAINS?
	static bool contains(vector<string> vect, string element);
public:
	static void create();
	static void draw();
	static void remove();
	// Registre + infos
	static void registerType(string type);
	static pair<float,float> getCoords(string type);
	static double getPercent(string type);
	// Draws
	static void updateType(string type, double percent);
};

#endif /* PROGRESSBAR_H_ */
