/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/08/04 11:26:59 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Dependencies.hpp"
#include "ServerConfig.hpp"

int sendall(int s, const char *buf, int len);

// Checks whether a string is all whitespace
bool isAllWhitespace(const std::string &str);

// Converts an int to a string
std::string intToString(int value);

// Checks if the input string stream has content after potentially removing a trailing semicolon
bool checkStreamForRemainingContent(std::stringstream &ss);

// Trims in place whitespace from both ends of a string
void trimBothEnds(std::string &line);

// Trims trailing whitespace of a string
void trimTrailingWS(std::string &line);

// Checks if vector has duplicate elements
bool hasDuplicates(const std::vector<int> &vec);
bool hasDuplicates(const std::vector<std::string> &vec);

// Display server configs in terminal to check
void displayServerConfigs(std::vector<ServerConfig> &serverConfigs);

// Extract the first world of a line
std::string extractFirstWord(const std::string& str);

#endif
