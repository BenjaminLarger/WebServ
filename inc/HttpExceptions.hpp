#ifndef HTTPEXCEPTIONS_HPP
#define HTTPEXCEPTIONS_HPP

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

#endif