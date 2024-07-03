/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:48:15 by blarger           #+#    #+#             */
/*   Updated: 2024/07/03 19:04:50 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/ConfigurationFile.hpp"

/* --------------CONSTRUCTORS */

ServerConfig	ServerConfig::parsConfigFile(char *filename)
{
	std::map<std::string, std::string> options; 
	std::ifstream	file(filename);
	std::string		currentLocation;
	ServerConfig	serverConfig;

	if (!file.is_open())
		throw (std::runtime_error("Could not open the configuration file!"));

	for (std::string line; std::getline(file, line);)
	{
		std::istringstream	isLine(line);
		std::string			key;
		
		isLine >> key;
		std::cout << "line = " << line << std::endl;
		std::cout << "key = " << key << std::endl << std::endl;
		if (key == "server") //skip server {
		{
			//std::getline(file, line);
		}
		else if (key[0] == '#') //skip comments
			continue ;
		else if (key == "listen")
		{
			isLine >> serverConfig.port;
			std::cout << "port = " << YELLOW  << serverConfig.port << RESET << std::endl;
			
		}
		else if (key == "server_name")
		{
			isLine >> serverConfig.host;
			std::cout << "host = " << YELLOW  << serverConfig.host << RESET << std::endl;
		}
		else if (key == "error_page")
		{
			int			code;
			std::string	path;
			isLine >> code >> path;
			serverConfig.errorPages[code] = path;
			std::cout << "error_page host = " << YELLOW  << code << RESET << " path = " << YELLOW << path << RESET << std::endl;
		}
		else if (key == "location")
		{
			isLine >> currentLocation;
			currentLocation = currentLocation.substr(1, currentLocation.length() - 1);
			std::cout << RED << currentLocation << RESET << std::endl;
			std::getline(file, line);
			std::istringstream	isLine(line);
			isLine >> key;
			std::cout << MAGENTA << key << RESET << std::endl;
			//while (true) (if line or next_line has '{')
				if (key == "root")
				{
					isLine >> serverConfig.locations[currentLocation].root;
					std::cout << "current location root = " << YELLOW  << serverConfig.locations[currentLocation].root << RESET << std::endl;
				}
				else if (key == "index")
				{
					isLine >> serverConfig.locations[currentLocation].index;
					std::cout << "current location index = " << YELLOW  << serverConfig.locations[currentLocation].index << RESET << std::endl;
				}
				else if (key == "allow_methods")
				{
					std::string	method;
					while (isLine >> method)
					{
						serverConfig.locations[currentLocation].allowedMethods.push_back(method);			
						std::cout << "current location allowed method = " << YELLOW  << method << RESET << std::endl;
					}
				}
			//if line has '}' break (if first line '{')
			std::cout << RED << line << RESET << std::endl;
			
			serverConfig.locations[currentLocation] = LocationConfig(); //does not exist
			std::cout << "serverConfig.locations[currentLocation] = " << YELLOW  << serverConfig.locations[currentLocation].root << RESET << std::endl;
		}
	}
	return (serverConfig);
}

/* --------------COPY */
/* ServerConfig::ServerConfig(const ServerConfig& other) : port(other.port), host(other.host), maxBodySize(other.maxBodySize)
{} */


/* --------------GETTER */
const unsigned int	&ServerConfig::getPort(void) const
{
	return (this->port);
}
