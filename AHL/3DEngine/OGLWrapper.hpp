#ifndef OGLWRAPER_HPP_
#define OGLWRAPER_HPP_

#include <math.h>
#include <time.h>

#include <list>
#include <vector>
#include <map>
#include <string>

#include <SDL/SDL.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL/SDL_image.h>
#include <GL/glext.h>

#include "BSPTree.hpp"
#include "Structs.hpp"
#include "Polygon.hpp"

using namespace std;

class OGLWrapper {
private :
	static BSPTree *tree;
	static vector<Polygon> polygons; // Syntaxe a chier, >> etant opérateur!!
	static map<MathVector, pair<int, int> > texCoords;
	static map<string, GLuint> images;
	static map<string, pair<int, int> > imageSizes;
	static MathVector eye;
	static MathVector visee;
	static GLfloat light_pos[4];
	static GLfloat ambiant_int[4];
	static GLfloat specular_int[4];
	static GLfloat diffuse_int[4];
	static float frustum[6][4];
	static string skyPath[];

public :
	// INITIALISATIONS
	static void init_GL();
	static void init_SDL();
	static void init_Textures();
	static void setupLight();

	// ENREGISTREMENTS
	static void registerPolygon(Polygon&);
	static void register_texture(string path);
	static void setCubeMap(string);
	static void setVisee(const MathVector&);
	static void setEye(const MathVector&);

	// ACTIVATIONS
	static void activate_texture(string path);
	static void lookAt(MathVector eye, MathVector visee);
	static void drawSkyBox();

	// TERMINAISON
	static void kill_SDL();

	// OUTILS
	static void display_loop();
	static void buildBSPTree();
	static pair<int, int> getBounds(string path);
	static bool shouldDisplay(Polygon*);
	static int boxIsInFrustum(BoundingBox* b);
	static long getCurrentMilli();

	// GL EXTENSIONS
	// pas le choix, les extensions puent
	static PFNGLGENQUERIESARBPROC glGenQueriesARB;
	static PFNGLDELETEQUERIESARBPROC glDeleteQueriesARB;
	static PFNGLBEGINQUERYARBPROC glBeginQueryARB;
	static PFNGLENDQUERYARBPROC glEndQueryARB;
	static PFNGLGETQUERYOBJECTIVARBPROC glGetQueryObjectivARB;
private :
	static bool PointInFrustum(const MathVector&);
	static void extractFrustum();
	static void display_scene();
	static int keyboard_manager(SDL_Event * event);
};

#endif /* OGLWRAPER_HPP_ */
