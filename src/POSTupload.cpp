/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTupload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:13:50 by blarger           #+#    #+#             */
/*   Updated: 2024/08/05 12:53:10 by blarger          ###   ########.fr       */
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

bool	lineIsEmpty(std::string line)
{
	for (int i = 0; line[i]; i++)
	{
		std::cout << (int)line[i] << ", ";
		if ((line[i] < 9 || line[i] > 13) && line[i] != 32)
			return (false);
	}
	std::cout << std::endl;
	std::cout << ORANGE << "Line is empty!\n" << RESET;
	return (true);
}

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

int	POST::extractValues(std::string line, std::map<int, std::string> &myMap, int index, std::string key, std::map<int, bool> &HasContent)
{
	std::string values;

	if (HasBody[index] == true)
	{
		std::cout << RED << "Body has been defined before content\n" << RESET;
		sendall(ClientFD, BODY_BEFORE_CONTENT_ERROR, strlen(BODY_BEFORE_CONTENT_ERROR));
		return (FAILURE);
	}
	values = line.substr(key.size() + 1);
	myMap[index] = values;
	std::cout << ORANGE << "map[" << index << "] = " << values << RESET << std::endl;	
	HasContent[index] = true;
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
	while (std::getline(requestStream, line) && line[line.size() - 1] == '\r')
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
	 while (std::getline(requestStream, line) && line[line.size() - 1] == '\r')
	 {
		//std::cout << "\n" << MAGENTA << "line = "<< line << RESET << std::endl;
		if (!strncmp(line.c_str(), "--", 2))
			break;		
	 }
	return (extractBoundary(contentType));
}

int	POST::parseContent(int index)
{
	std::string	key;

	std::cout << "index = " << index << std::endl;
	for (int i = 0; i < index; i++)
	{
		if (HasContentDisposition[i] == false)
		{
			std::cout << RED << index << " has not content disposition!" << RESET << std::endl;
			return (FAILURE);
		}
		key = extractFirstWord(contentDispositionMap[index]);
		std::cout << BLUE << "key = " <<  key << RESET << std::endl;
		if (key != "form-data;" && key != "form-data;")
			std::cout << RED << "Webserver can only handle form-data key of content disposition!\n" << RESET << std::endl;
		else
		{
			
		}
		
		/* std::istringstream stream(contentDispositionMap[index]);
		stream >> str;
		while (str.empty() == false)
		{
			
		} */
		
	}
	return (SUCCESS);
}

int	POST::extractMultipartFormData()
{
	std::string	key;
	std::string	value;
	std::string boundary;
	std::string line;
	int			index = 0;
	hasClosingBoundary = false;
	HasContentType[0] = false;
	HasContentDisposition[0] = false;
	

	std::cout << "\nUPLOAD BODY :\n";
//Parse the Content-Type header to get the boundary string. => done
//Ensure the boundary string is correctly identified and doesn't appear in the data. => to implement
	boundary = skipBoundaryPart();
	std::cout << "boundary = " << boundary << std::endl;
	if (boundary.empty())
	{
		std::cout << RED << "Boundary separartion not found!\n" << RESET;
		//sendall(ClientFD, HAS_NOT_BOUNDARY_ERROR, strlen(HAS_NOT_BOUNDARY_ERROR));
		return (FAILURE); //handle error
	}
	while (std::getline(requestStream, line) && line[line.size() - 1] == '\r')
	 {
		std::cout << BLUE << line << std::endl;
		key = extractFirstWord(line);
		std::cout << "key = " << MAGENTA << key << RESET << std::endl;
		if (isBoundary(line, boundary) == true)
		{
			//New part has been iddentified
			std::cout << YELLOW << "New part iddentified!\n" << RESET;
			index++;
			HasContentType[index] = false;
			HasContentDisposition[index] = false;
			HasBody[index] = false;
		}
		else if (key == "Content-Disposition:")
		{
			std::cout << YELLOW << "Extracting Content Disposition\n" << RESET;
			//Content-Disposition is madatory and specifies how the content is to be handled, often indicating form field names and filenames
			if (extractValues(line, contentDispositionMap, index, key, HasContentDisposition) == FAILURE)
				return (FAILURE);
		}
		else if (key == "Content-Type:")
		{
			std::cout << YELLOW << "Extracting Content Type\n" << RESET;
			//This header is optional and specifies the media type of the content.
			if (extractValues(line, contentTypeMap, index, key, HasContentType) == FAILURE)
				return (FAILURE);
		}
		else if (isClosingBoundary(line, boundary) == true)
		{
			std::cout << GREEN << "Has closing boundary ==> return SUCCESS\n" << RESET;
			hasClosingBoundary = true;
			return (parseContent(index));
		}
		else if (HasContentDisposition[index] == true && lineIsEmpty(line) == false)
		{
			//Body apppears at the right place
			std::cout << YELLOW << "Extracting Body\n" << RESET;
			bodyMap[index] += line;
			std::cout << "Body[" << index << "] = " << YELLOW << bodyMap[index] << RESET << std::endl;
			HasBody[index] = true;
		}
		else if (lineIsEmpty(line) == false)
		{
			//Body appears before / without content disposition
			std::cout << YELLOW << "Body appears before / without content disposition\n"<< RESET ;
			sendall(ClientFD, CONTENT_AFTER_BODY_ERROR, strlen(CONTENT_AFTER_BODY_ERROR));
			return (FAILURE);
		}
	 }
/* 	if (hasClosingBoundary == false)
	{
		sendall(ClientFD, CLOSING_BOUNDARY_ERROR, strlen(CLOSING_BOUNDARY_ERROR));
		return (FAILURE);
	} */
	std::cout << GREEN << "Has not closing boundary ==> return SUCCESS\n" << RESET << std::endl;
	return (parseContent(index));
}
