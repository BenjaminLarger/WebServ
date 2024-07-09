/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientPetition.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:23:27 by blarger           #+#    #+#             */
/*   Updated: 2024/07/09 11:27:24 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

#ifndef CLIENT_PETITION_HPP
#define CLIENT_PETITION_HPP

#include "Webserv.hpp"

class GET : public Webserv
{

private:
	// Request line
	std::string pathToRessource;
	std::string HTTPversion;
	// Header
	std::string host;
	std::string userAgent;
	std::string accept;
	// Body
	std::string body;

public:
	GET(Webserv _server, int serverFD, int clientFD, std::string clientInput);
	GET();
	~GET(void);

	void findHeader(std::string &key, std::istringstream &isLine);
};

#endif