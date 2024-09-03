/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:57:51 by isporras          #+#    #+#             */
/*   Updated: 2024/09/03 18:08:58 by blarger          ###   ########.fr       */
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
