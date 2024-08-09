/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:59 by demre             #+#    #+#             */
/*   Updated: 2024/08/09 13:28:15 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "core.hpp"
#include "dependencies.hpp"
#include "utils.hpp"

// Sends an error response to the client
void sendErrorResponse(int clientSocket, int statusCode,
                       const std::string &statusMessage, std::string errorBody);

// Finds and sends a default error page to the client
void sendDefaultErrorPage(int clientSocket, int statusCode,
                          std::string errorMessage,
                          std::map<int, std::string> errorPages);

