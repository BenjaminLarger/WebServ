#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip> // For std::hex and std::setw

bool comparePNGFiles(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Error opening one of the files: " << file1 << " or " << file2 << std::endl;
        return false;
    }

    std::vector<char> buffer1((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
    std::vector<char> buffer2((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());

	std::cout << "SIZE :" << buffer1.size() << " || " << buffer2.size() << std::endl;
    if (buffer1.size() != buffer2.size()) {
        std::cout << "Files have different sizes." << std::endl;
    }

    bool areIdentical = true;
	//	std::cout << "CONTENT :" << buffer1 << " || " << buffer2 << std::endl;
    for (size_t i = 0; i < buffer1.size(); ++i) {
        if (buffer1[i] != buffer2[i]) {
            areIdentical = false;
            std::cout << "Difference at byte " << i << ": "
                      << "file1 = 0x" << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(buffer1[i]) & 0xFF) << ", "
                      << "file2 = 0x" << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(buffer2[i]) & 0xFF) << std::dec << std::endl;
        }
				if (i > 500)
					break ;
    }

    return areIdentical;
}

int main() {
    std::string file1 = "upload/ufc.png";
    std::string file2 = "upload/ufc_original.png";

    if (comparePNGFiles(file1, file2)) {
        std::cout << "The files are identical." << std::endl;
    } else {
        std::cout << "The files are different." << std::endl;
    }

    return 0;
}