/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:52:13 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 16:32:22 by demre            ###   ########.fr       */
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

// Returns a response to the client
std::string addOkResponseHeaderToBody(std::string responseBody);

// Returns a redirection header response (Location + Content-Length: 0)
std::string redirectionHeader(const int &code, const std::string &location);

// Returns the header status line
std::string getHeaderStatusLine(const int &code);

// Returns the reason phrase of a status code
std::string getReasonPhrase(const int &code);

#endif
