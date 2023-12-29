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

    std::string timesStr = data[0].substr(data[0].find(':') + 6);
    std::string distancesStr = data[1].substr(data[1].find(':') + 3);

    uint32_t times[NUM_RACES];
    uint32_t distances[NUM_RACES];

    uint32_t answer = 1;

    sscanf(timesStr.c_str(), "%u %u %u %u", &times[0], &times[1], &times[2], &times[3]);
    sscanf(distancesStr.c_str(), "%u %u %u %u", &distances[0], &distances[1], &distances[2], &distances[3]);
    
    for (std::size_t index = 0; index < NUM_RACES; ++index) {
        uint32_t winCount = 0;

        for (uint32_t time = 1; time < times[index]; ++time) {
            uint32_t distance = time * (times[index] - time);

            if (distance > distances[index]) {
                ++winCount;
            }
        }

        answer *= winCount;
    }

    std::cout << "Answer = " << answer << std::endl;
}
