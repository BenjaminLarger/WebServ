/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:56:16 by demre             #+#    #+#             */
/*   Updated: 2024/08/12 10:55:38 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ErrorUtils.hpp"
#include "Webserv.hpp"
#include "core.hpp"
#include "dependencies.hpp"

#define BODY_BEFORE_CONTENT_ERROR "ERROR: content cannot be defined after body!"
#define CONTENT_AFTER_BODY_ERROR                                               \
  "ERROR: body cannot be defined before content disposition!"
#define CLOSING_BOUNDARY_ERROR                                                 \
  "ERROR: Multipart/Form-Data must include closing boundary!"
#define HAS_NOT_BOUNDARY_ERROR                                                 \
  "ERROR: Multipart/Form-Data must include boundary separation!"
#define UPLOAD_FILE_DIR "./var/www/uploads/files/"
struct Content
{
  bool HasContentType;
  bool HasContentDisposition;
  bool HasBody;

  std::string contentDisposition;
  std::string contentType;
  std::string body;
  std::string name;
  std::string filename;
};

class POST
{
private:
  // Request line
	std::vector<char> clientInputVector;
	std::string clientInputString;
  std::string pathToResource;
  std::string HTTPversion;
  std::istringstream requestStream;
  // Header
  std::string host;
  std::string contentType;
  int contentLength;
	std::string boundary;

  // ServerConfig
  int ClientFD;
  std::map<int, Content> contentMap;
	std::map<std::string, std::string> _formValues;
  const ServerConfig &serverConfig;
	std::vector<char>	fileContentBinary;

  //BodyUplaod
  //Header

  //Util
  void extractFirstLine();
  void extractHeaders();
  std::string extractBody();
  bool saveInLogFile(std::map<std::string, std::string> formValues);
  std::map<std::string, std::string>	formValuestoMap(std::string body);
  int extractMultipartFormData();
  std::string createPostOkResponse(std::map<std::string, std::string> formValues);
	std::string createPostOkResponseWithFile(std::map<std::string, std::string> formValues);
	std::string createPostOkResponseWithFilename(std::map<std::string, std::string> formValues);

  //Util uplaod file
  void readAllRequest(void); //can delete before submit project
  std::map<int, std::string> headerUpload;
  int extractValues(std::string line, std::map<int, Content> &myMap, int index,
                    std::string key, const std::string &content);
  bool isBoundary(std::string line);
  std::string extractBoundary(const std::string &input);
  bool isClosingBoundary(std::string line);
  std::string makeCopy(const std::string &original);
  int parseContent(int index);
  void parseContentDisposition(int index, const std::string &content);
  void parseContentType(int index, std::string &content);
  bool hasClosingBoundary;
  std::string skipBoundaryPart(void);
  int handleFileUpload(int index);
	void	trimImageBody(std::string &binaryFileContent);
	void	handleBody(const std::string &line, int index);
	void	handleNewPart(int &index);
	std::vector<char>	getBoundaryEnd();
	std::vector<char> extractBinaryContent(const std::vector<char>& content);
	
	std::vector<char> extractPng(const std::vector<char>& content);
	std::vector<char> extractFile(const std::vector<char>& content, std::vector<char> finalBoundary);
	void savePngFile(const std::vector<char>& content, const std::string& filename);
	bool isValidPNG(const std::string& filename);//can delete before submit

  POST(const POST &);
  POST &operator=(const POST &);

public:
  POST(ClientInfo &client, int serverFD, int clientFD, std::vector<char> &clientInput,
       const ServerConfig &serverConfig);
  POST();
  ~POST(void);
};
