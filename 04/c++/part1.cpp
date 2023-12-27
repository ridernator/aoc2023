#include <cmath>
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
    const auto& rows = readFileToVector();
    uint32_t winners[10];
    uint32_t mine[25];
    uint32_t totalPoints = 0;

    for (const auto& row : rows) {
        uint32_t cardWinners = 0;
        sscanf(row.c_str(), "Card %u: %u %u %u %u %u %u %u %u %u %u | %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u",
               &winners[0],
               &winners[0], &winners[1], &winners[2], &winners[3], &winners[4],
               &winners[5], &winners[6], &winners[7], &winners[8], &winners[9],
               &mine[0], &mine[1], &mine[2], &mine[3], &mine[4],
               &mine[5], &mine[6], &mine[7], &mine[8], &mine[9],
               &mine[10], &mine[11], &mine[12], &mine[13], &mine[14],
               &mine[15], &mine[16], &mine[17], &mine[18], &mine[19],
               &mine[20], &mine[21], &mine[22], &mine[23], &mine[24]);

        for (const auto& my : mine) {
            for (const auto& winner : winners) {
                if (my == winner) {
                    ++cardWinners;

                    break;
                }
            }
        }

        if (cardWinners > 0) {
            totalPoints += std::pow(2, cardWinners - 1);
        }
    }

    std::cout << "Total points = " << totalPoints << std::endl;
}
