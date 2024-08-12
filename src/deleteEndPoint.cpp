/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteEndPoint.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:35:47 by isporras          #+#    #+#             */
/*   Updated: 2024/08/12 11:20:27 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"

std::string GET::createHtmlDeleteRequest(std::vector<std::string> files, std::string uploadspth)
{
    std::ostringstream html;
    
    html << extractHtmlContentFromFile("./var/www/delete/delete_request.html");
    html << "<body>";
    html << "<h1>Files in " << uploadspth << "</h1>";
    html << "<table border='1'>";
    html << "<tr><th>File Name</th><th class = \"narrow\">Action</th></tr>";

    for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
    {
        html << "<tr>";
        html << "<td>" << *it << "</td>";
        html << "<td class = \"narrow\">";
        html << "<button onclick=\"deleteFile('" << *it << "', this)\">Delete</button>";
        html << "</td>";
        html << "</tr>";
    }

    html << "</table>";
    html << "</body>";
    html << "</html>";

    return html.str();
}

std::string GET::manageDeleteEndPoint()
{
  std::string uploadspth = "var/www/uploads/files";
  std::vector<std::string> files = listFilesInDirectory(uploadspth);
  std::string htmlDeleteBody;
  
  htmlDeleteBody = createHtmlDeleteRequest(files, uploadspth);
  return (htmlDeleteBody);
}