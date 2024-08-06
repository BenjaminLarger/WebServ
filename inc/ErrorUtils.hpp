/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:59 by demre             #+#    #+#             */
/*   Updated: 2024/08/06 19:24:09 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include "dependencies.hpp"
#include "Webserv.hpp"
#include "core.hpp"
#include "utils.hpp"

// Sends an error response to the client
void sendErrorResponse(int clientSocket, std::string statusCode,
                       const std::string &statusMessage, std::string errorBody);
// Finds and sends a default error page to the client
void sendDefaultErrorPage(int clientSocket, std::string statusCode, std::string errorMessage,
                        std::map<int, std::string> errorPages);

#endif
