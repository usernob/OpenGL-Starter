#include <fstream>
#include <iostream>
#include <sstream>
#include <utils.hpp>

void utils::readFile(const char *filename, std::string &output) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "file: " << filename << std::endl;
        throw std::runtime_error("Failed to read file");
    }

    std::stringstream ss;
    std::string line;
    while (std::getline(file, line)) {
        ss << line << "\n";
    }

    output = ss.str();
    file.close();
}
