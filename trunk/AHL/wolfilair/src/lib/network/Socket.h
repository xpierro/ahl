/*
 * Socket.h
 *
 *  Created on: 15 oct. 2010
 *      Author: Pierre
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <arpa/inet.h>
#include <list>
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
	static list<Socket*>* sockets;
	int sock; // Socket descriptor
	struct sockaddr addr; // Informations about the host we want to connect to
	bool connectable;
	bool connected;
	string remoteHost;
	int remotePort;
public:
	Socket(string host, short port);
	virtual ~Socket();

	/**
	 * Connects the socket to the remote host.
	 */
	bool connectToHost();

	/**
	 * Read all data until a newLine char has been met.
	 */
	int readDataLine(char*, int);

	/**
	 * Send a data chunk to the remote host, too low level.
	 */
	int writeDataChunk(char*, int);

	/**
	 * Closes the socket
	 */
	void close();

	static void initSockets();
	static void closeSockets();
private:
	bool createBSDSocket(string& host, int port);
};

}

#endif /* SOCKET_H_ */
