/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dependencies.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:25:14 by blarger           #+#    #+#             */
/*   Updated: 2024/09/02 17:29:25 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// ---------------------INCLUDE
#include <algorithm>
#include <arpa/inet.h>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <ostream>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <ctime>

// ---------------------BUFFER
#define BUFFER_SIZE 4096

// ---------------------COLORS
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[1;31m"
#define DARK_RED "\033[2;31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define ORANGE "\033[38;5;214m"
#define BLUE "\033[34m"
#define BOLD_BLUE "\033[1;34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[1; 37m"
#define GREY "\033[1;30m"

// ---------------------FUNCTION RESTURNS
#define SUCCESS 1
#define FAILURE -1

// ---------------------UPLOAD DIRECTORY
#define DEFAULT_CONFIG_PATH "config/default.conf"
#define UPLOAD_FILE_DIR "./var/www/uploads/files/"
#define LOG_DIR_PATH "./var/www/uploads/form_logs/"
