/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 12:52:51 by blarger           #+#    #+#             */
/*   Updated: 2024/09/02 19:53:54 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

struct SessionData
{
    time_t connectionStart;
		time_t connectionEnd;
};

std::string generateSessionID();
std::string	findSessionID(std::string request);
std::string	handleCookiesSessions(std::map<std::string, SessionData> &sessions,
																	const std::string &reqBuffer,
																	ClientRequest &req);
bool addLogoutTimeCookies(std::string filePath, std::map<std::string, SessionData> &sessions, const std::string &sessionId);
