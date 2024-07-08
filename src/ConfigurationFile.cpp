/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:48:15 by blarger           #+#    #+#             */
/*   Updated: 2024/07/08 18:31:05 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/ConfigurationFile.hpp"

/* --------------CONSTRUCTORS */
ServerConfig::ServerConfig(){};

ServerConfig::ServerConfig(const char *filename)
{
	std::map<std::string, std::string> options;
	std::ifstream file(filename);
	std::string currentLocation;

	if (!file.is_open())
		throw(std::runtime_error("Could not open the configuration file!"));

	for (std::string line; std::getline(file, line);)
	{
		std::istringstream isLine(line);
		std::string key;

		isLine >> key;
		std::cout << "----------------------------------------------\n";
		std::cout << BLUE << "line = " << line << std::endl;
		std::cout << ORANGE << "key = " << key << RESET << std::endl;
		if (key == "server")
		{
			isLine >> key;
			std::cout << ORANGE << "key = " << key << ", key len = " << strlen(key.c_str()) << RESET << std::endl;
		}
		if (key == "{")
		{
			isLine >> key;
			std::cout << ORANGE << "key = " << key << RESET << std::endl;
		}
		if (key[0] == '#')
			continue;
		else if (key == "client_max_body_size")
		{
			isLine >> this->maxBodySize;
			std::cout << "Max body size = " << YELLOW << this->maxBodySize << RESET << std::endl;
		}
		else if (key == "listen")
		{
			isLine >> this->port;
			std::cout << "port = " << YELLOW << this->port << RESET << std::endl;
		}
		else if (key == "server_name")
		{
			isLine >> this->host;
			std::cout << "host = " << YELLOW << this->host << RESET << std::endl;
		}
		else if (key == "error_page")
		{
			int code;
			std::string path;
			isLine >> code >> path;
			this->errorPages[code] = path;
			std::cout << "error_page host = " << YELLOW << code << RESET << " path = " << YELLOW << path << RESET << std::endl;
		}
		else if (key == "location")
		{
			isLine >> currentLocation;
			std::cout << "cur location " << YELLOW << currentLocation << RESET << std::endl;
			// currentLocation = currentLocation.substr(1, currentLocation.length() - 1);
			std::getline(file, line);
			std::istringstream isLine(line);
			isLine >> key;
			while (true) //(if line or next_line has '{')
			{
				std::cout << MAGENTA << "key = " << key << RESET << std::endl;
				if (key == "root")
				{
					isLine >> this->locations[currentLocation].root;
					std::cout << "current location root = " << YELLOW << this->locations[currentLocation].root << RESET << std::endl;
				}
				else if (key == "index")
				{
					std::istringstream isLine(line);
					std::string index;
					isLine >> index;
					isLine >> index;
					std::cout << "index = " << index << std::endl;
					this->locations[currentLocation].index = index;
					std::cout << "current location index = " << YELLOW << this->locations[currentLocation].index << RESET << std::endl;
				}
				else if (key == "return")
				{
					// to implement (redirection)
				}
				else if (key == "autoindex")
				{
					std::string autoIndexNature;
					isLine >> autoIndexNature;
					if (autoIndexNature == "on")
						this->locations[currentLocation].autoIndex = true;
					else
						this->locations[currentLocation].autoIndex = false;
				}
				else if (key == "allow_methods")
				{
					std::cout << RED << "in loop\n"
							  << RESET;
					std::istringstream isLine(line);
					std::string method;
					isLine >> method;
					while (true)
					{
						if (method == "allow_methods")
							isLine >> method;
						if (method[strlen(method.c_str()) - 1] == ',' || method[strlen(method.c_str()) - 1] == ';' || method[strlen(method.c_str()) - 1] == ';' || method[strlen(method.c_str()) - 1] == '}' || method[strlen(method.c_str()) - 1] == ' ')
							method[strlen(method.c_str()) - 1] = '\0';
						std::cout << "method = " << YELLOW << method << RESET << std::endl;
						if (strncmp(method.c_str(), "POST", 4) && strncmp(method.c_str(), "GET", 3) && strncmp(method.c_str(), "DELETE", 6))
							break;
						this->locations[currentLocation].allowedMethods.push_back(method);
						std::cout << "current location allowed method = " << YELLOW << method << RESET << std::endl;
						std::string prevMethod = method;
						isLine >> method;
						std::cout << "prev method = " << RED << prevMethod << ", method = " << method << RESET << std::endl;
						if (method == prevMethod)
							break;
						std::cout << "method = " << MAGENTA << method << RESET << std::endl;
					}
				}
				else if (key[0] == '}')
					break;
				std::getline(file, line);
				std::istringstream isLine(line);
				isLine >> key;
			}
			// if line has '}' break (if first line '{')
			std::cout << RED << line << RESET << std::endl;

			this->locations[currentLocation] = LocationConfig(); // does not exist
			std::cout << "this->locations[currentLocation] = " << YELLOW << this->locations[currentLocation].root << RESET << std::endl;
		}
	}
}

/* --------------COPY */
/* ServerConfig::ServerConfig(const ServerConfig& other) : port(other.port), host(other.host), maxBodySize(other.maxBodySize)
{} */

/* --------------GETTER */
const unsigned int &ServerConfig::getPort(void) const
{
	return (this->port);
}
