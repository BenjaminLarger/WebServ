/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:06:22 by blarger           #+#    #+#             */
/*   Updated: 2024/07/09 12:07:25 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"

/* --------------CONSTRUCTORS */																													//IPV4	//TCP Protocol
Webserv::Webserv(const char *_filename, ServerConfig config) : filename(_filename), /* config(filename) */ port(config.getPort()), serverFD(socket(AF_INET, SOCK_STREAM, 0)), optval(1)
{
	std::ifstream file(filename);

	// countAndParseServer(filename);

	/* for (unsigned int i = 0; i < this->numberOfServers; i++)
	{
	} */
	if (serverFD < 0)
	{
		throw(std::range_error("socket failed!"));
	}
	if (setNonBlocking(serverFD) < 0)
	{
		close(serverFD);
		throw(std::range_error("set_nonblocking failed!"));
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;//IPV4 adress family
	address.sin_addr.s_addr = INADDR_ANY;//Hear from default 0.0.0.0
	address.sin_port = htons(port);//htons convert the port bytes order to the net one

	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)//Configuration of the socket to avoid the "Address already in use" error
		throw(std::range_error("setsockopt(SO_REUSEADDR) failed"));

	if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)//Associate the server socket with a specific adress and port
	{
		close(serverFD);
		throw(std::range_error("bind failed"));
	}

	if (listen(serverFD, 10) < 0)//Server is listenning
	{
		close(serverFD);
		throw(std::range_error("listen failed!"));
	}

	serverListeningLoop(serverFD);
}

Webserv::Webserv(void) : filename(DEFAULT_CONFIG_PATH), /*  config(filename), */ port(config.getPort()), serverFD(socket(AF_INET, SOCK_STREAM, 0)), optval(1)
{
}

/* --------------DECONSTRUCTORS */
Webserv::~Webserv(void)
{
	close(serverFD);
}

/* --------------COPY */
/* Webserv::Webserv(const Webserv &other) : port(other.port), serverFD(other.serverFD)
{
} */

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
