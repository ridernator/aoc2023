#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#define INPUT "../data/input"

#define NUM_RACES 4

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
    const auto& data = readFileToVector();
    std::string buffer = "";

    uint64_t time;
    uint64_t distance;

    for (const auto& character : data[0]) {
        if (std::isdigit(character)) {
            buffer += character;
        }
    }
    time = std::stoull(buffer);

    buffer.clear();
    for (const auto& character : data[1]) {
        if (std::isdigit(character)) {
            buffer += character;
        }
    }
    distance = std::stoull(buffer);

    uint64_t winCount = 0;

    for (uint64_t attempt = 1; attempt < time; ++attempt) {
        uint64_t thisDistance = attempt * (time - attempt);

        if (thisDistance > distance) {
            ++winCount;
        }
    }

    std::cout << "Number of ways to win = " << winCount << std::endl;
}
