#pragma once

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Dependencies.hpp"
#include "ConfigurationFile.hpp"

// ************************************************************************** //
//                               Class                                		  //
// ************************************************************************** //

class Webserv {

private:
	unsigned int			port;
	int						serverFD;
	int						optval;
	
public:
	Webserv();
	Webserv(unsigned int _port);
	Webserv(const Webserv& other);
	~Webserv(void);
	const Webserv 	&operator=(const Webserv& other);

	const int			&getServerFD(void) const;
	const unsigned int	&getPort(void) const;
	

};

/* ---------------------UTILS FUNCTIONS */
void	processClientInput(const char *clientInput, int server_fd, int clientFD);
int		set_nonblocking(int fd);
void	serverListeningLoop(int server_fd);


#endif