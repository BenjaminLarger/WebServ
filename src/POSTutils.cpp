/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTutils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:20:52 by blarger           #+#    #+#             */
/*   Updated: 2024/08/09 19:30:49 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"

std::string POST::createPostOkResponse(std::map<std::string, std::string> formValues)
{
    std::stringstream httpResponse;
	std::string responseBody;
	
	responseBody = extractHtmlContentFromFile("./var/www/form/form_response.html");
	responseBody += "            <tbody>\n";
	responseBody += "                <tr>\n";
	responseBody += "                    <td>Name</td>\n";
	responseBody += "                    <td>" + formValues["name"] + "</td>\n";
	responseBody += "                </tr>\n";
	responseBody += "                <tr>\n";
	responseBody += "                    <td>Age</td>\n";
	responseBody += "                    <td>" + formValues["age"] + "</td>\n";
	responseBody += "                </tr>\n";
	responseBody += "            </tbody>\n";
	responseBody += "        </table>\n";
	responseBody += "        <a href=\"/\" class=\"button\">Back to Home</a>\n";
	responseBody += "    </div>\n";
	responseBody += "</body>\n";
	responseBody += "</html>\n";

    // Headers
	httpResponse << "HTTP/1.1 201 Created\r\n";
    httpResponse << "Content-Type: text/html\r\n";
    httpResponse << "Content-Length: " << responseBody.size() << "\r\n";
    httpResponse << "Date: " << getCurrentTimeHttpFormat() << "\r\n";
    httpResponse << "\r\n";
	httpResponse << responseBody;

	std::cout << GREEN << "Response: " << httpResponse.str() << std::endl;
	return (httpResponse.str());
}

bool POST::saveInLogFile(std::map<std::string, std::string> formValues)
{
	std::string	logFilePth = "./var/www/uploads/form_logs/form.log";
	std::ofstream logFileStream(logFilePth.c_str(), std::ios_base::app);

	if (!logFileStream.is_open())
	{
		throw HttpException(500, "Internal Server Error: Uneable to open logfile.");
		return (false);
	}
	else
	{
		for (std::map<std::string, std::string>::iterator it = formValues.begin(); it != formValues.end(); ++it)
			logFileStream << it->first << ": " << it->second << std::endl;
	}
	logFileStream << "----------------------------------------" << std::endl;
	logFileStream.close();
	
	return (true);
}

std::string generateClientID() {
    // Obtener la fecha y hora actual
    std::time_t now = std::time(0);
    std::tm* now_tm = std::localtime(&now);

    // Obtener los milisegundos actuales
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long ms = ts.tv_nsec / 1000000;

    // Convertir la fecha y hora a una cadena
    std::ostringstream ss;
    ss << (now_tm->tm_year + 1900)
       << std::setw(2) << std::setfill('0') << (now_tm->tm_mon + 1)
       << std::setw(2) << std::setfill('0') << now_tm->tm_mday
       << std::setw(2) << std::setfill('0') << now_tm->tm_hour
       << std::setw(2) << std::setfill('0') << now_tm->tm_min
       << std::setw(2) << std::setfill('0') << now_tm->tm_sec
       << std::setw(3) << std::setfill('0') << ms;

    return ss.str();
}
	
std::map<std::string, std::string>	POST::formValuestoMap(std::string body)
{
	std::map<std::string, std::string> formValues;
	std::istringstream bodyStream(body);
	std::string keyValuePair;

  while (std::getline(bodyStream, keyValuePair, '&'))
  {
    size_t pos = keyValuePair.find('=');
    if (pos != std::string::npos)
    {
      std::string key = keyValuePair.substr(0, pos);
      std::string value = keyValuePair.substr(pos + 1);
      formValues[key] = value;
    }
  }
  formValues["ID"] = generateClientID();
  return (formValues);
}

std::string POST::buildPostApplicationHtmlResponse(std::map<std::string, std::string> formValues)
{
  std::string responseBody;

  responseBody = "<html><body><h1>Form data received</h1><table>";
  for (std::map<std::string, std::string>::iterator it = formValues.begin();
       it != formValues.end(); ++it)
  {
    responseBody
        += "<tr><td>" + it->first + "</td><td>" + it->second + "</td></tr>";
  }
  return responseBody;
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
