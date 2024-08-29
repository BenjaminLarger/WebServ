/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETdirectoryListing.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:25:42 by demre             #+#    #+#             */
/*   Updated: 2024/08/29 12:22:42 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "core.hpp"

std::vector<std::string> GET::listDirectoryContent(const std::string &path)
{
  std::vector<std::string> contents;

  // Open the directory specified by path.
  DIR *dir = opendir(path.c_str());
  if (dir)
  {
    // struct that represents a directory entry
    struct dirent *ent;
    // Reads the next directory entry until returns NULL
    while ((ent = readdir(dir)) != NULL)
    {
      // Add name of the directory entry (ent->d_name) to vector if it is a regular file
      if (ent->d_type == DT_REG)
        contents.push_back(ent->d_name);
    }
    closedir(dir);
  }
  else
  {
    throw HttpException(500, "Failed to open directory " + path);
  }
  return (contents);
}

std::string GET::generateDirectoryListing(
    const std::string &path, const std::vector<std::string> &contents)
{
    std::ostringstream html;

    // Comienza la generación de HTML con referencia al archivo CSS externo
    html << extractHtmlContentFromFile("./var/www/html/directory_listing.html");
    html << "<body>";

    html << "<div class=\"container\">";
    html << "<h1>Directory listing for " << path << "</h1><ul>";

    // Genera el listado de archivos
    for (std::vector<std::string>::const_iterator it = contents.begin(); it != contents.end(); ++it)
    {
        html << "<li><a href=\"" << "upload" << "/" << *it << "\">" << *it << "</a></li>";
    }
    html << "</ul></div>";

    html << "</body></html>";
  return (html.str());
}