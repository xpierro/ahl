/*
 * Socket.cpp
 *
 *  Created on: 15 oct. 2010
 *      Author: Pierre
 */

#include "Socket.h"

#include "../graphic/GL.h"

#include <cell/sysmodule.h>

namespace PS3 {

bool Socket::netInitialized = false;
list<Socket*>* Socket::sockets = new list<Socket*>();

Socket::Socket(string host, short port) {
	initSockets();
	connectable = false;
	connected = false;
	remoteHost = host;
	remotePort = port;
	sockets->push_back(this);
}

bool Socket::createBSDSocket(string& host, int port) {
	if(port < 0) {
		return false;
	}
	if (host.size() <= 0) {
		return false;
	}
	remoteHost = host;
	remotePort = port;
	struct hostent *h = gethostbyname(host.c_str());
	if (h == NULL) {
		return false;
	}
	memset(&addr, 0, sizeof(addr));
	((struct sockaddr_in*)&addr)->sin_family = AF_INET;
	((struct sockaddr_in*)&addr)->sin_addr.s_addr =
										*((unsigned long*) h->h_addr_list[0]);
	((struct sockaddr_in*)&addr)->sin_port = htons(port);
	// no PF_INET declaration...
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return true;
}

Socket::~Socket() { }

static void netCallback(uint64_t status, uint64_t params, void* userdata) {
	(void) params;
	(void) userdata;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		Socket::closeSockets();
	}
}

void Socket::initSockets() {
	if (!netInitialized) {
		cellSysmoduleLoadModule(CELL_SYSMODULE_NET);
		sys_net_initialize_network();
		netInitialized = true;
		GL::addUserCallback(netCallback);
	}
}

//!\ Blocking, use select or threading in the future.
int Socket::readDataLine(char* buffer, int bufferLength) {
	if (!connected || !connectable) {
		return 0;
	}
	char c = '\0';
	int i = 0;
	int byteReceived = 1;
	// Read one by one, to stop at \n
	while (c != '\n' && i < bufferLength && byteReceived > 0) {
		byteReceived = recv(sock, &c, 1, MSG_DONTWAIT);
		if (byteReceived > 0) {
			buffer[i] = c;
			i += 1;
		}
	}
	buffer[i] = '\0';
	return i;
}

int Socket::writeDataChunk(char* chunk, int chunkSize) {
	return send(sock, chunk, chunkSize, MSG_DONTWAIT);
}

void Socket::closeSockets() {
	list<Socket*>::iterator it = sockets->begin();
	while(it != sockets->end()) {
		(*it)->close();
		++it;
	}
	sys_net_finalize_network();
}

bool Socket::connectToHost() {
	if (!connectable) {
		connectable = createBSDSocket(remoteHost, remotePort);
	}
	if (!connectable) {
		return false;
	}
	if (!connected) {
		connected = 0 == connect(sock, &addr, sizeof(struct sockaddr_in));
	}
	return connected;

}

void Socket::close() {
	if (connected || connectable ) {
		socketclose(sock);
	}
}

}
