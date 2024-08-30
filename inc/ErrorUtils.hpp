/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:59 by demre             #+#    #+#             */
/*   Updated: 2024/08/30 16:10:43 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "core.hpp"
#include "dependencies.hpp"
#include "utils.hpp"

// Finds and sends a default error page to the client
std::vector<char> composeErrorHtmlPage(int const &statusCode,
                                       std::string const &errorMessage,
                                       std::map<int, std::string> errorPages);
