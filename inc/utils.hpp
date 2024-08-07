/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 19:07:35 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

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

// Display server configs in terminal to check
void displayServerConfigs(std::vector<ServerConfig> &serverConfigs);

// Extract the first world of a line
std::string extractFirstWord(const std::string &str);

// Check if a line is empty
bool lineIsEmpty(std::string line);

#endif
