/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:59 by demre             #+#    #+#             */
/*   Updated: 2024/08/08 13:38:18 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

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

#endif
