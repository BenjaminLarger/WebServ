/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTupload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:13:50 by blarger           #+#    #+#             */
/*   Updated: 2024/08/02 18:11:05 by blarger          ###   ########.fr       */
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

	//requestStream.clear();
//	requestStream.seekg(0);
std::getline(requestStream, line);
	std::cout << line << "here\n";
	while (std::getline(requestStream, line) && line[line.size() - 1] == '\r')
	{
		std::cout << "\n" << MAGENTA << line << RESET << std::endl;
		if (!strncmp(line.c_str(), "--", 2))
			break;
	}
	//std::cout << line << std::endl; we get the right line here
	return (extractBoundary(line));
}
void POST::extractUploadBody()
{
	std::string boundary;
	std::string	key;
	std::string	value;
	std::string	line;
	int					index = 0;

	std::cout << "\nUPLOAD BODY :\n";

	boundary = skipBoundaryPart();
	std::cout << boundary << std::endl;
	if (boundary.empty())
		return ; //handle error
	
	
	std::cout << BLUE << boundary << std::endl;
	while (std::getline(requestStream, line) && line[line.size() - 1] == '\r')
	{
		std::istringstream	copyStream(line);
		copyStream >> key;
		if (key == "Content-Disposition")
		{
			(void)value;
		}
		else if (key == "Content-Type:")
		{
			(void)value;
		}
		else if (key == boundary)
			index++;
	}
}

