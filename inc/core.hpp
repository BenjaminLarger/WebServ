/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:52:13 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 12:40:30 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CORE_HPP
#define CORE_HPP

#include "dependencies.hpp"
#include "utils.hpp"

int sendall(int s, const char *buf, int len);

// Checks if the path is a directory
bool isDirectory(const std::string &path);
// Sends directly the given response to the client
void sendRGeneric(int clientFD, std::string responseStr);


#endif
