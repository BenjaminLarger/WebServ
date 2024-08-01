/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dependencies.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:25:14 by blarger           #+#    #+#             */
/*   Updated: 2024/08/01 11:01:40 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

// ---------------------INCLUDE
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <ostream>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <csignal>

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