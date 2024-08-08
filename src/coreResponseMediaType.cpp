/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coreResponseMediaType.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:44:04 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 19:53:20 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"

static std::map<std::string, std::string> createMediaTypeMap()
{
  std::map<std::string, std::string> mediaTypes;

  mediaTypes["txt"] = "text/plain";
  mediaTypes["html"] = "text/html";
  mediaTypes["htm"] = "text/html";
  mediaTypes["css"] = "text/css";
  mediaTypes["js"] = "application/javascript";
  mediaTypes["json"] = "application/json";
  mediaTypes["xml"] = "application/xml";
  mediaTypes["jpg"] = "image/jpeg";
  mediaTypes["jpeg"] = "image/jpeg";
  mediaTypes["png"] = "image/png";
  mediaTypes["gif"] = "image/gif";
  mediaTypes["webp"] = "image/webp";
  mediaTypes["svg"] = "image/svg+xml";
  mediaTypes["mp3"] = "audio/mpeg";
  mediaTypes["ogg"] = "audio/ogg";
  mediaTypes["wav"] = "audio/wav";
  mediaTypes["webm"] = "audio/webm";
  mediaTypes["mp4"] = "video/mp4";
  mediaTypes["avi"] = "video/x-msvideo";
  mediaTypes["pdf"] = "application/pdf";

  return (mediaTypes);
}

std::string getMediaType(const std::string &filename)
{
  // Initialize the media types map
  static const std::map<std::string, std::string> mediaTypes
      = createMediaTypeMap();

  // Find the last dot in the filename
  std::string::size_type dotIndex = filename.find_last_of('.');
  if (dotIndex != std::string::npos)
  {
    std::string extension = filename.substr(dotIndex + 1);

    for (std::string::iterator it = extension.begin(); it != extension.end();
         ++it)
      *it = tolower(*it);

    std::map<std::string, std::string>::const_iterator it
        = mediaTypes.find(extension);

    if (it != mediaTypes.end())
      return (it->second);
  }
  // Default to binary stream
  return ("application/octet-stream");
}
