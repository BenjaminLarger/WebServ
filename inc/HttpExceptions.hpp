/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpExceptions.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:43:48 by demre             #+#    #+#             */
/*   Updated: 2024/08/30 16:43:50 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>

class HttpException : public std::exception
{
private:
  int statusCode;
  std::string errorMessage;

public:
  HttpException(const int &code, const std::string &message);
  virtual ~HttpException() throw();
  virtual const char *what() const throw();

  int getStatusCode() const;
  std::string getErrorMessage() const;
};
