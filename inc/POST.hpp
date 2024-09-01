/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:56:16 by demre             #+#    #+#             */
/*   Updated: 2024/09/01 16:56:41 by blarger          ###   ########.fr       */
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

  //Util
  void extractFirstLine();
  void extractHeaders();
  std::string extractBody();
  bool saveInLogFile(std::map<std::string, std::string> formValues);
  std::map<std::string, std::string>	formValuestoMap(std::string body);
  std::vector<char> createPostOkResponse(std::map<std::string, std::string> formValues);
	std::vector<char> createPostOkResponseWithFile(std::map<std::string, std::string> formValues);
	std::vector<char> createPostOkResponseWithFilename(std::map<std::string, std::string> formValues);

  //Util uplaod file
  int extractMultipartFormData(std::string &_boundary);
  void readAllRequest(void); //can delete before submit project
  std::map<int, std::string> headerUpload;
	//Parsing
  int extractValues(std::string line, std::map<int, Content> &myMap, int index,
                    std::string key, const std::string &content);
  int parseContent(int index);
  void parseContentDisposition(int index, const std::string &content);
  void parseContentType(int index, std::string &content);
	void	handleBody(const std::string &line, int index);
	void	handleNewPart(int &index);
	
	//CGI
	void	setPostEnvVariables();

	//Boundary									
  bool isBoundary(std::string line);
  std::string extractBoundary(const std::string &input);
  bool isClosingBoundary(std::string line);
  bool hasClosingBoundary;
  std::string makeCopy(const std::string &original);
  std::string skipBoundaryPart(void);
	std::vector<char>	getBoundaryEnd();
	//Upload
  int handleFileUpload(int index);
	void	trimImageBody(std::string &binaryFileContent);
	std::vector<char> extractBinaryContent(const std::vector<char>& content);
	
	std::vector<char> extractFile(const std::vector<char>& content, std::vector<char> finalBoundary);

  POST(const POST &);
  POST &operator=(const POST &);

public:
  POST(Webserv &webserv, ClientInfo &client, int clientFD, std::vector<char> &clientInput,
       const ServerConfig &serverConfig, std::string &boundary);
  POST();
  ~POST(void);
};
