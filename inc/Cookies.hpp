/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 12:52:51 by blarger           #+#    #+#             */
/*   Updated: 2024/09/03 18:43:56 by blarger          ###   ########.fr       */
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

//find ID session present in the client request
std::string findSessionID(std::string request);

//find  the whole cookie line in the client request
std::string getCookieRequestLine(ClientRequest &req);

//Add closing time in cookie log
void logConnectionCloseTime(const std::string &sessionId);

//Add request type and time in cookie log
void logClientSessionRequest(ClientRequest &req);

//Save client session ID in client.req struct
void saveSessionIdClient(std::map<std::string, SessionData> &sessions,
                          const std::string &reqBuffer,
                          ClientRequest &clientReq);
