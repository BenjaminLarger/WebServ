/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/08/02 17:30:12 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Dependencies.hpp"

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

#endif
