/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTupload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:13:50 by blarger           #+#    #+#             */
/*   Updated: 2024/08/06 16:21:56 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"
#include "utils.hpp"

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

void	POST::parseContentDisposition(int index, const std::string &content)
{
	std::string key;
	std::istringstream stream(content);
	std::string	lastWorld;
	(void)index;
	std::cout << "\n" << "----------------------- PARSE CONTENT DISPOSITION-----------------------\n";
	std::cout << ORANGE << "content = " << content << RESET << std::endl;
	//skip form/data
	std::getline(stream, key, ';');
	std::cout << BLUE << "key = " << key << RESET << std::endl;
	//stream >> key;
	/* stream >> key;
	std::getline(stream, key, ';'); */
	while (true)
	{
		std::getline(stream, key, '=');
		std::cout << BLUE << "key = " << key << RESET << std::endl;
		if (lineIsEmpty(key) == true || lastWorld == key)
			break ;
		else if (!strncmp(key.c_str(), " name", 4) || key == "name")
		{
			std::getline(stream, contentMap[index].nameValue, ';');
			std::cout << YELLOW << "name = " << contentMap[index].nameValue << RESET << std::endl;
		}
		else if (!strncmp(key.c_str(), " filename", 8))
		{
			std::getline(stream, contentMap[index].filenameValue, ';');
			std::cout << YELLOW << "filename = " << contentMap[index].filenameValue << RESET << std::endl;
		}
		else if (key.empty() == true || lineIsEmpty(key) == true || lastWorld == key)
			break ;
		lastWorld = key;
	}
	std::cout << "\n" << "----------------------------------------------\n\n";
}

void	POST::parseContentType(int index, std::string &content)
{
	std::cout << "\n" << "----------------------- PARSE CONTENT DISPOSITION-----------------------\n";
	std::istringstream stream(content);
	std::getline(stream, contentMap[index].nameValue, ';');
//	contentType[index].contentType = content;
	trimBothEnds(contentMap[index].nameValue);
	std::cout << YELLOW << "Content type value = " << content << RESET << std::endl;
	std::cout << "\n" << "----------------------------------------------\n\n";
}

int	POST::parseContent(int index)
{
	std::string	key;

	std::cout << "index = " << index << std::endl;
	for (int i = 0; i <= index; i++)
	{
		if (contentMap[i].HasContentDisposition == false)
		{
			std::cout << RED << index << " has not content disposition!" << RESET << std::endl;
			return (FAILURE);
		}
		key = extractFirstWord(contentMap[i].contentDisposition);
		std::cout << BLUE << "key = " <<  key << RESET << std::endl;
		if (key != "form-data;" && key != "form-data;")
			std::cout << RED << "Webserver can only handle form-data key of content disposition!\n" << RESET << std::endl;
		else
		{
			parseContentDisposition(i, contentMap[i].contentDisposition);
		}
		if (contentMap[i].HasContentType == true)
		{
			key = extractFirstWord(contentMap[i].contentType);
			parseContentType(i, contentMap[i].contentType);
		}
		/* else
			contentType[i].contentType = "text/plain"; */
		
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
	contentMap[0].HasContentType = false;
	contentMap[0].HasContentDisposition = false;
	

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
			contentMap[index].HasContentType = false;
			contentMap[index].HasContentDisposition = false;
			contentMap[index].HasBody = false;
		}
		else if (key == "Content-Disposition:")
		{
			std::cout << YELLOW << "Extracting Content Disposition\n" << RESET;
			//Content-Disposition is madatory and specifies how the content is to be handled, often indicating form field names and filenames
			if (extractValues(line, contentMap, index, key, "Content disposition") == FAILURE)
				return (FAILURE);
		}
		else if (key == "Content-Type:")
		{
			std::cout << YELLOW << "Extracting Content Type\n" << RESET;
			//This header is optional and specifies the media type of the content.
			if (extractValues(line, contentMap, index, key, "Content type") == FAILURE)
				return (FAILURE);
		}
		else if (isClosingBoundary(line, boundary) == true)
		{
			std::cout << GREEN << "Has closing boundary ==> return SUCCESS\n" << RESET;
			hasClosingBoundary = true;
			return (parseContent(index));
		}
		else if (contentMap[index].HasContentDisposition == true && lineIsEmpty(line) == false)
		{
			//Body apppears at the right place
			std::cout << YELLOW << "Extracting Body\n" << RESET;
			contentMap[index].body += line;
			std::cout << "Body[" << index << "] = " << YELLOW << contentMap[index].body << RESET << std::endl;
			contentMap[index].HasBody = true;
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
