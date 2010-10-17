/*
 * GL.cpp
 *
 *  Created on: 10 oct. 2010
 *      Author: Pierre
 */

#include "GL.h"

#include <cell/sysmodule.h>
#include <stdio.h>
#include <sys/spu_initialize.h>

#include<list>

using namespace std;

namespace PS3 {

GL::GL() { }

GL::~GL() { }

bool GL::systemExited = false;
bool GL::libraryStarted = false;
PSGLdevice* GL::device = NULL;
PSGLcontext* GL::context = NULL;
unsigned int GL::width;
unsigned int GL::height;

list<void(*)(const uint64_t, const uint64_t, void*)>* GL::callbacks =
		new list<void(*)(const uint64_t, const uint64_t, void*)>();

/**
 * Fonction appel�e lorsque l'utilisateur veut quitter le programme.
 */
void GL::systemCallback(const uint64_t status, const uint64_t param,
						   void *userdata) {
	// On execute toutes les fonctions enregistr�es par l'utilisateur en r�ponse
	// � une interruption.
	list<void(*)(const uint64_t, const uint64_t, void*)>::iterator it =
			callbacks->begin();
	while(it != callbacks->end()) {
		(*it)(status, param, userdata);
		++it;
	}

	switch (status) {
	case CELL_SYSUTIL_REQUEST_EXITGAME: systemExited = true;
										break;
	case CELL_SYSUTIL_DRAWING_BEGIN:
	case CELL_SYSUTIL_DRAWING_END:
	default: break;
	}
}

void GL::init() {
	if (!libraryStarted) {
		// On demande 1 spu pour PSGL sur les 6 disponibles
		sys_spu_initialize(6, 1);

		// Met en pause la PS3 tant que la sortie HDMI n'est pas pr�te.
		CellVideoOutState videoState;
		do {
			cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);
		} while(videoState.state != CELL_VIDEO_OUT_OUTPUT_STATE_ENABLED );

		// On enregistre nos option d'initialisation.
		PSGLinitOptions initOpts;
		initOpts.enable = PSGL_INIT_MAX_SPUS | PSGL_INIT_INITIALIZE_SPUS;
		initOpts.maxSPUs = 1;
		initOpts.initializeSPUs = false;
		initOpts.persistentMemorySize = 0;
		initOpts.transientMemorySize = 0;
		initOpts.errorConsole = 0;
		initOpts.fifoSize = 0;
		initOpts.hostMemorySize = 128 * 1024 * 1024;  // 128 mb

		psglInit(&initOpts);


		device = psglCreateDeviceAuto(GL_ARGB_SCE,
				GL_DEPTH_COMPONENT24,
				GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE);
		psglGetDeviceDimensions(device, &width, &height);

		context = psglCreateContext();
		psglMakeCurrent(context, device);
		psglResetCurrentContext();

		// On initialise le Viewport

		// On cherche �  obtenir les dimensions effectives de dessin.
		GLuint renderWidth, renderHeight;
		psglGetRenderBufferDimensions(device, &renderWidth, &renderHeight);

		// On fixe la taille du Viewport
		glViewport(0, 0, renderWidth, renderHeight);

		// On met en place la projection orthogonale.
		GLfloat aspectRatio = psglGetDeviceAspectRatio(device);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspectivef(70.0, aspectRatio, 1., 1000.);

		glClearColor(0.f, 0.f, 0.f, 1.f);
		// Doesn't display back-facing polygons
		// (counter clock wise vertices)
		glDisable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		// Il faut effacer au moins une fois l'ecran.
		glClear(GL_COLOR_BUFFER_BIT
				| GL_DEPTH_BUFFER_BIT
				| GL_STENCIL_BUFFER_BIT);
		psglSwap();

		// On enregistre l'event handler sur le slot 0.
		cellSysutilRegisterCallback(0, systemCallback, NULL);
		libraryStarted = true;
	}
}

void GL::executeMainLoop(int (*loop)()) {
	init();
	while(!systemExited && loop() != MAIN_LOOP_STOP) {
		cellSysutilCheckCallback(); // We check for interruptions
	}
	close();
}

void GL::close() {
	// We unload the callback at slot 0
	cellSysutilUnregisterCallback(0);
	glFinish();
	psglDestroyContext(context);
	psglDestroyDevice(device);
	psglExit();
}

void GL::addUserCallback(void (*callback)(const uint64_t, const uint64_t, void*)) {
	callbacks->push_back(callback);
}

unsigned int GL::getWidth() {
	return width;
}

unsigned int GL::getHeight() {
	return height;
}

}
