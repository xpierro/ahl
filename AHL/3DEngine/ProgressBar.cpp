#include "ProgressBar.hpp"
#include "OGLWrapper.hpp"
#include "SDL/SDL_ttf.h"
#include <iostream>

string tex = "misc/progressbar";

TTF_Font* ProgressBar::police;
GLuint ProgressBar::texId;

const float ProgressBar::tWhite[] = { 1, 1, 1 };
const float ProgressBar::tBorder[] = { 1, 0.6, 0.5 };

const float ProgressBar::fBar = 1. / 8.;
const float ProgressBar::fBorder = fBar + 1. / 60.;

vector<string> ProgressBar::vTypes;
map<string, double> ProgressBar::mTypesPercents;

float ProgressBar::baseCoordX = -.9;
float ProgressBar::baseCoordY = -fBorder;
const float ProgressBar::spaceBetweenTypes = -.1;
map<string, pair<float, float> > ProgressBar::mTypesCoords;

void ProgressBar::create() {
	OGLWrapper::register_texture(tex);
	TTF_Init();
	glGenTextures(1, &texId);
	police = TTF_OpenFont("PBAR.TTF", 64);
}

void ProgressBar::remove() {
	TTF_CloseFont(police);
	TTF_Quit();
}

bool ProgressBar::contains(vector<string> vect, string element) {
	for (size_t i = 0; i < vect.size(); i++) {
		if (vect[i] == element) {
			return true;
		}
	}

	return false;
}

void ProgressBar::registerType(string type) {
	if (!contains(vTypes, type)) {
		vTypes.push_back(type);

		baseCoordY += spaceBetweenTypes;
		mTypesCoords[type] = pair<float, float>(baseCoordX, baseCoordY);
		mTypesPercents[type] = 0.;
	}
}

pair<float,float> ProgressBar::getCoords(string type) {
	if (!contains(vTypes, type)) {
		return mTypesCoords[type];
	}

	return make_pair(0., 0.); // ? o/
}

double ProgressBar::getPercent(string type) {
	if (!contains(vTypes, type)) {
		return mTypesPercents[type];
	}

	return 0.;
}

/*
void ProgressBar::drawGlobal(double percent) {
	char glob[7] = "global";
	char empty[1] = "";

	drawSpecific(percent, glob, empty, allx, globy);
}

void ProgressBar::drawShit(double percent, char *spec) {
	char shit[5] = "shit";

	drawSpecific(percent, shit, spec, allx, shity);
}
*/

int puissance2sup(int i) {
    double logbase2 = log(i) / log(2);
    return (int) floor(pow(2.0, ceil(logbase2)));
}

void ProgressBar::drawSpecific(const char *type, const char *spec, double percent, float x, float y) {
	int w, h;

	glEnable(GL_TEXTURE_2D);
	SDL_Color color = { 120, 120, 120 };
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	char percent_c[128];
	if (strcmp(spec, "") == 0) {
		sprintf(percent_c, "%s %3d%%", type, (int) (percent * 100));
	} else {
		sprintf(percent_c, "%s %s %3d%%", type, spec, (int) (percent * 100));
	}

	SDL_Surface* texte = TTF_RenderText_Blended(police, percent_c, color);
    SDL_Surface* texte2 = NULL;
	if(texte != NULL) {
        //Calcul de taille
        w = puissance2sup(texte->w);
        h = puissance2sup(texte->h);
		//w = texte->w;
		//h = texte->h;

         texte2 = SDL_CreateRGBSurface(
			 SDL_HWSURFACE,
			 w,
			 h,
			 24,
			 texte->format->Rmask,
			 texte->format->Gmask,
			 texte->format->Bmask,
			 texte->format->Amask);
    }

	SDL_BlitSurface(texte, NULL, texte2, NULL);
	SDL_FreeSurface(texte);

	if (SDL_MUSTLOCK(texte2)) {
		SDL_LockSurface(texte2);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texte2->w, texte2->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texte2->pixels);

	if (SDL_MUSTLOCK(texte2)) {
		SDL_UnlockSurface(texte2);
	}

	SDL_FreeSurface(texte2);

	float qw = (float) texte2->w / (float) 1024;
	float qh = (float) texte2->h / (float) 1024;

	glBegin(GL_QUADS); {
		glTexCoord2f(0, 1); glVertex3f(x     , y - qh, 0);
		glTexCoord2f(1, 1); glVertex3f(x + qw, y - qh, 0);
		glTexCoord2f(1, 0); glVertex3f(x + qw, y     , 0);
		glTexCoord2f(0, 0); glVertex3f(x     , y     , 0);
	} glEnd();

	glDeleteTextures(1, &texId);
	glDisable(GL_TEXTURE_2D);
}

void ProgressBar::drawGlobal(double percent) {
	double step = percent * 2 - 1;



	glColor3f(tBorder[0], tBorder[1], tBorder[2]);

	glBegin(GL_QUADS);
	glVertex3d(-1., fBar, 0.);
	glVertex3d( 1., fBar, 0.);
	glVertex3d( 1., fBorder , 0.);
	glVertex3d(-1., fBorder , 0.);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	OGLWrapper::activate_texture(tex);

	glColor3f(tWhite[0], tWhite[1], tWhite[2]);

	glBegin(GL_QUADS);
	glTexCoord2d(0., 0.); glVertex3d(-1.  , -fBar, 0.);
	glTexCoord2d(1., 0.); glVertex3d( step, -fBar, 0.);
	glTexCoord2d(1., 1.); glVertex3d( step,  fBar, 0.);
	glTexCoord2d(0., 1.); glVertex3d(-1.  ,  fBar, 0.);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glColor3f(tBorder[0], tBorder[1], tBorder[2]);

	glBegin(GL_QUADS);
	glVertex3d(-1., -fBorder , 0.);
	glVertex3d( 1., -fBorder , 0.);
	glVertex3d( 1., -fBar, 0.);
	glVertex3d(-1., -fBar, 0.);
	glEnd();
}

void ProgressBar::updateType(string type, double percent) {
	if (contains(vTypes, type)) {
		mTypesPercents[type] = percent;
	}
}

void ProgressBar::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double x, y, p;
	string current;
	double sum = 0;
	size_t size = vTypes.size();
	for (size_t i = 0; i < size; i++) {
		current = vTypes[i];

		p = mTypesPercents[current];
		sum += p;

		x = mTypesCoords[current].first;
		y = mTypesCoords[current].second;

		// TODO une map pour les specs en cours o////
		drawSpecific(current.c_str(), "", p, x, y);
	}

	double percent = 0;
	if (size != 0) {
		percent = sum / (double) size;
	}

	drawGlobal(percent);

	SDL_GL_SwapBuffers();

    GLenum erreur = glGetError();
    if (erreur != GL_NO_ERROR) {
        fprintf(
            stderr,
            "Erreur OpenGL dans display : %s\n",
            gluErrorString(erreur));
    }
}
