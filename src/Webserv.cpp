/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:06:22 by blarger           #+#    #+#             */
/*   Updated: 2024/07/08 14:11:09 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"

void Webserv::countServer(char *filename)
{
	std::ifstream file(filename);
	int openCurlyBraceCount = 0;
	int closeCurlyBraceCount = 0;
	size_t pos;

	this->numberOfServers = 0;

	if (!file.is_open())
		throw(std::runtime_error("Could not open the configuration file!"));

	for (std::string line; std::getline(file, line);)
	{
		std::istringstream isLine(line);
		std::string key;
		isLine >> key;
		std::cout << key << std::endl;
		if (key == "server")
		{
			openCurlyBraceCount = 0;
			closeCurlyBraceCount = 0;
			this->numberOfServers++;
			pos = line.find('{');
			if (pos != std::string::npos)
				openCurlyBraceCount++;
			pos = line.find('}');
			if (pos != std::string::npos)
				closeCurlyBraceCount++;
			for (std::string line; std::getline(file, line) && openCurlyBraceCount != closeCurlyBraceCount;)
			{
				std::istringstream isLine(line);
				std::cout << ORANGE << "server count = " << this->numberOfServers << ", open brace count = " << openCurlyBraceCount << ", closed brace count = " << closeCurlyBraceCount << RESET << std::endl;
				std::cout << MAGENTA << "line : " << line << RESET << std::endl;
				pos = line.find('{');
				if (pos != std::string::npos)
					openCurlyBraceCount++;
				pos = line.find('}');
				if (pos != std::string::npos)
					closeCurlyBraceCount++;
			}
		}
	}
	if (closeCurlyBraceCount != openCurlyBraceCount)
		throw(std::runtime_error("parsing config: unclosed brace!"));
	std::cout << YELLOW << "server count = " << this->numberOfServers << RESET << std::endl;
}

/* --------------CONSTRUCTORS */
Webserv::Webserv(unsigned int _port, char *filename) : port(_port), serverFD(socket(AF_INET, SOCK_STREAM, 0)), optval(1)
{
	std::ifstream file(filename);

	countServer(filename);
	if (!file.is_open())
		throw(std::runtime_error("Could not open the configuration file!"));

	for (unsigned int i = 0; i < this->numberOfServers; i++)
	{
	}
	if (serverFD < 0)
	{
		throw(std::range_error("socket failed!"));
	}
	if (set_nonblocking(serverFD) < 0)
	{
		close(serverFD);
		throw(std::range_error("set_nonblocking failed!"));
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		throw(std::range_error("setsockopt(SO_REUSEADDR) failed"));

	if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		close(serverFD);
		throw(std::range_error("bind failed"));
	}

	if (listen(serverFD, 10) < 0)
	{
		close(serverFD);
		throw(std::range_error("listen failed!"));
	}

	serverListeningLoop(serverFD);
}

Webserv::Webserv(void) : port(8080), serverFD(socket(AF_INET, SOCK_STREAM, 0)), optval(1)
{
	if (serverFD < 0)
	{
		throw(std::range_error("socket failed!"));
	}
	if (set_nonblocking(serverFD) < 0)
	{
		close(serverFD);
		throw(std::range_error("set_nonblocking failed!"));
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
		throw(std::range_error("setsockopt(SO_REUSEADDR) failed"));

	if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		close(serverFD);
		throw(std::range_error("bind failed"));
	}

	if (listen(serverFD, 10) < 0)
	{
		close(serverFD);
		throw(std::range_error("listen failed!"));
	}

	serverListeningLoop(serverFD);
}

/* --------------DECONSTRUCTORS */
Webserv::~Webserv(void)
{
	close(serverFD);
}

/* --------------COPY */
Webserv::Webserv(const Webserv &other) : port(other.port), serverFD(other.serverFD)
{
}

/* --------------OPERATOR */
const Webserv &Webserv::operator=(const Webserv &other)
{
	if (this != &other)
	{
		port = other.getPort();
		serverFD = other.getServerFD();
	}
	return *this;
}

/* --------------GETTER */
const unsigned int &Webserv::getPort(void) const
{
	return (this->port);
}

const int &Webserv::getServerFD(void) const
{
	return (this->serverFD);
}