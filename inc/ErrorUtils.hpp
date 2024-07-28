#ifndef UTILS_H
#define UTILS_H

#include "Dependencies.hpp"

void sendErrorResponse(int clientSocket, std::string statusCode, const std::string& statusMessage);

#endif