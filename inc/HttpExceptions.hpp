#ifndef HTTPEXCEPTIONS_HPP
#define HTTPEXCEPTIONS_HPP

#include <exception>
#include <string>

class HttpException : public std::exception
{
	private:
		std::string statusCode;
		std::string errorMessage;

	public:
		HttpException(const std::string &code, const std::string &message);
		virtual ~HttpException() throw();
		virtual const char *what() const throw();
		std::string getStatusCode() const;
		std::string getErrorMessage() const;
};

#endif