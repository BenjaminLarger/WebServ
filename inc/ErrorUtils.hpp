/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:59 by demre             #+#    #+#             */
/*   Updated: 2024/08/14 12:21:41 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "core.hpp"
#include "dependencies.hpp"
#include "utils.hpp"

// Sends an error response to the client
// void sendErrorResponse(int clientSocket, int statusCode,
//                        const std::string &statusMessage, std::string errorBody);

// Finds and sends a default error page to the client
std::string composeErrorHtmlPage(int const &statusCode,
                                 std::string const &errorMessage,
                                 std::map<int, std::string> errorPages);
