/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTpng.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 20:02:46 by blarger           #+#    #+#             */
/*   Updated: 2024/09/01 16:56:36 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"
#include "utils.hpp"

std::vector<char> POST::extractBinaryContent(const std::vector<char>& content)
{
	trimBothEnds(boundary);
		std::vector<char> finalBoundary = getBoundaryEnd();
		
    std::vector<char>::const_iterator it = content.begin();
    std::vector<char>::const_iterator end = content.end();

    // Find the position of the first blank line (end of headers)
		std::cout << RED << "contentMap[2].name = " << contentMap[2].name << RESET << std::endl;
		std::string searchPattern = contentMap[2].name + "\r\n\r\n";
		std::vector<char>::const_iterator blankLinePos = std::search(it, end, searchPattern.begin(), searchPattern.end());
		std::cout << "searchPattern = " << searchPattern << "ended" << std::endl;
    if (blankLinePos == end)
    {
        // No blank line found, return an empty vector
        return std::vector<char>();
    }

    // Move iterator to the start of the binary content (after the blank line)
		std::cout << "contentMap[2].name size = " << contentMap[2].name.size() << RESET << std::endl;
    it = blankLinePos + contentMap[2].name.size() + 4;

    // Find the position of the final boundary
    std::vector<char>::const_iterator boundaryPos = std::search(it, end, finalBoundary.begin(), finalBoundary.end());
    if (boundaryPos == end)
    {
        // No final boundary found, return an empty vector
				std::cout << RED << "No final boundary found!\n" << RESET << std::endl;
        return std::vector<char>();
    }

    // Extract the binary content
    std::vector<char> binaryContent(it, boundaryPos);
    //std::cout << "extractBinaryContent " << std::string(binaryContent.begin(), binaryContent.end()) << std::endl;
    return (binaryContent);
}

std::vector<char>	POST::getBoundaryEnd()
{
    // Convert the boundary string to a vector of chars
    std::vector<char> boundaryVec(boundary.begin(), boundary.end());

		boundaryVec.insert(boundaryVec.begin(), '-');
		boundaryVec.insert(boundaryVec.begin(), '-');
    // Append the final boundary marker "--"
    boundaryVec.push_back('-');
    boundaryVec.push_back('-');

    std::cout << "Final Boundary: " << std::string(boundaryVec.begin(), boundaryVec.end()) << std::endl;

    return (boundaryVec);
}