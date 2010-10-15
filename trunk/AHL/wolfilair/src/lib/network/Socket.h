/*
 * Socket.h
 *
 *  Created on: 15 oct. 2010
 *      Author: Pierre
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <arpa/inet.h>
#include <netdb.h>
#include <netex/net.h>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

namespace PS3 {

/**
 * Java-style socket class to handle simple network interactions.
 */
class Socket {
private:
	static bool netInitialized;
	int sock; // Socket descriptor
	struct sockaddr addr; // Informations about the host we want to connect to
public:
	Socket(string host, short port);
	virtual ~Socket();
	int connectToHost();
	void readDataLine(char* buffer, int bufferLength);

	static void initSocket();
	static void closeSocket();
};

}

#endif /* SOCKET_H_ */
