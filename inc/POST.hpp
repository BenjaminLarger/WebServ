/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:56:16 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 13:51:44 by isporras         ###   ########.fr       */
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

struct Content {
		bool	HasContentType;
		bool	HasContentDisposition;
		bool	HasBody;
		
		std::string	contentDisposition;
		std::string contentType;
		std::string	body;
		std::string	name;
		std::string	filename;
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
	
	std::map<int, Content> contentMap;
  int ClientFD;
  //Util
  void        extractFirstLine();
  void        extractHeaders();
  void        extractBody(int clientFD);
  std::string buildPostHtmlResponse();
  int         extractMultipartFormData();
  void        sendResponse(int clientFD, std::string responseBody);

  //Util uplaod file
  void readAllRequest(void); //can delete before submit project
  std::map<int, std::string> headerUpload;
  int	extractValues(std::string line, std::map<int, Content> &myMap, int index, std::string key, const std::string &content);
  bool isBoundary(std::string line, std::string boundary);
  std::string extractBoundary(const std::string &input);
  bool isClosingBoundary(std::string line, std::string boundary);
  std::string makeCopy(const std::string &original);
  int parseContent(int index);
	void	parseContentDisposition(int index, const std::string &content);
	void	parseContentType(int index, std::string &content);
  bool hasClosingBoundary;
  std::string skipBoundaryPart(void);
	int handleFileUpload(int index);

	POST(const POST&);
  POST& operator=(const POST&);
	
public:
  POST(int serverFD, int clientFD, std::string &clientInput);
  POST();
  ~POST(void);
};

#endif