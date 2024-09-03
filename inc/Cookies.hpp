/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 12:52:51 by blarger           #+#    #+#             */
/*   Updated: 2024/09/03 17:45:18 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

struct SessionData
{
  std::string URI;
  time_t connectionTime;
  time_t connectionStart;
  time_t connectionEnd;
};

std::string generateSessionID();

std::string findSessionID(std::string request);

std::string handleCookiesSessions(std::map<std::string, SessionData> &sessions,
                                  const std::string &reqBuffer,
                                  ClientRequest &req);

void logConnectionCloseTime(const std::string &sessionId);

void logClientSessionRequest(ClientRequest &req);

void checkSessionIdClient(std::map<std::string, SessionData> &sessions,
                          const std::string &reqBuffer,
                          ClientRequest &clientReq);
