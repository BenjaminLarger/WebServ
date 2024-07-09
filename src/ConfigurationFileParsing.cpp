/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFileParsing.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:08:47 by blarger           #+#    #+#             */
/*   Updated: 2024/07/09 10:35:55 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"

std::map<int, std::string> ServerConfig::findErrorPage(std::istringstream &isLine)
{
	int code;
	std::string path;
	std::map<int, std::string> errorPage;
	isLine >> code >> path;

	errorPage[code] = path;
	std::cout << "error_page host = " << YELLOW << code << RESET << " path = " << YELLOW << path << RESET << std::endl;
	return (errorPage);
}

void ServerConfig::findLocation(std::string &line, ServerConfig &config, std::ifstream &file, std::string currentLocation)
{
	std::string key;

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
			isLine >> config.locations[currentLocation].root;
			std::cout << "current location root = " << YELLOW << config.locations[currentLocation].root << RESET << std::endl;
		}
		else if (key == "index")
		{
			std::istringstream isLine(line);
			std::string index;
			isLine >> index;
			isLine >> index;
			std::cout << "index = " << index << std::endl;
			config.locations[currentLocation].index = index;
			std::cout << "current location index = " << YELLOW << config.locations[currentLocation].index << RESET << std::endl;
		}
		else if (key == "return")
		{
			// to implement (redirection)
		}
		/* What is Directory Listing?
		Directory listing is a feature that allows the server to display the contents
		of a directory if no specific file is requested. When enabled, accessing a
		directory URL without a specified file will list all files and subdirectories
		within that directory. */
		else if (key == "autoindex")
		{
			std::string autoindexState;
			isLine >> autoindexState;
			if (autoindexState == "on")
				config.locations[currentLocation].autoindexOn = true;
			else
				config.locations[currentLocation].autoindexOn = false;
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
				config.locations[currentLocation].allowedMethods.push_back(method);
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

	config.locations[currentLocation] = LocationConfig(); // does not exist
	std::cout << "config.locations[currentLocation] = " << YELLOW << config.locations[currentLocation].root << RESET << std::endl;
}
