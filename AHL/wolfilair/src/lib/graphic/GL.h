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
protected:
	static unsigned int width;
	static unsigned int height;
public:
	static const int MAIN_LOOP_STOP = 0;

	GL();
	virtual ~GL();

	/**
	 * Initialises the ps3 graphic context
	 */
	static void init();

	/**
	 * Executes the given parameter before the loop.
	 */
	virtual void executeBeforeLoop(void (*)(void));

	/**
	 * Executes the given parameter in loop as long as the return value isn't
	 * MAIN_LOOP_STOP
	 */
	virtual void executeMainLoop(int (*)(void));

	/**
	 * Quits the implementation cleanly.
	 */
	virtual void close();

	/**
	 * Adds callback functions launched when the system callback is executed
	 */
	static void addUserCallback(void(*)(const uint64_t, const uint64_t, void*));

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
