#include <algorithm>
#include <bitset>
#include <cinttypes>
#include <cmath>
#include <cstddef>
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

uint32_t isMatch(const std::string& line,
                 const std::vector<uint32_t>& positions) {
    uint32_t returnVal = 1;

    uint32_t totalHashes = 0;

    for (const auto& position : positions) {
        totalHashes += position;
    }

    if (totalHashes != std::count(line.begin(), line.end(), '#')) {
        return 0;
    }

    std::string tempString = "." + line + ".";
    std::replace(tempString.begin(), tempString.end(), '?', '.');

    std::string hashString;

    for (const auto& position : positions) {
        hashString = ".";
        for (uint32_t i = 0; i < position; ++i) {
            hashString += '#';
        }
        hashString += ".";

        size_t find = tempString.find(hashString);

        if (find == std::string::npos) {
            returnVal = 0;

            break;
        } else {
            tempString = tempString.substr(find + position);
        }
    }

    return returnVal;
}

int main() {
    auto rows = readFileToVector();

    std::vector<std::string> data;
    std::vector<std::vector<uint32_t>> positions;

    for (const auto& row : rows) {
        data.push_back(row.substr(0, row.find(' ')));

        std::istringstream rowIss(row.substr(row.find(' ') + 1));
        std::string tempString;
        std::vector<uint32_t> position;

        while (std::getline(rowIss, tempString, ',')) {
            position.push_back(std::stoul(tempString));
        }

        positions.push_back(position);
    }

    uint32_t sum = 0;

    for (std::size_t dataIndex = 0; dataIndex < data.size(); ++dataIndex) {
        std::vector<std::size_t> indicies;
        std::string lineCopy;

        for (std::size_t charIndex = 0; charIndex < data[dataIndex].size(); ++charIndex) {
            if (data[dataIndex][charIndex] == '?') {
                indicies.push_back(charIndex);
            }
        }

        uint32_t maxLoop = std::pow(2, indicies.size());

        for (uint32_t loopIndex = 0; loopIndex < maxLoop; ++loopIndex) {
            lineCopy = data[dataIndex];

            for (uint32_t subIndex = 0; subIndex < indicies.size(); ++subIndex) {
                if ((loopIndex & (1 << subIndex)) > 0) {
                    lineCopy[indicies[subIndex]] = '#';
                }
            }

            sum += isMatch(lineCopy, positions[dataIndex]);
        }
    }

    std::cout << "Sum of matches = " << sum << std::endl;
}
