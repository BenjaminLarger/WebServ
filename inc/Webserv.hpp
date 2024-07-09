#pragma once

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Dependencies.hpp"
#include "ConfigurationFile.hpp"

// ************************************************************************** //
//                               Class                                		  //
// ************************************************************************** //

class Webserv : public ServerConfig
{

private:
	const char *filename;
	ServerConfig config;
	unsigned int port;
	int serverFD;
	int optval;
	unsigned int numberOfServers;

public:
	Webserv();
	Webserv(const char *filename, ServerConfig config);
	~Webserv(void);
	const Webserv &operator=(const Webserv &other);

	const int &getServerFD(void) const;
	const unsigned int &getPort(void) const;

	int setNonBlocking(int fd);
	void serverListeningLoop(int serverFD);
	std::vector<pollfd> initializePollFDSWithServerSocket(int serverFD);
	void monitorSocketEvents(std::vector<pollfd> &fds, int serverFD);
	struct pollfd setNewTempFDStruct(int newSocket);
	void processConnectionData(int serverFD, std::vector<pollfd> &fds, size_t &i);
	void processClientInput(const char *clientInput, int serverFD, int clientFD);

	// std::vector<ServerConfig> config;

	void countAndParseServer(const char *filename);
};

/* ---------------------UTILS FUNCTIONS */

#endif