/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpExceptions.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 18:36:04 by isporras          #+#    #+#             */
/*   Updated: 2024/08/06 18:56:00 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpExceptions.hpp"

HttpException::HttpException(const std::string &code, const std::string &message)
	: statusCode(code), errorMessage(message) {}

HttpException::~HttpException() throw() {}

const char *HttpException::what() const throw() {
	return errorMessage.c_str();
}

std::string HttpException::getStatusCode() const {
	return statusCode;
}

std::string HttpException::getErrorMessage() const {
	return errorMessage;
}