/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:57:51 by isporras          #+#    #+#             */
/*   Updated: 2024/08/30 16:43:39 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ErrorUtils.hpp"
#include "HttpExceptions.hpp"
#include "Webserv.hpp"

class DELETE
{
public:
  DELETE(ClientInfo &client, const ServerConfig &serverConfig);
  ~DELETE(void);
};
