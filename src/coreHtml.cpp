/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coreHtml.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:45:04 by demre             #+#    #+#             */
/*   Updated: 2024/09/04 12:53:15 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"
#include "dependencies.hpp"
#include "utils.hpp"

std::string createFileListHtml(const std::string &dirPath)
{
  std::vector<std::string> files = listFilesInDirectory(dirPath);
  std::string html = "<html><body><h2>Files in Directory:</h2><ul>";

  for (std::vector<std::string>::const_iterator it = files.begin();
       it != files.end(); ++it)
    html += "<li><a href=\"/delete?file=upload" + *it + "\">" + *it + "</a></li>";
  html += "</ul></body></html>";

  return html;
}