/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTupload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:13:50 by blarger           #+#    #+#             */
/*   Updated: 2024/08/03 18:46:37 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"

/*
DEFINITION
	Multipart/form-data is a media type used for handling
	forms that include file uploads. Unlike the simpler
	application/x-www-form-urlencoded, which encodes the
	data as key-value pairs, multipart/form-data allows
	for more complex data, including binary files.
STRUCTURE
	Boundary String: Used to separate different parts of the message.
	Parts: Each part contains headers and body content. A typical part
	for a file upload includes:
	Content-Disposition: Specifies the form-data, including the name
	and filename.
	Content-Type: The media type of the file (e.g., image/png,
	text/plain).
	Body: The actual content of the file.
	The format typically looks like this:

		--boundary
		Headers

		Body
		--boundary--
*/

bool	isClosingBoundary(std::string line, std::string boundary)
{
	if (line[line.size() - 1] == '\r' && line[line.size() - 2] == '-' && line[line.size() - 3] == '-')
	{
		if (strncmp(line.c_str(), boundary.c_str(), boundary.size()))
			return (true);
	}
	return (false);
}
std::string extractFirstWord(const std::string& str)
{
    std::istringstream stream(str);
    std::string firstWord;

    stream >> firstWord;
    return (firstWord);
}
std::string makeCopy(const std::string &original)
{
    if (original.length() < 4) {
        return "";
    }
    std::string copy;
    for (size_t i = 4; i < original.length(); ++i) {
        copy += original[i];
    }
		std::cout << RED << copy << RESET << std::endl;
    return copy;
}

bool	isBoundary(std::string line, std::string boundary)
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

void	extractValues(std::string line, std::map<int, std::string> &myMap, int index, std::string key)
{
	std::string values;

	values = line.substr(key.size() + 1);
	myMap[index] = values;
	std::cout << ORANGE << "map[" << index << "] = " << values << RESET << std::endl;	
}

std::string extractBoundary(const std::string& input)
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

std::string	POST::skipBoundaryPart(void)
{
	std::string line;

	requestStream.clear();
	requestStream.seekg(0);
	std::cout << "\nUPLOAD BODY :\n";
	//skip until bundary
	 while (std::getline(requestStream, line) && line[line.size() - 1] == '\r')
	 {
		std::cout << "\n" << MAGENTA << line << RESET << std::endl;
		if (!strncmp(line.c_str(), "--", 2))
			break;		
	 }
	return (extractBoundary(contentType));
}
void	POST::extractUploadBody()
{
	std::string	key;
	std::string	value;
	std::string boundary;
	std::string line;
	int			index = 0;
	hasClosingBoundary = false;
	

	std::cout << "\nUPLOAD BODY :\n";
	boundary = skipBoundaryPart();
	std::cout << boundary << std::endl;
	if (boundary.empty())
		return ; //handle error
	while (std::getline(requestStream, line) && line[line.size() - 1] == '\r')
	 {
		std::cout << BLUE << line << std::endl;
		key = extractFirstWord(line);
		std::cout << "key = " << MAGENTA << key << RESET << std::endl;
		if (isBoundary(line, boundary) == true)
			index++;
		else if (key == "Content-Disposition:")
		{
			extractValues(line, contentDispositionMap, index, key);
			/* requestStream >> value;
			contentDispositionMap[index] = value;
			std::cout << YELLOW << "contentDispositionMap[" << index << "] = " << value << RESET << std::endl; */
		}
		else if (key == "Content-Type:")
		{
			extractValues(line, contentTypeMap, index, key);
/* 			requestStream >> value;
			this->contentTypeMap[index] = value;
			std::cout << YELLOW << "contentTypeMap[" << index << "] = " << value << RESET << std::endl; */
		}
		
		//else if (!strncmp(line.c_str(), boundary.c_str(), boundary.size() - 2)/*  && boundary[boundary.size() - 2] == '-' && boundary[boundary.size() - 1] == '-' */)
		else if (isClosingBoundary(line, boundary) == true)
		{
			std::cout << YELLOW << "Has closing boundary\n" << RESET;
			hasClosingBoundary = true;
		}
	 }
	 
}
