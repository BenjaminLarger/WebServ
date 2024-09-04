/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTupload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:13:50 by blarger           #+#    #+#             */
/*   Updated: 2024/09/04 13:09:13 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"
#include "utils.hpp"

void	POST::parseContentDisposition(int index, const std::string &content)
{
	std::string 				key;
	std::istringstream	stream(content);
	std::string					lastWorld;

	std::getline(stream, key, ';');

	while (true)
	{
		std::getline(stream, key, '=');
		if (lineIsEmpty(key) == true || lastWorld == key) {
			break ;
		}
		else if (!strncmp(key.c_str(), " name", 4) || key == "name")
		{
			std::getline(stream, contentMap[index].name, ';');
			trimQuotes(contentMap[index].name);
		}
		else if (!strncmp(key.c_str(), " filename", 9))
		{
			std::getline(stream, contentMap[index].filename, ';');
			trimQuotes(contentMap[index].filename);
		}
		else if (key.empty() == true || lineIsEmpty(key) == true || lastWorld == key)
			break ;
		lastWorld = key;
	}
}

void	POST::parseContentType(int index, std::string &content)
{
	if (contentMap[index].HasContentType == true)
	{
		std::istringstream stream(content);
		std::getline(stream, contentMap[index].name, ';');
		trimBothEnds(contentMap[index].name);
	}
	else
	{
		std::istringstream stream("text/plain");
		std::getline(stream, contentMap[index].name, ';');
		trimBothEnds(contentMap[index].name);
	}
}

int	POST::parseContent(int index)
{
	std::string	key;

	for (int i = 0; i <= index; i++)
	{
		if (contentMap[i].HasContentDisposition == false)
			throw HttpException(400, "Bad request: Client request has not content disposition");
		key = extractFirstWord(contentMap[i].contentDisposition);
		if (key != "form-data;" && key != "form-data;")
		{
			throw HttpException(400, "Bad request: can only handle form-data key of content disposition");
		}
		else
		{
			parseContentDisposition(i, contentMap[i].contentDisposition);
		}
		parseContentType(i, contentMap[i].contentType);
	}
	_formValues["ID"] = clientID;
	return (handleFileUpload(index));
}

int	POST::extractMultipartFormData(std::string &_boundary)
{
	std::string	key;
	std::string	value;
	std::string line;
	int					index = 0;
	//int					contentRead = 0;
	hasClosingBoundary = false;
	contentMap[0].HasContentType = false;
	contentMap[0].HasContentDisposition = false;
	_formValues["Name"] = "";
	_formValues["Age"] = "";
	_formValues["File"] = "";
	
	requestStream.clear();
	requestStream.seekg(0);
	_boundary = boundary;
	boundary = skipBoundaryPart();
	if (boundary.empty())
		throw HttpException(400, "Bad Request: missing boundary in post request.");
	while (std::getline(requestStream, line))
	 {
		key = extractFirstWord(line);
		if (isClosingBoundary(line) == true)
		{
			hasClosingBoundary = true;
			return (parseContent(index));
		}
		else if (isBoundary(line) == true)
		{
			//New part has been iddentified
			handleNewPart(index);
		}
		else if (key == "Content-Disposition:")
		{
			//Content-Disposition is madatory and specifies how the content is to be handled, often indicating form field names and filenames
			if (extractValues(line, contentMap, index, key, "Content disposition") == FAILURE)
				throw HttpException(400, "Bad Request: invalid content disposition request.");
		}
		else if (key == "Content-Type:")
		{
			//This header is optional and specifies the media type of the content.
			if (extractValues(line, contentMap, index, key, "Content type") == FAILURE)
				throw HttpException(400, "Bad Request: invalid post request.");
		}
		else if (contentMap[index].HasContentDisposition == true && lineIsEmpty(line) == false)
		{
			//Body apppears at the right place
			handleBody(line, index);
		}
	 }
	//final boundary not yet identified. Other chunks might complete the POST request
	return (FAILURE);
}

int POST::handleFileUpload(int index)
{
	//std::string directory = "./upload/";
	for (int i = 0; i <= index; i++)
	{
		//check the file exist
		if (lineIsEmpty(contentMap[i].filename) == false)
			{
			//std::string	filePath = directory + contentMap[i].filename;
			_formValues["File"] = contentMap[i].filename;;
			std::string	filePath = UPLOAD_FILE_DIR + contentMap[i].filename;
			std::ofstream outFile(filePath.c_str(), std::ios::binary);
			if (!outFile)
				throw HttpException(500, strerror(errno));
			fileContentBinary = extractBinaryContent(clientInputVector);
			
			outFile.write(fileContentBinary.data(), fileContentBinary.size());
			if (!outFile)
        throw (HttpException(500, "Failed to write in outfile"));
			std::cout << GREEN << "File" << contentMap[i].filename <<" successfully uploaded !" << RESET << std::endl;
			outFile.close();
		}
	}
	return (SUCCESS);
}