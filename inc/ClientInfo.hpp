/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 20:29:41 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 10:22:48 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CLIENT_INFO_HPP
#define CLIENT_INFO_HPP

#include "Dependencies.hpp"

class ClientInfo
{
private:
public:
  ClientInfo(/* args */);
  ~ClientInfo();

  int socketFD;
  size_t serverIndex; // Index of the server in serverConfigs vector
  // std::map<size_t, std::string> buffer;
  std::string buffer;
};

#endif