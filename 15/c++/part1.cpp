#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#define INPUT "../data/input"

/**
 * Read a file entirely into a stringstream
 *
 * @param The name of the file to read
 * @return The contents of the file
 **/
std::stringstream readFile(const std::string& filename = INPUT) {
    std::ifstream fileStream(filename);
    std::stringstream returnVal;

    returnVal << fileStream.rdbuf();

    return returnVal;
}

/**
 * Read a file entirely into a vector of strings
 *
 * @param The name of the file to read
 * @return The vector of lines in the file
 **/
std::vector<std::string> readFileToVector(const std::string& filename = INPUT) {
    std::ifstream fileStream(filename);
    std::string string;
    std::vector<std::string> returnVal;

    while (std::getline(fileStream, string)) {
        returnVal.push_back(string);
    }

    return returnVal;
}

int main() {
    auto rows = readFileToVector();

    // Read in data
    std::vector<std::string> allData;
    std::string tempString;
    for (const auto& row : rows) {
        std::istringstream rowIss(row);

        while (std::getline(rowIss, tempString, ',')) {
            allData.push_back(tempString);
        }
    }

    uint64_t hashSum = 0;
    for (const auto& data : allData) {
        uint64_t hash = 0;

        for (const auto& character : data) {
            hash += character;
            hash *= 17;
            hash %= 256;
        }

        hashSum += hash;
    }

    std::cout << "Sum of hashes = " << hashSum << std::endl;
}
