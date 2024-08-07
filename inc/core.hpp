/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:52:13 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 20:28:03 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CORE_HPP
#define CORE_HPP

#include "HttpExceptions.hpp"
#include "dependencies.hpp"
#include "utils.hpp"

int sendall(int s, const char *buf, int len);

// Checks if the path is a directory
bool isDirectory(const std::string &path);

// Sends directly the given response to the client
void sendRGeneric(int clientFD, std::string responseStr);

// Extracts the html content and return it as a string
std::string extractHtmlContentFromFile(const std::string &filePath);

// Returns a 200 OK header with an HTML response in the given body
std::string composeOkHtmlResponse(std::string responseBody);

// Returns a redirection header response (Location + Content-Length: 0)
std::string createRedirectResponse(const int &code,
                                   const std::string &location);

// Returns the header status line
std::string getHeaderStatusLine(const int &code);

// Returns the reason phrase of a status code
std::string getReasonPhrase(const int &code);

// Reads the content of a binary file into a std::vector<char>
std::vector<char> readFile(const std::string &filename);

// Returns a 200 OK header with the binary content of a file. File type is determined from the file path
std::string composeFileResponse(const std::vector<char> &fileContent,
                                std::string filepath);

// Returns the media type (Content-Type) based on the file extension
std::string getMediaType(const std::string &filename);

// Checks if given path or any of its parent paths exist in a map of locations.
bool pathOrParentFolderExistsInLocations(
    const std::string &pathToResource,
    const std::map<std::string, LocationConfig> &locations,
    std::map<std::string, LocationConfig>::const_iterator &it);

#endif
