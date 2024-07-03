/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfiguratoinFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:48:15 by blarger           #+#    #+#             */
/*   Updated: 2024/07/03 12:56:38 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/ConfigurationFile.hpp"

/* --------------CONSTRUCTORS */

void	ServerConfig::parsConfigFile(char *filename)
{
	std::map<std::string, std::string> options; 
	std::string		line;
	std::ifstream	file(filename);
	bool			error;

	if (!file.is_open())
		throw (std::runtime_error("Could not open the configuration file!"));

	for (std::string line; std::getline(file, line);)
	{
		std::istringstream	isLine(line);
		std::string			id, eq, val;
		
		if (!(isLine >> id))
			throw std::range_error("Failed to extract id from the line.");
		else if (id[0] == '#')
			continue ;
		else if (!(isLine >> eq >> val >> std::ws) || eq != "=" || isLine.get() != EOF)
			throw std::range_error("");
		else
			options[id] = val;
	}
}

/* --------------DECONSTRUCTORS */
ServerConfig::~ServerConfig(void)
{
}

/* --------------COPY */
ServerConfig::ServerConfig(const ServerConfig& other) : port(other.port), host(other.host), maxBodySize(other.maxBodySize)
{

}

/* --------------OPERATOR */
const ServerConfig 	&ServerConfig::operator=(const ServerConfig& other)
{
	if (this != &other) {
		port = other.port;
        host = other.host;
		maxBodySize = other.maxBodySize;
    }
    return *this;
}

/* --------------GETTER */
const unsigned int	&ServerConfig::getPort(void) const
{
	return (this->port);
}
