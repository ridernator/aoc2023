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
    auto data = readFileToVector();

    for (std::size_t y = 0; y < data.size(); ++y) {
        for (std::size_t x = 0; x < data[y].size(); ++x) {
            if (data[y][x] == 'O') {
                int32_t backY;
                for (backY = y; backY >= 0; --backY) {
                    if (backY == 0) {
                        break;
                    } else if (data[backY - 1][x] != '.') {
                        break;
                    }
                }

                data[y][x] = '.';
                data[backY][x] = 'O';
            }
        }
    }

    // Count rocks
    uint64_t sum = 0;
    for (std::size_t y = 0; y < data.size(); ++y) {
        for (std::size_t x = 0; x < data[y].size(); ++x) {
            if (data[y][x] == 'O') {
                sum += data.size() - y;
            }
        }
    }

    std::cout << "Total load is " << sum << std::endl;
}
