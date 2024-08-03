/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTupload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:13:50 by blarger           #+#    #+#             */
/*   Updated: 2024/08/03 13:29:15 by blarger          ###   ########.fr       */
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

std::string extractBoundary(const std::string& input)
{
   std::size_t pos = input.find('=');
	 std::cout << "input = " << input << ", " << pos << std::endl;
  if (pos != std::string::npos)
	{
		std::cout << input.substr(pos + 1) << std::endl;
		return input.substr(pos + 1);
	}
	else
		return "";
}

std::string	POST::skipBoundaryPart(void)
{
	std::string line;
	std::string boundary;
	int			index = 0;

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
	boundary = extractBoundary(contentType);

	std::string	key = line;
	std::string	value;
	if (boundary.empty())
	 	return ; //Handle error
		
	while (key)
	 {
		requestStream >> key;
		std::cout << "key = " << MAGENTA << key << RESET << std::endl;
		if (key == boundary)
			index++;
		else if (key == "Content-Disposition")
		{
			requestStream >> value;
			contentDispositionMap[index] = value;
		}
		else if (key == "Content-Type")
		{
			requestStream >> value;
			this->contentTypeMap[index] = value;
		}
	 }
	 
}
	