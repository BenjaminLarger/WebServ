/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 20:29:05 by demre             #+#    #+#             */
/*   Updated: 2024/08/30 16:05:22 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientInfo.hpp"

ClientInfo::ClientInfo(/* args */)
{
	this->req.bodyTooLarge = false;
	std::cout << GREEN << "shouldCloseConnection = false" << RESET << std::endl;
	this->req.shouldCloseConnection = false;
}

ClientInfo::~ClientInfo() {}
