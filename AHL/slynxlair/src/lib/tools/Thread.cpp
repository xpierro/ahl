/*
 * Thread.cpp
 *
 *  Created on: 13 oct. 2010
 *      Author: Pierre
 */

#include "Thread.h"

#include <pthread.h>

namespace PS3 {

Thread::Thread() {
	_threadStopped = false;
}

Thread::~Thread() { }

/**
 * Cette fonction convertit object en Thread et execute sa m�thode execute(),
 * cela permet d'utiliser la lib pthread avec des membres de classes C++
 */
static void* threadDispatcher(void* object) {
	Thread *t = static_cast<Thread*>(object);
	t->execute();
	return NULL;
}

void Thread::start() {
	pthread_create(&threadId, NULL, threadDispatcher, this);
}

void Thread::stop() {
	// Cette variable signalera � la boucle principale du thread qu'elle doit
	// s'arr�ter
	_threadStopped = true;
	// On attend l'arr�t...
	pthread_join(threadId, NULL);

}

bool Thread::threadStopped() {
	return _threadStopped;
}

}
