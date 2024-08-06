/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/08/06 17:23:24 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

#include "ServerConfig.hpp"
#include "dependencies.hpp"
#include "core.hpp"

// Checks whether a string is all whitespace
bool isAllWhitespace(const std::string &str);

// Converts an int to a string
std::string intToString(int value);

// Checks if the input string stream has content after potentially removing a trailing semicolon
bool streamHasRemainingContent(std::stringstream &ss);
bool streamHasRemainingContent(std::istringstream &iss);

// Trims in place whitespace from both ends of a string
void trimBothEnds(std::string &line);

// Trims trailing whitespace of a string
void trimTrailingWS(std::string &line);

// Checks if vector has duplicate elements
bool hasDuplicates(const std::vector<int> &container);
bool hasDuplicates(const std::vector<std::string> &container);
bool hasDuplicates(const std::map<int, std::string> &container, int value);

// Display server configs in terminal to check
void displayServerConfigs(std::vector<ServerConfig> &serverConfigs);

// Extract the first world of a line
std::string extractFirstWord(const std::string &str);

// Extract the html content and return it as a string
std::string extractHtmlContent(const std::string &filePath);

// Send a response to the client
void sendResponse(int clientFD, std::string responseBody);

// Check if a line is empty
bool	lineIsEmpty(std::string line);

#endif
