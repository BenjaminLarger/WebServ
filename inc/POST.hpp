/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:56:16 by demre             #+#    #+#             */
/*   Updated: 2024/08/06 12:36:07 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef POST_HPP
#define POST_HPP

#include "ErrorUtils.hpp"
#include "Webserv.hpp"
#include "core.hpp"

#define BODY_BEFORE_CONTENT_ERROR "ERROR: content cannot be defined after body!"
#define CONTENT_AFTER_BODY_ERROR                                               \
  "ERROR: body cannot be defined before content disposition!"
#define CLOSING_BOUNDARY_ERROR                                                 \
  "ERROR: Multipart/Form-Data must include closing boundary!"
#define HAS_NOT_BOUNDARY_ERROR                                                 \
  "ERROR: Multipart/Form-Data must include boundary separation!"

class Content {
	bool	contentHasContentTypeTypeMap;
	bool	HasContentDisposition;
	bool	HasBody;
	std::string	contentDisposition;
	std::string contentType;
	std::string	body;	
};

class POST
{
private:
  // Request line
  std::map<std::string, std::string> headers;
  std::string pathToRessource;
  std::string HTTPversion;
  std::istringstream requestStream;
  // Header
  std::string host;
  std::string contentType;
  int contentLength;
  // Body
  std::string body;

  //BodyUplaod
  //Header
  std::map<int, std::string> contentDispositionMap;
  std::map<int, std::string> contentTypeMap;
  std::map<int, std::string> bodyMap;
  std::map<int, bool> HasContentType;
  std::map<int, bool> HasContentDisposition;
  std::map<int, bool> HasBody;
	std::map<int, Content> contentMap;
  int ClientFD;
  //Util
  void extractFirstLine();
  void extractHeaders();
  void extractBody(int clientFD);
  int extractMultipartFormData();

  //Util uplaod file
  void readAllRequest(void); //can delete before submit project
  std::map<int, std::string> headerUpload;
  int extractValues(std::string line, std::map<int, std::string> &myMap,
                    int index, std::string key,
                    std::map<int, bool> &HasContent);
  bool isBoundary(std::string line, std::string boundary);
  std::string extractBoundary(const std::string &input);
  bool isClosingBoundary(std::string line, std::string boundary);
  std::string makeCopy(const std::string &original);
  int parseContent(int index);
	void	parseContentDisposition(int index, const std::string &content);

  bool hasClosingBoundary;
  std::string skipBoundaryPart(void);

public:
  POST(int serverFD, int clientFD, std::string &clientInput);
  POST();
  ~POST(void);
};

#endif