/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 12:52:51 by blarger           #+#    #+#             */
/*   Updated: 2024/08/12 13:37:09 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

class Cookies
{
private:
  Cookies(const Cookies &);
  Cookies &operator=(const Cookies &);

public:
  Cookies
		(ClientInfo &client, int serverFD, int clientFD, std::vector<char> &clientInput,
       const ServerConfig &serverConfig);
  ~Cookies(void);
};