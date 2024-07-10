/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/07/10 11:57:54 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "GET.hpp"

void GET::findHeader(std::string &key, std::istringstream &isLine)
{
  std::string newKey;
  if (key == "Host:")
  {
    isLine >> this->host;
    isLine >> newKey;
    if (key != newKey)
      findHeader(newKey, isLine);
  }
  else if (key == "User-Agent:")
  {
    isLine >> this->userAgent;
    std::cout << YELLOW << "userAgent = " << this->host << RESET << std::endl;
    isLine >> newKey;
    std::cout << YELLOW << "key = " << key << RESET << std::endl;
    if (key != newKey)
      findHeader(newKey, isLine);
  }
  else if (key == "Accept:")
  {
    isLine >> this->accept;
    std::cout << YELLOW << "accept = " << this->accept << RESET << std::endl;
    isLine >> newKey;
    std::cout << YELLOW << "key = " << key << RESET << std::endl;
    if (key != newKey)
      findHeader(newKey, isLine);
  }
}

int countJumpLine(std::string str)
{
  int count = 0;

  for (int i = 0; str[i]; i++)
  {
    if (str[i] == '\n')
      count++;
  }
  return (count);
}

std::string	GET::createResponseBody(void)
{
std::string	responseBody =	"<!DOCTYPE html>\n"
                                "<html lang='en'>\n"
                                "<head>\n"
                                "	<meta charset='UTF-8'>\n"
                                "	<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
                                "	<title>Cool Page</title>\n"
                                "	<style>\n"
                                "		body { font-family: 'Courier New', monospace; background-color: #000000; color: #00FF00; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }\n"
                                "		h1 { font-size: 3rem; text-transform: uppercase; }\n"
                                "		p { font-size: 2rem; text-transform: uppercase; }\n"
                                "		.container { text-align: center; }\n"
                                "		img { max-width: 100%; height: auto; }\n"
                                "	</style>\n"
                                "</head>\n"
                                "<body>\n"
                                "	<div class='container'>\n"
                                "		<h1>Hello 42!</h1>\n"
                                "		<p>#BornToCode</p>\n"
                                // Insert your image link here
                                "		<a href='https://ibb.co/R4CMvMD'><img src='https://raw.githubusercontent.com/BenjaminLarger/BenjaminLarger/main/walk.gif' alt='aniversario42' border='0'></a>\n"                                
                                "	</div>\n"                                
                                "</body>\n"
                                "</html>\n";
    return (responseBody);								
}
void	GET::sendResponse(int clientFD)
{
	//The format of an HTTP response is defined by the HTTP specification (RFC 2616 for HTTP/1.1).
	//Body: The actual content (e.g., HTML, JSON).
	std::string	responseBody =	createResponseBody();
	//Here it is convenient to use ostring to concatenate
	std::ostringstream	response;
	//Status Line: Specifies the HTTP version, status code, and status message.
	response << "HTTP/1.1 200 OK\r\n";
	//Headers: Metadata about the response.
	response << "Content-Type : " << responseBody.size() << "\r\n";
	response << "Connection close\r\n";
	response << "\r\n";
	response << responseBody;

	std::string	responseStr = response.str();
	//send function is similar to write, but it is specific to socket.
	//Supports additional flags to modify behavior (e.g., MSG_NOSIGNAL to prevent sending a SIGPIPE signal).
	//Syntax: ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	if (send(clientFD, responseStr.c_str(), responseStr.size(), 0) == -1)
		throw (std::runtime_error("fail sending the message"));
}

GET::GET(Webserv server, int serverFD, int clientFD, std::string &clientInput)
{
  (void)server;
  (void)clientInput;
  (void)serverFD;
  (void)clientFD;

	std::cout << RED << countJumpLine(clientInput) << " jumplines in client request!" << RESET << std::endl;
  if (countJumpLine(clientInput) <= 3)
    return;
  std::istringstream isLine(clientInput);
  std::string key;

  std::cout << MAGENTA << clientInput << RESET << std::endl;
  isLine >> key;
  isLine >> this->pathToRessource;
  isLine >> this->HTTPversion;
  // END OF FIRST LINE

  // START PARSING HEADER AND BODY
  isLine >> key; // Header

  // Read client input => Using flag ? using a "readline"
  if (key == "Host:" || key == "User-Agent:" || key == "Accept:")
    findHeader(key, isLine);
  //write(clientFD, "GET client info\n", 14);
  std::cout << "pathToRessource = " << YELLOW << this->pathToRessource << RESET
            << std::endl;
  std::cout << "HTTPversion = " << YELLOW << this->HTTPversion << RESET
            << std::endl;
  std::cout << "host = " << YELLOW << this->host << RESET << std::endl;
  std::cout << "userAgent = " << YELLOW << this->userAgent << RESET
            << std::endl;
  std::cout << "accept = " << YELLOW << this->accept << RESET << std::endl;
  std::cout << RESET;
  clientInput.erase();
  try
  {
	  sendResponse(clientFD);
  }
  catch(const std::exception& e)
  {
	std::cerr << RED << e.what() << RESET <<'\n';
  }
  
}

GET::GET(){};

GET::~GET(){};