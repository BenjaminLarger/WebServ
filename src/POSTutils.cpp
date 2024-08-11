/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTutils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:20:52 by blarger           #+#    #+#             */
/*   Updated: 2024/08/11 17:40:32 by blarger          ###   ########.fr       */
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

	return (httpResponse.str());
}

bool POST::saveInLogFile(std::map<std::string, std::string> formValues)
{
	std::string	logFilePth = "./var/www/uploads/form_logs/form.log";
	std::ofstream logFileStream(logFilePth.c_str(), std::ios_base::app);

	if (!logFileStream.is_open())
		throw HttpException(500, "Internal Server Error: Uneable to open logfile.");
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

std::string POST::makeCopy(const std::string &original)
{
  if (original.length() < 4)
  {
    return "";
  }
  std::string copy;
  for (size_t i = 4; i < original.length(); ++i)
  {
    copy += original[i];
  }
  std::cout << RED << copy << RESET << std::endl;
  return copy;
}

bool	POST::isClosingBoundary(std::string line)
{
	if (line[line.size() - 1] == '\r' && line[line.size() - 2] == '-' && line[line.size() - 3] == '-')
	{
		if (strncmp(line.c_str(), boundary.c_str(), boundary.size()))
			return (true);
	}
	return (false);
}

bool	POST::isBoundary(std::string line)
{
	std::string newline;
	if (line[0] == '-' && line[1] == '-')
	{
		newline = makeCopy(line);
		if (!strcmp(newline.c_str(), boundary.c_str()))
		{
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
		throw HttpException(400, "Bad request: body appears before content disposition.");
	}
	values = line.substr(key.size() + 1);
	if (content == "Content disposition")
		myMap[index].contentDisposition = values;
	else if (content == "Content type")
		myMap[index].contentType = values;
	if (content == "Content disposition")
		myMap[index].HasContentDisposition = true;
	else if (content == "Content type")
		myMap[index].HasContentType = true;
	return (SUCCESS);
}

std::string POST::extractBoundary(const std::string& input)
{
   std::size_t pos = input.find('=');
  if (pos != std::string::npos)
	{
		if (input.substr(pos + 1)[0] == '-' && input.substr(pos + 1)[1] == '-')
			return input.substr(pos + 3);
	}
	return "";
}

void	POST::readAllRequest()//delete
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
	 while (std::getline(requestStream, line))
	 {
		if (!strncmp(line.c_str(), "--", 2))
			break;		
	 }
	 std::cout << "------------------------------------__\n";
	return (extractBoundary(contentType));
}

void	POST::handleBody(const std::string &line, int index)
{
	if (contentMap[index].HasBody == true)
				contentMap[index].body += '\n';
			contentMap[index].body += line;
			contentMap[index].HasBody = true;
}

void	POST::handleNewPart(int index)
{
	index++;
		contentMap[index].HasContentType = false;
		contentMap[index].HasContentDisposition = false;
		contentMap[index].HasBody = false;
}
