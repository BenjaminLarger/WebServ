#include <iostream>
#include <fstream>
#include <string>

void copyPngFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    // Open the input file in binary mode
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error opening input file: " << inputFilePath << std::endl;
        return;
    }

    // Read the contents of the file into a string
    std::string fileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
		std::cout << fileContents << std::endl;
    inputFile.close();

    // Open the output file in binary mode
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error opening output file: " << outputFilePath << std::endl;
        return;
    }

    // Write the contents of the string to the output file
    outputFile.write(fileContents.data(), fileContents.size());
    outputFile.close();
}

int main() {
    std::string inputFilePath = "upload/ufc.png";
    std::string outputFilePath = "upload/ufc_copy.png";
    copyPngFile(inputFilePath, outputFilePath);
    return 0;
}