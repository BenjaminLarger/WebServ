#pragma once

#ifndef POST_HPP
#define POST_HPP

#include "Webserv.hpp"
#include "ErrorUtils.hpp"

class POST : public Webserv
{

	private:
		// Request line
		std::map<std::string, std::string>	headers;
		std::istringstream					requestStream;
		std::string							pathToRessource;
		std::string							HTTPversion;
		// Header
		std::string							host;
		std::string							contentType;
		int									contentLength;
		// Body
		std::string							body;

		void	extractFirstLine();
		void	extractHeaders();
		void	extractBody(int clientFD);
	public:
		POST(Webserv _server, int serverFD, int clientFD, std::string &clientInput);
		POST();
		~POST(void);


};

#endif