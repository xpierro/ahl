#include "OGLWrapper.hpp"
#include <sys/timeb.h>
#include <IL/devil_cpp_wrapper.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

MathVector OGLWrapper::eye(0, 0.01, 0);
MathVector OGLWrapper::visee(0, 0.01, -0.01);
GLfloat OGLWrapper::light_pos[4] = {-2048, -2048, 0, 1};
GLfloat OGLWrapper::ambiant_int[4] = {1, 1, 1, 1};
GLfloat OGLWrapper::specular_int[4] = {1, 1, 1, 1};
GLfloat OGLWrapper::diffuse_int[4] = {1, 1, 1, 1};
float OGLWrapper::frustum[6][4];
BSPTree* OGLWrapper::tree;
map<string, GLuint> OGLWrapper::images;
vector<Polygon> OGLWrapper::polygons;
map<MathVector, pair<int, int> > OGLWrapper::texCoords;
map<string, pair<int, int> > OGLWrapper::imageSizes;

material laiton = {100, {0.7, 0.7, 0.7, 1},
                           {0.7, 0.7, 0.7, 1},
                           {0.7, 0.7, 0.7, 1}};


GLuint cube_map_texture_ID;

PFNGLGENQUERIESARBPROC OGLWrapper::glGenQueriesARB = NULL;
PFNGLDELETEQUERIESARBPROC OGLWrapper::glDeleteQueriesARB = NULL;
PFNGLBEGINQUERYARBPROC OGLWrapper::glBeginQueryARB = NULL;
PFNGLENDQUERYARBPROC OGLWrapper::glEndQueryARB = NULL;
PFNGLGETQUERYOBJECTIVARBPROC OGLWrapper::glGetQueryObjectivARB = NULL;

// INITIALISATIONS

void OGLWrapper::init_SDL() {
    SDL_Init(SDL_INIT_VIDEO);

    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == -1) {
        fprintf(stderr, "Impossible d'activer le double buffer\n");
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("VMF Viewer",NULL);

    if (SDL_SetVideoMode(640, 640, 32, SDL_OPENGL | SDL_RESIZABLE) == NULL) {
        fprintf(stderr, "Impossible de passer en mode OpenGL : %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    glGenQueriesARB = (PFNGLGENQUERIESARBPROC) SDL_GL_GetProcAddress("glGenQueriesARB");
    glDeleteQueriesARB = (PFNGLDELETEQUERIESARBPROC) SDL_GL_GetProcAddress("glDeleteQueriesARB");
    glBeginQueryARB = (PFNGLBEGINQUERYARBPROC) SDL_GL_GetProcAddress("glBeginQueryARB");
    glEndQueryARB = (PFNGLENDQUERYARBPROC) SDL_GL_GetProcAddress("glEndQueryARB");
    glGetQueryObjectivARB = (PFNGLGETQUERYOBJECTIVARBPROC) SDL_GL_GetProcAddress("glGetQueryObjectivARB");
}

void OGLWrapper::init_GL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(60, 1, 0.01, 200);
    lookAt(eye, visee);
}

void OGLWrapper::init_Textures() {
	ilInit();
	iluInit();
	ilutInit();
	ilEnable(IL_CONV_PAL);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilEnable(IL_ORIGIN_SET);

	ilutEnable(ILUT_OPENGL_CONV);
	ilutRenderer(ILUT_OPENGL);
}

void OGLWrapper::setupLight() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat t[4] = {0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, t);

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiant_int);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_int);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_int);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(laiton.shininess));
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, laiton.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, laiton.specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, laiton.diffuse);
}



void OGLWrapper::setEye(const MathVector& eye_n) {
	eye = MathVector(eye_n.getX() * 0.0005, eye_n.getY() * 0.0005, eye_n.getZ() * 0.0005);
}

void OGLWrapper::setVisee(const MathVector& visee_n) {
	visee = MathVector(visee_n.getX() * 0.0005, visee_n.getY() * 0.0005, visee_n.getZ() * 0.0005);
}

void OGLWrapper::extractFrustum()
{
   float   proj[16];
   float   modl[16];
   float   clip[16];
   float   t;

   /* Get the current PROJECTION matrix from OpenGL */
   glGetFloatv( GL_PROJECTION_MATRIX, proj );

   /* Get the current MODELVIEW matrix from OpenGL */
   glGetFloatv( GL_MODELVIEW_MATRIX, modl );

   /* Combine the two matrices (multiply projection by modelview) */
   clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
   clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
   clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
   clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

   clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
   clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
   clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
   clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

   clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
   clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
   clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
   clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

   clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
   clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
   clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
   clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

   /* Extract the numbers for the RIGHT plane */
   frustum[0][0] = clip[ 3] - clip[ 0];
   frustum[0][1] = clip[ 7] - clip[ 4];
   frustum[0][2] = clip[11] - clip[ 8];
   frustum[0][3] = clip[15] - clip[12];

   /* Normalize the result */
   t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
   frustum[0][0] /= t;
   frustum[0][1] /= t;
   frustum[0][2] /= t;
   frustum[0][3] /= t;

   /* Extract the numbers for the LEFT plane */
   frustum[1][0] = clip[ 3] + clip[ 0];
   frustum[1][1] = clip[ 7] + clip[ 4];
   frustum[1][2] = clip[11] + clip[ 8];
   frustum[1][3] = clip[15] + clip[12];

   /* Normalize the result */
   t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
   frustum[1][0] /= t;
   frustum[1][1] /= t;
   frustum[1][2] /= t;
   frustum[1][3] /= t;

   /* Extract the BOTTOM plane */
   frustum[2][0] = clip[ 3] + clip[ 1];
   frustum[2][1] = clip[ 7] + clip[ 5];
   frustum[2][2] = clip[11] + clip[ 9];
   frustum[2][3] = clip[15] + clip[13];

   /* Normalize the result */
   t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
   frustum[2][0] /= t;
   frustum[2][1] /= t;
   frustum[2][2] /= t;
   frustum[2][3] /= t;

   /* Extract the TOP plane */
   frustum[3][0] = clip[ 3] - clip[ 1];
   frustum[3][1] = clip[ 7] - clip[ 5];
   frustum[3][2] = clip[11] - clip[ 9];
   frustum[3][3] = clip[15] - clip[13];

   /* Normalize the result */
   t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
   frustum[3][0] /= t;
   frustum[3][1] /= t;
   frustum[3][2] /= t;
   frustum[3][3] /= t;

   /* Extract the FAR plane */
   frustum[4][0] = clip[ 3] - clip[ 2];
   frustum[4][1] = clip[ 7] - clip[ 6];
   frustum[4][2] = clip[11] - clip[10];
   frustum[4][3] = clip[15] - clip[14];

   /* Normalize the result */
   t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
   frustum[4][0] /= t;
   frustum[4][1] /= t;
   frustum[4][2] /= t;
   frustum[4][3] /= t;

   /* Extract the NEAR plane */
   frustum[5][0] = clip[ 3] + clip[ 2];
   frustum[5][1] = clip[ 7] + clip[ 6];
   frustum[5][2] = clip[11] + clip[10];
   frustum[5][3] = clip[15] + clip[14];

   /* Normalize the result */
   t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
   frustum[5][0] /= t;
   frustum[5][1] /= t;
   frustum[5][2] /= t;
   frustum[5][3] /= t;

}

bool OGLWrapper::PointInFrustum(const MathVector &v) {
   int p;
   for( p = 0; p < 6; p++ ) {
      if( frustum[p][0] * v.getX() + frustum[p][1] * v.getY() + frustum[p][2] * v.getZ() + frustum[p][3] < -0.01 ) {
         return false;
      }
   }
   return true;
}

// MEGAA OPTIMISATION: seuls deux points d'une bounding box sont interessants!!
// Inspiré de : http://www.lighthouse3d.com/opengl/viewfrustum/index.php?gatest3
int OGLWrapper::boxIsInFrustum(BoundingBox* b) {
	int result = IN;
	for(int p = 0; p < 6; p++) {
		MathVector plNormal(frustum[p][0] / 0.0005, frustum[p][1] / 0.0005, frustum[p][2] / 0.0005);
		MathVector pV = b->getPVertex(plNormal);
		double classification =
				  frustum[p][0] / 0.0005 * pV.getX()
				+ frustum[p][1] / 0.0005 * pV.getY()
				+ frustum[p][2] / 0.0005 * pV.getZ()
				+ frustum[p][3] / 0.0005;
		if (classification < - 0.01) {
			return OUT;
		} else {
			MathVector nV = b->getNVertex(plNormal);
			classification =
				  frustum[p][0] / 0.0005 * nV.getX()
				+ frustum[p][1] / 0.0005 * nV.getY()
				+ frustum[p][2] / 0.0005 * nV.getZ()
				+ frustum[p][3] / 0.0005;
			if (classification < - 0.01) {
				result = SPANNING;
			}
		}
	 }
	return(result);
}

// culling
bool OGLWrapper::shouldDisplay(Polygon* p) {
	const vector<MathVector>& vertices = p->getVertices();
	for (size_t i = 0; i < vertices.size(); i++) {
		if(PointInFrustum(vertices[i])) {
			return true;
		}
	}
	if (PointInFrustum(p->getCenter())) {
		return true;
	}
	return false;
}

void OGLWrapper::lookAt(MathVector eye, MathVector visee) {
	GLenum matrixMode;
	glGetIntegerv(GL_MATRIX_MODE, (GLint*)&matrixMode);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    gluLookAt(eye.getX(), eye.getY(), eye.getZ(), visee.getX(), visee.getY(), visee.getZ(), 0, 1, 0);
    glScaled(0.0005, 0.0005, 0.0005);
    glMatrixMode(matrixMode);
    extractFrustum();
}

GLuint loadTexture(string path) {
	vector<string> exts;
	exts.push_back(".bmp");
	exts.push_back(".tga");
	exts.push_back(".png");
	exts.push_back(".jpg");
	exts.push_back(".vtf");

	for (size_t i = 0; i < exts.size(); i++) {
		string pathext = path;
		pathext.append(exts[i]);
		if(ilLoadImage(pathext.data())) { // SINON LOAD PLUSIEURS FOIS LA MEME TEXTURE TODO : faire ailleurs
			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);
			if (ImageInfo.Origin != IL_ORIGIN_UPPER_LEFT) {
				iluFlipImage();
			}
			if (!iluBuildMipmaps()) {
				cout << "Mipmaps impossibles à générer" << endl;
			}
			GLuint tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			gluBuild2DMipmaps(GL_TEXTURE_2D, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT) ,GL_UNSIGNED_BYTE, ilGetData());

			return tex;
		}
	}
	cout << "Impossible de charger : " << path << endl;
	return 0;
}

pair<int, int> OGLWrapper::getBounds(string path) {
	if (imageSizes.count(path) != 0) {
		return imageSizes[path];
	} else {
		return pair<int, int>(0, 0);
	}
}

void OGLWrapper::register_texture(string path) {
	if (images.count(path) == 0) {
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLuint tex = loadTexture(path);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		imageSizes.insert(pair<string, pair<int, int> >(path, pair<int, int>(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT))));
		images.insert(pair<string, GLuint>(path, tex));
	}
}

void OGLWrapper::activate_texture(string path) {
	if (images.count(path) != 0) {
		glBindTexture(GL_TEXTURE_2D, images[path]);
	}
}

void OGLWrapper::kill_SDL() {
	SDL_Quit();
}

void OGLWrapper::registerPolygon(Polygon& polygon) {
	polygons.push_back(polygon);
}

long OGLWrapper::getCurrentMilli() {
	struct timeb t;
	ftime(&t);
    return t.time * 1000 + t.millitm;
}

void OGLWrapper::display_loop() {
	int continuer = 1;
	SDL_Event event;
	lookAt(eye, visee);
	display_scene();
	// Boucle traitant les évènements de la fenetre
	while (continuer) {
		// On attend le prochain évènement
	    SDL_PollEvent(&event);
	    // On traite l'évènement
	    switch (event.type) {
	    case SDL_QUIT:
	    	// On a fermé la fenetre
	    	continuer = 0;
	    	break;
	    case SDL_KEYDOWN:
	    	// On a appuyé sur une touche
	    	continuer = keyboard_manager(&event);
	    	lookAt(eye, visee);
	    	display_scene();
	    	break;
	    case SDL_VIDEORESIZE :
	        //rescale(event.resize.w, event.resize.h);
	        break;
	    }

	  }

}

void OGLWrapper::display_scene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSkyBox();
	MathVector eye_r = eye / 0.0005;
	MathVector visee_r = visee / 0.0005;
	tree->draw(eye_r, visee_r, true);
/*
	for (size_t i = 0; i < polygons.size(); i++) {
		if (shouldDisplay(&(polygons[i])))
			polygons[i].draw();
	}
*/
	SDL_GL_SwapBuffers();
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		fprintf(stderr, "%s...\n", gluErrorString(error));
	}
}

string OGLWrapper::skyPath[6];

const int INFRONT = 0;
const int BACK = 1;
const int UP = 2;
const int DOWN = 3;
const int LEFT = 4;
const int RIGHT = 5;

void OGLWrapper::setCubeMap(string path) {
	skyPath[UP] = path; skyPath[UP].append("up");
	skyPath[DOWN] = path; skyPath[DOWN].append("dn");
	skyPath[INFRONT] = path; skyPath[INFRONT].append("ft");
	skyPath[BACK] = path; skyPath[BACK].append("bk");
	skyPath[LEFT] = path; skyPath[LEFT].append("lf");
	skyPath[RIGHT] = path; skyPath[RIGHT].append("rt");

	for (int i = 0; i < 6; i++) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		register_texture(skyPath[i]);
	}
}

// utilise eye pr se positionner!
void OGLWrapper::drawSkyBox() {
	glMatrixMode(GL_MODELVIEW);
	// Store the current matrix
    glPushMatrix();

    // Reset and transform the matrix.
    glLoadIdentity();
    gluLookAt(
        eye.getX(), eye.getY(),eye.getZ(),
        visee.getX(), visee.getY(), visee.getZ(),
        0, 1, 0);

    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);

    double m = 0.5 / 0.005;    // Restore enable bits and matrix

    activate_texture(skyPath[BACK]);
    glBegin(GL_QUADS);
   	glTexCoord2f(1, 0); glVertex3f(-m, m, m);
   	glTexCoord2f(0, 0); glVertex3f(m, m, m);
   	glTexCoord2f(0, 1); glVertex3f(m, -m, m);
   	glTexCoord2f(1, 1); glVertex3f(-m, -m, m);
    glEnd();

    activate_texture(skyPath[INFRONT]);
    glBegin(GL_QUADS);
   	glTexCoord2f(0, 1); glVertex3f(-m , -m, -m);
   	glTexCoord2f(1, 1); glVertex3f(m, -m, -m);
   	glTexCoord2f(1, 0); glVertex3f(m, m, -m);
   	glTexCoord2f(0, 0); glVertex3f(-m, m, -m);
    glEnd();

    /*
    activate_texture(skyPath[DOWN]);
    glBegin(GL_TRIANGLE_STRIP);
   	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z);
   	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y,	z + length);
   	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,z);
   	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,z + length);
    glEnd();
*/
    glPushMatrix();

    activate_texture(skyPath[UP]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-m, m, m);
   	glTexCoord2f(1, 0); glVertex3f(-m, m, -m);
   	glTexCoord2f(1, -1); glVertex3f(m, m, -m);
   	glTexCoord2f(0, -1); glVertex3f(m, m, m);
    glEnd();
    glPopMatrix();

    // On my left
    activate_texture(skyPath[RIGHT]);
    glBegin(GL_QUADS);
   	glTexCoord2f(0, 1); glVertex3f(-m, -m, m);
   	glTexCoord2f(1, 1); glVertex3f(-m, -m, -m);
   	glTexCoord2f(1, 0); glVertex3f(-m, m, -m);
   	glTexCoord2f(0, 0); glVertex3f(-m, m, m);
    glEnd();

    activate_texture(skyPath[LEFT]);
    glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(m, m, -m);
	glTexCoord2f(0, 1); glVertex3f(m, -m, -m);
	glTexCoord2f(1, 1); glVertex3f(m, -m, m);
	glTexCoord2f(1, 0); glVertex3f(m, m, m);
	glEnd(); // 01 00 10 11

    glPopAttrib();
    glPopMatrix();
}


int OGLWrapper::keyboard_manager(SDL_Event * event) {
	if (event->type == SDL_KEYDOWN) {
		MathVector Vd(eye, visee);
		switch(event->key.keysym.sym) {
	    case SDLK_ESCAPE:
	    	return 0;
	    case SDLK_z:
	    	eye = eye + Vd;
	    	visee = visee + Vd;
	        break;
	    case SDLK_s:
	    	eye = eye - Vd;
	    	visee = visee - Vd;
	        break;
	    case SDLK_q :
	    	eye -= Vd.vectP(MathVector(0, 1, 0));
	    	visee -= Vd.vectP(MathVector(0, 1, 0));
	        break;
	    case SDLK_d :
	    	eye += Vd.vectP(MathVector(0, 1, 0));
	    	visee += Vd.vectP(MathVector(0, 1, 0));
	        break;
	    case SDLK_RIGHT :
	    	visee = eye + Vd.rotate(-0.1, MathVector(0, 1, 0));
	        break;
	    case SDLK_LEFT :
	    	visee = eye + Vd.rotate(0.1, MathVector(0, 1, 0));
	    	break;
	    case SDLK_DOWN :
	    	visee = eye + Vd.rotate(0.1, Vd.vectP(MathVector(0, 1, 0)).getNormalized());
	    	break;
	    case SDLK_UP :
	    	visee = eye + Vd.rotate(-0.1, Vd.vectP(MathVector(0, 1, 0)).getNormalized());
	    	break;
	    default:
	    	break;
	    }
	}
	return 1;
}



void OGLWrapper::buildBSPTree() {
	vector<Polygon*> polygons_p;
	for (size_t i = 0; i < polygons.size(); i++) {
		polygons_p.push_back(&polygons[i]);
	}
	tree = BSPTree::buildBSPTree(polygons_p);
}
