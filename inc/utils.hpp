/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/09/02 17:11:37 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientInfo.hpp"
#include "ServerConfig.hpp"
#include "core.hpp"
#include "dependencies.hpp"

// Checks whether a string is all whitespace
bool isAllWhitespace(const std::string &str);

// Converts a number to a string
std::string toString(int num);
std::string toString(size_t num);

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

// Display server configs in the terminal
void displayServerConfigs(std::vector<ServerConfig> const &serverConfigs);
// Display client request location data in the terminal
void displayClientRequestLocationData(ClientInfo const &client);
// Display parsed header request in the terminal
void displayParsedHeaderRequest(ClientInfo const &client);

// Extract the first world of a line
std::string extractFirstWord(const std::string &str);

// Check if a line is empty
bool lineIsEmpty(std::string line);

// Delete quotes of a string
std::string trimQuotes(std::string &str);

int countJumpLine(std::string str);

std::string getCurrentTimeHttpFormat();

// Ensures there is a leading "/" and no trailing "/"
std::string formatPath(const std::string &path);

// From last "/" of string: Extension is after last ".". FileName is between last "/" and last "."
void getFileNameAndExtension(const std::string &path, std::string &fileName,
                             std::string &extension);

//Check if request is complete
bool hasBlankLineInput(std::string &clientInput, std::string &boundary, ClientInfo &client);

//Delete the content of log file
void	deleteLogContentFile(const std::string &filePath);