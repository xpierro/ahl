/*
 * Socket.cpp
 *
 *  Created on: 15 oct. 2010
 *      Author: Pierre
 */

#include "Socket.h"

#include "../GL.h"

#include <cell/sysmodule.h>

namespace PS3 {

bool Socket::netInitialized = false;

Socket::Socket(string host, short port) {
	initSocket();
	struct hostent *h = gethostbyname(host.c_str());
	memset(&addr, 0, sizeof(addr));
	((struct sockaddr_in*)&addr)->sin_family = AF_INET;
	((struct sockaddr_in*)&addr)->sin_addr.s_addr = *((unsigned long*) h->h_addr_list[0]);
	((struct sockaddr_in*)&addr)->sin_port = htons(port);
	// no PF_INET declaration...
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

Socket::~Socket() {
	// TODO Auto-generated destructor stub
}

static void netCallback(uint64_t status, uint64_t params, void* userdata) {
	(void) params;
	(void) userdata;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		Socket::closeSocket();
	}
}

void Socket::initSocket() {
	if (!netInitialized) {
		cellSysmoduleLoadModule(CELL_SYSMODULE_NET);
		sys_net_initialize_network();
		netInitialized = true;
		GL::addUserCallback(netCallback);
	}
}

//!\ Blocking, use select or threading in the future.
int Socket::readDataLine(char* buffer, int bufferLength) {
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

void Socket::closeSocket() {
	sys_net_finalize_network();
}

int Socket::connectToHost() {
	return connect(sock, &addr, sizeof(struct sockaddr_in));
}

}
