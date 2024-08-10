/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTpng.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 20:02:46 by blarger           #+#    #+#             */
/*   Updated: 2024/08/10 20:07:11 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"
#include "utils.hpp"

std::vector<char> POST::extractPng(const std::vector<char>& content)
{
    // PNG file signature (8 bytes)
    const char pngSignatureArray[] = { '\x89', 'P', 'N', 'G', '\r', '\n', '\x1A', '\n' };
    std::vector<char> pngSignature(pngSignatureArray, pngSignatureArray + sizeof(pngSignatureArray) / sizeof(pngSignatureArray[0]));

    // PNG file end marker (12 bytes)
    const char pngEndMarkerArray[] = { '\x00', '\x00', '\x00', '\x00', 'I', 'E', 'N', 'D', '\xAE', '\x42', '\x60', '\x82' };
    std::vector<char> pngEndMarker(pngEndMarkerArray, pngEndMarkerArray + sizeof(pngEndMarkerArray) / sizeof(pngEndMarkerArray[0]));

    // Find the start of the PNG file
    std::vector<char>::const_iterator startIt = std::search(content.begin(), content.end(), pngSignature.begin(), pngSignature.end());
    if (startIt == content.end()) {
        throw std::runtime_error("PNG signature not found");
    }

    // Find the end of the PNG file
    std::vector<char>::const_iterator endIt = std::search(startIt, content.end(), pngEndMarker.begin(), pngEndMarker.end());
    if (endIt == content.end()) {
        throw std::runtime_error("PNG end marker not found");
    }

    // Include the end marker in the result
    endIt += pngEndMarker.size();

    // Extract the PNG data
    return std::vector<char>(startIt, endIt);
}

void POST::savePngFile(const std::vector<char>& content, const std::string& filename)
{
    // Open the file in binary mode
    std::ofstream outFile(filename.c_str(), std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Failed to open file for writing");
    }

    // Write the content to the file
    outFile.write(content.data(), content.size());
    if (!outFile) {
        throw std::runtime_error("Failed to write data to file");
    }

    // Close the file
    outFile.close();
}

bool POST::isValidPNG(const std::string& filename)//can delete before submit
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file)
		{
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    // Check the PNG signature
    const unsigned char pngSignature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    unsigned char fileSignature[8];
    file.read(reinterpret_cast<char*>(fileSignature), 8);
    if (!file || std::memcmp(pngSignature, fileSignature, 8) != 0) {
        return false;
    }

    // Check the IEND chunk at the end of the file
    const unsigned char iendChunk[12] = {0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
    file.seekg(-12, std::ios::end);
    unsigned char fileEnd[12];
    file.read(reinterpret_cast<char*>(fileEnd), 12);
    if (!file || std::memcmp(iendChunk, fileEnd, 12) != 0) {
        std::cout << "Bad iendChunk signature!\n";
        std::cout << "Expected: ";
        for (int i = 0; i < 12; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(iendChunk[i]) << " ";
        }
        std::cout << "\nActual:   ";
        for (int i = 0; i < 12; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(fileEnd[i]) << " ";
        }
        std::cout << std::endl;
        return false;
    }
		std::cout << GREEN << "ufc file valid !\n" << RESET;
    return true;
}