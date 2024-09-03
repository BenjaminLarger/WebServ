/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTutils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:20:52 by blarger           #+#    #+#             */
/*   Updated: 2024/09/03 18:02:36 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"

std::vector<char> POST::createPostOkResponse(
    std::map<std::string, std::string> formValues, ClientRequest &clientReq)
{
  std::stringstream httpResponse;
  std::string responseBody;
  std::vector<char> charVecResponse;
  std::string sessionIdLine
      = getCookieRequestLine(clientReq);

  responseBody
      = extractHtmlContentFromFile("./var/www/form/form_response.html");
  responseBody += "            <tbody>\n";
  responseBody += "                <tr>\n";
  responseBody += "                    <td>Name</td>\n";
  responseBody += "                    <td>" + formValues["name"] + "</td>\n";
  responseBody += "                </tr>\n";
  responseBody += "                <tr>\n";
  responseBody += "                    <td>Age</td>\n";
  responseBody += "                    <td>" + formValues["age"] + "</td>\n";
  responseBody += "                </tr>\n";
  responseBody += "                <tr>\n";
  responseBody += "                    <td>Filename</td>\n";
  responseBody
      += "                    <td>" + contentMap[2].filename + "</td>\n";
  responseBody += "                </tr>\n";
  responseBody += "            </tbody>\n";
  responseBody += "        </table>\n";
  responseBody += "    </div>\n";
  responseBody += "    <div>\n";
  responseBody += "    </div>\n";
  responseBody += "</body>\n";
  responseBody += "</html>\n";

  // Headers
  httpResponse << "HTTP/1.1 201 Created\r\n";
  httpResponse << "Content-Type: text/html\r\n";
  httpResponse << "Content-Length: " << responseBody.size() << "\r\n";
  httpResponse << sessionIdLine;
  httpResponse << "Date: " << getHttpDate() << "\r\n";
  httpResponse << "\r\n";
  httpResponse << responseBody;

  std::string responseStr = httpResponse.str();
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());
  return (charVecResponse);
}

std::vector<char> POST::createPostOkResponsePlainText(const std::string &str,
																											ClientRequest &clientReq)
{
  std::stringstream httpResponse;
  std::vector<char> charVecResponse;
  std::string sessionIdLine
      = getCookieRequestLine(clientReq);

  // Headers
  httpResponse << "HTTP/1.1 201 Created\r\n";
  httpResponse << "Content-Type: text/plain\r\n";
  httpResponse << "Content-Length: " << str.size() << "\r\n";
  httpResponse << sessionIdLine;
  httpResponse << "Date: " << getHttpDate() << "\r\n";
  httpResponse << "\r\n";
  httpResponse << str;

  std::string responseStr = httpResponse.str();
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());
  return (charVecResponse);
}

std::vector<char> POST::createPostOkResponseWithFile(
    std::map<std::string, std::string> formValues, ClientRequest &clientReq)
{
  std::stringstream httpResponse;
  std::string responseBody;
  std::string filePath = UPLOAD_FILE_DIR + contentMap[2].filename;
  std::string reqPath = "/upload/" + contentMap[2].filename;
  std::vector<char> charVecResponse;
  std::string sessionIdLine
      = getCookieRequestLine(clientReq);

  // Check if the file exists
  std::ifstream file(filePath.c_str());
  if (!file.good())
    filePath = ""; // Set filePath to empty if file does not exist
  file.close();

  responseBody
      = extractHtmlContentFromFile("./var/www/form/form_response.html");
  responseBody += "            <tbody>\n";
  responseBody += "                <tr>\n";
  responseBody += "                    <td>Name</td>\n";
  responseBody += "                    <td>" + formValues["name"] + "</td>\n";
  responseBody += "                </tr>\n";
  responseBody += "                <tr>\n";
  responseBody += "                    <td>Age</td>\n";
  responseBody += "                    <td>" + formValues["age"] + "</td>\n";
  responseBody += "                </tr>\n";
  responseBody += "                <tr>\n";
  responseBody += "                    <td>Filename</td>\n";
  responseBody
      += "                    <td>" + contentMap[2].filename + "</td>\n";
  responseBody += "                </tr>\n";
  responseBody += "            </tbody>\n";
  responseBody += "        </table>\n";
  responseBody += "    </div>\n";
  responseBody += "    <div>\n";
  responseBody += "    </div>\n";
  responseBody += "</body>\n";
  responseBody += "</html>\n";
  if (!filePath.empty())
  {
    responseBody += "        <div style=\"text-align: center;\">\n";
    responseBody
        += "            <h2 style=\"color: white;\">File uploaded:</h2>\n";
    std::string fileExtension = filePath.substr(filePath.find_last_of(".") + 1);
    if (fileExtension == "mp4")
    {
      responseBody
          += "            <video width=\"320\" height=\"240\" controls>\n";
      responseBody += "                <source src=\"" + reqPath
                      + "\" type=\"video/mp4\">\n";
      responseBody
          += "                Your browser does not support the video tag.\n";
      responseBody += "            </video>\n";
    }
    else if (fileExtension == "pdf")
    {
      responseBody += "            <embed src=\"" + reqPath
                      + "\" width=\"600\" height=\"500\" alt=\"pdf\" />\n";
    }
    else if (fileExtension == "jpg" || fileExtension == "jpeg"
             || fileExtension == "png" || fileExtension == "gif")
    {
      responseBody += "            <img src=\"" + reqPath
                      + "\" alt=\"Uploaded Image\" style=\"display: block; "
                        "margin-left: auto; margin-right: auto;\" />\n";
    }
    else if (fileExtension == "mp3" || fileExtension == "wav")
    {
      responseBody += "            <audio controls>\n";
      responseBody += "                <source src=\"" + reqPath
                      + "\" type=\"audio/" + fileExtension + "\">\n";
      responseBody += "                Your browser does not support the audio "
                      "element.\n";
      responseBody += "            </audio>\n";
    }
    else if (fileExtension == "txt" || fileExtension == "csv")
    {
      responseBody += "            <iframe src=\"" + reqPath
                      + "\" width=\"600\" height=\"500\"></iframe>\n";
    }
    else if (fileExtension == "zip" || fileExtension == "tar"
             || fileExtension == "gz")
    {
      responseBody += "            <a href=\"" + reqPath
                      + "\" download>Download " + contentMap[2].filename
                      + "</a>\n";
    }
    else
    {
      responseBody += "            <p>Unsupported file type.</p>\n";
    }
    responseBody += "        </div>\n";
  }
  else
  {
    responseBody += "        <p>File not found.</p>\n";
  }
  responseBody += "    </div>\n";
  responseBody += "</body>\n";
  responseBody += "</html>\n";

  // Headers
  httpResponse << "HTTP/1.1 201 Created\r\n";
  httpResponse << "Content-Type: text/html\r\n";
  httpResponse << "Content-Length: " << responseBody.size() << "\r\n";
  httpResponse << sessionIdLine;
  httpResponse << "Date: " << getHttpDate() << "\r\n";
  httpResponse << "\r\n";
  httpResponse << responseBody;

  std::string responseStr = httpResponse.str();
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());
  return (charVecResponse);
}

bool POST::saveInLogFile(std::map<std::string, std::string> formValues)
{
  std::string logFilePth = "./var/www/uploads/form_logs/form.log";
  std::ofstream logFileStream(logFilePth.c_str(), std::ios_base::app);

  if (!logFileStream.is_open())
    throw HttpException(500, "Internal Server Error: Uneable to open logfile.");
  else
  {
    for (std::map<std::string, std::string>::iterator it = formValues.begin();
         it != formValues.end(); ++it)
      logFileStream << it->first << ": " << it->second << std::endl;
  }
  logFileStream << "----------------------------------------" << std::endl;
  logFileStream.close();

  return (true);
}

std::string generateClientID()
{
  // Obtener la fecha y hora actual
  std::time_t now = std::time(0);
  std::tm *now_tm = std::localtime(&now);

  // Obtener los milisegundos actuales
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  long ms = ts.tv_nsec / 1000000;

  // Convertir la fecha y hora a una cadena
  std::ostringstream ss;
  ss << (now_tm->tm_year + 1900) << std::setw(2) << std::setfill('0')
     << (now_tm->tm_mon + 1) << std::setw(2) << std::setfill('0')
     << now_tm->tm_mday << std::setw(2) << std::setfill('0') << now_tm->tm_hour
     << std::setw(2) << std::setfill('0') << now_tm->tm_min << std::setw(2)
     << std::setfill('0') << now_tm->tm_sec << std::setw(3) << std::setfill('0')
     << ms;

  return ss.str();
}

std::map<std::string, std::string> POST::formValuestoMap(std::string body)
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
  if (original.length() < 2)
  {
    return "";
  }
  std::string copy;
  for (size_t i = 2; i < original.length(); ++i)
  {
    copy += original[i];
  }
  trimBothEnds(copy);
  return (copy);
}

bool POST::isClosingBoundary(std::string line)
{
  if (line[line.size() - 1] == '\r' && line[line.size() - 2] == '-'
      && line[line.size() - 3] == '-')
  {
    if (strncmp(line.c_str(), boundary.c_str(), boundary.size()))
      return (true);
  }
  return (false);
}

bool POST::isBoundary(std::string line)
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

int POST::extractValues(std::string line, std::map<int, Content> &myMap,
                        int index, std::string key, const std::string &content)
{
  std::string values;

  /* if (contentMap[index].HasBody == true)
	{
		throw HttpException(400, "Bad request: body appears before content disposition.");
	} */
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

std::string POST::extractBoundary(const std::string &input)
{
  std::size_t pos = input.find('=');
  if (pos != std::string::npos)
  {
    std::string bound = input.substr(pos + 1);
    std::size_t endPos = bound.find('\r');
    if (endPos != std::string::npos)
    {
      return bound.substr(0, endPos);
    }
    return bound; // In case there is no '\r' character
  }
  return "";
}

void POST::handleBody(const std::string &line, int index)
{
  if (contentMap[index].HasBody == true)
    contentMap[index].body += '\n';
  contentMap[index].body += line;
  if (index == 0)
    _formValues["name"] = line;
  else if (index == 1)
    _formValues["age"] = line;
  _formValues["ID"] = generateClientID();
  contentMap[index].HasBody = true;
}

void POST::handleNewPart(int &index)
{
  index++;
  contentMap[index].HasContentType = false;
  contentMap[index].HasContentDisposition = false;
  contentMap[index].HasBody = false;
}

std::vector<char> POST::extractBinaryContent(const std::vector<char> &content)
{
  trimBothEnds(boundary);
  std::vector<char> finalBoundary = getBoundaryEnd();

  std::vector<char>::const_iterator it = content.begin();
  std::vector<char>::const_iterator end = content.end();

  // Find the position of the first blank line (end of headers)
  std::string searchPattern = contentMap[2].name + "\r\n\r\n";
  std::vector<char>::const_iterator blankLinePos
      = std::search(it, end, searchPattern.begin(), searchPattern.end());
  if (blankLinePos == end)
  {
    // No blank line found, return an empty vector
    return std::vector<char>();
  }

  // Move iterator to the start of the binary content (after the blank line)
  it = blankLinePos + contentMap[2].name.size() + 4;

  // Find the position of the final boundary
  std::vector<char>::const_iterator boundaryPos
      = std::search(it, end, finalBoundary.begin(), finalBoundary.end());
  if (boundaryPos == end)
  {
    // No final boundary found, return an empty vector
    return std::vector<char>();
  }

  // Extract the binary content
  std::vector<char> binaryContent(it, boundaryPos);
  return (binaryContent);
}

std::vector<char> POST::getBoundaryEnd()
{
  // Convert the boundary string to a vector of chars
  std::vector<char> boundaryVec(boundary.begin(), boundary.end());

  boundaryVec.insert(boundaryVec.begin(), '-');
  boundaryVec.insert(boundaryVec.begin(), '-');
  // Append the final boundary marker "--"
  boundaryVec.push_back('-');
  boundaryVec.push_back('-');

  return (boundaryVec);
}

std::string POST::skipBoundaryPart(void)
{
  std::string line;

  requestStream.clear();
  requestStream.seekg(0);
  while (std::getline(requestStream, line))
  {
    if (!strncmp(line.c_str(), "--", 2))
      break;
  }
  return (extractBoundary(contentType));
}