#pragma once

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Dependencies.hpp"
#include "ConfigurationFile.hpp"

// ************************************************************************** //
//                               Class                                		  //
// ************************************************************************** //

class Webserv
{

private:
	unsigned int port;
	int serverFD;
	int optval;
	unsigned int numberOfServers;

public:
	Webserv();
	Webserv(unsigned int _port, char *filename);
	Webserv(const Webserv &other);
	~Webserv(void);
	const Webserv &operator=(const Webserv &other);

	const int &getServerFD(void) const;
	const unsigned int &getPort(void) const;

	std::vector<ServerConfig> config;

	void countAndParseServer(char *filename);
};

/* ---------------------UTILS FUNCTIONS */
void processClientInput(const char *clientInput, int server_fd, int clientFD);
int setNonBlocking(int fd);
void serverListeningLoop(int server_fd);
std::vector<pollfd> initializePollFDSWithServerSocket(int serverFD);
void monitorSocketEvents(std::vector<pollfd> &fds, int serverFD);
struct pollfd setNewTempFDStruct(int newSocket);
void processConnectionData(int serverFD, std::vector<pollfd> &fds, size_t &i);

#endif