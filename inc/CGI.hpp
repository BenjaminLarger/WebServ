/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:46:37 by demre             #+#    #+#             */
/*   Updated: 2024/08/09 13:27:43 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

std::string executePhp(std::string pathToRessource);

