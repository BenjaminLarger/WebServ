/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETdirectoryListing.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:25:42 by demre             #+#    #+#             */
/*   Updated: 2024/08/30 15:16:51 by demre            ###   ########.fr       */
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
      if (std::string(ent->d_name) == "." || std::string(ent->d_name) == "..")
        continue;
      // Add name of the directory entry (ent->d_name) to vector if it is a regular file or directory
      if (ent->d_type == DT_REG)
        contents.push_back(ent->d_name);
      else if (ent->d_type == DT_DIR)
      {
        std::string name = ent->d_name;
        name += "/";
        contents.push_back(name);
      }
    }
    closedir(dir);
  }
  else
    throw HttpException(500, "Failed to open directory " + path);

  return (contents);
}

std::string GET::generateDirectoryListing(
    const std::string &path, const std::vector<std::string> &contents)
{
  std::ostringstream html;
  html << extractHtmlContentFromFile("./var/www/html/directory_listing.html");

  html << "<body>\n";
  html << "<div class=\"container\">\n";
  html << "<h1>Directory listing for " << path << "</h1>\n";
  html << "<ul>\n";

  // List all files and directories
  for (std::vector<std::string>::const_iterator it = contents.begin();
       it != contents.end(); ++it)
  {
    html << "<li>" << *it << "</li>\n";
  }

  html << "</ul>\n</div>\n";
  html << "</body>\n</html>\n";

  return (html.str());
}
