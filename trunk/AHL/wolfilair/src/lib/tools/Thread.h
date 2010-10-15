/*
 * Thread.h
 *
 *  Created on: 13 oct. 2010
 *      Author: Pierre
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

namespace PS3 {

class Thread {
private:
	pthread_t threadId;
	bool _threadStopped;

public:
	Thread();
	virtual ~Thread();

	/**
	 * Démarre le Thread
	 */
	void start();

	/**
	 * Termine le thread
	 */
	void stop();

	/**
	 * Fonction executée après le démarrage du Thread
	 */
	virtual void execute() = 0;

	bool threadStopped();
};

}

#endif /* THREAD_H_ */
