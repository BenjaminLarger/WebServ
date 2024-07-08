/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dependencies.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:25:14 by blarger           #+#    #+#             */
/*   Updated: 2024/07/08 18:32:50 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

// ---------------------INCLUDE
#include <ostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <poll.h>
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// ---------------------COLORS
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[1;31m"
#define DARK_RED "\033[2;31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define ORANGE "\033[38;5;214m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[1; 37m"
#define GREY "\033[1;30m"

// ---------------------MESSAGES
#define DEFAULT_CONFIG_PATH "configuration_files/simple.conf"

#endif