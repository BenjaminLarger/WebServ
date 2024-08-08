/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpExceptions.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 18:36:04 by isporras          #+#    #+#             */
/*   Updated: 2024/08/08 13:59:01 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpExceptions.hpp"

HttpException::HttpException(const int &code, const std::string &message)
    : statusCode(code), errorMessage(message)
{
}

HttpException::~HttpException() throw() {}

const char *HttpException::what() const throw()
{
  return (errorMessage.c_str());
}

int HttpException::getStatusCode() const { return (statusCode); }

std::string HttpException::getErrorMessage() const { return (errorMessage); }