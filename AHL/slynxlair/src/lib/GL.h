/*
 * GL.h
 *
 *  Created on: 10 oct. 2010
 *      Author: Pierre
 */

#ifndef GL_H_
#define GL_H_

#include <psgl/psgl.h>
#include <psgl/psglu.h>
#include <list>

using namespace std;

namespace PS3 {

class GL {
private:
	static unsigned int width;
	static unsigned int height;
public:
	static const int MAIN_LOOP_STOP = 0;

	GL();
	virtual ~GL();

	/**
	 * Initialise la librairie graphique de la PS3
	 */
	static void init();

	/**
	 * Execute la fonction passée en argument en boucle tant que le retour de la
	 * fonction est différent de MAIN_LOOP_STOP
	 */
	static void executeMainLoop(int (*)(void));

	/**
	 * Referme la librairie graphique proprement.
	 */
	static void close();

	/**
	 * Ajoute des fonctions à executer à la réception d'une interruption
	 */
	static void addUserCallback(void (*)(const uint64_t,
										 const uint64_t,
										 void*));
	static unsigned int getWidth();
	static unsigned int getHeight();

private:
	static list<void(*)(const uint64_t, const uint64_t, void*)> *callbacks;
	static PSGLdevice *device;
	static PSGLcontext *context;

	static bool systemExited;
	static bool libraryStarted;
	static void systemCallback(const uint64_t, const uint64_t, void*);
};

}

#endif /* GL_H_ */
