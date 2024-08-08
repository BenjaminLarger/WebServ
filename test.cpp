#include <iostream>
#include <fstream>
#include <vector>

bool comparePNGFiles(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Error opening one of the files: " << file1 << " or " << file2 << std::endl;
        return false;
    }

    std::vector<char> buffer1((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
    std::vector<char> buffer2((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());

    if (buffer1.size() != buffer2.size()) {
        return false;
    }

    return std::equal(buffer1.begin(), buffer1.end(), buffer2.begin());
}

int main() {
    std::string file1 = "file1.png";
    std::string file2 = "file2.png";

    if (comparePNGFiles(file1, file2)) {
        std::cout << "The files are identical." << std::endl;
    } else {
        std::cout << "The files are different." << std::endl;
    }

    return 0;
}