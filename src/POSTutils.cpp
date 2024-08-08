/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTutils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:20:52 by blarger           #+#    #+#             */
/*   Updated: 2024/08/08 13:47:54 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"

bool	POST::isClosingBoundary(std::string line, std::string boundary)
{
	if (line[line.size() - 1] == '\r' && line[line.size() - 2] == '-' && line[line.size() - 3] == '-')
	{
		if (strncmp(line.c_str(), boundary.c_str(), boundary.size()))
			return (true);
	}
	return (false);
}

bool	POST::isBoundary(std::string line, std::string boundary)
{
	std::string newline;
	if (line[0] == '-' && line[1] == '-')
	{
		std::cout << ORANGE << "line = " << line << RESET << std::endl;
		newline = makeCopy(line);
		std::cout << RED << newline << RESET << std::endl;
		std::cout << RED << boundary << RESET << std::endl;
		if (!strcmp(newline.c_str(), boundary.c_str()))
		{
			std::cout << ORANGE << "is boundary\n" << RESET << std::endl;
			return (true);
		}
		
	}
	return (false);
}

int	POST::extractValues(std::string line, std::map<int, Content> &myMap, int index, std::string key, const std::string &content)
{
	std::string values;

	if (contentMap[index].HasBody == true)
	{
		std::cout << RED << "Body has been defined before content\n" << RESET;
		//sendall(ClientFD, BODY_BEFORE_CONTENT_ERROR, strlen(BODY_BEFORE_CONTENT_ERROR));
		//send error page
		return (FAILURE);
	}
	values = line.substr(key.size() + 1);
	if (content == "Content disposition")
		myMap[index].contentDisposition = values;
	else if (content == "Content type")
		myMap[index].contentType = values;
	std::cout << ORANGE << "map[" << index << "] = " << values << RESET << std::endl;	
	if (content == "Content disposition")
		myMap[index].HasContentDisposition = true;
	else if (content == "Content type")
		myMap[index].HasContentType = true;
	return (SUCCESS);
}

std::string POST::extractBoundary(const std::string& input)
{
   std::size_t pos = input.find('=');
	 std::cout << "input = " << input << ", " << pos << std::endl;
  if (pos != std::string::npos)
	{
		std::cout << input.substr(pos + 3) << std::endl;
		std::cout << CYAN << "input.substr(pos + 1)[0] = " << input.substr(pos + 1)[0] << ", input.substr(pos + 1)[1] = " << input.substr(pos + 1)[1] << RESET << std::endl;
		if (input.substr(pos + 1)[0] == '-' && input.substr(pos + 1)[1] == '-')
			return input.substr(pos + 3);
	}
	return "";
}

void	POST::readAllRequest()
{
	std::string line;

	requestStream.clear();
	requestStream.seekg(0);
	std::cout << "\n--------------READING ALL CONTENT--------------";
	while (std::getline(requestStream, line))
	 {
		std::cout << "\n" << MAGENTA << line << RESET;
	 }
	 std::cout << std::endl;
}
std::string	POST::skipBoundaryPart(void)
{
	std::string line;

	readAllRequest();
	requestStream.clear();
	requestStream.seekg(0);
	//skip until bundary
	 while (std::getline(requestStream, line))
	 {
		//std::cout << "\n" << MAGENTA << "line = "<< line << RESET << std::endl;
		if (!strncmp(line.c_str(), "--", 2))
			break;		
	 }
	return (extractBoundary(contentType));
}
