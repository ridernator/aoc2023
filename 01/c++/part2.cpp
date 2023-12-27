#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <limits>

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
    auto lines = readFileToVector();
    std::string digits = "1234567890";
    std::array<std::string, 10> numbers = { "zero",
                                            "one",
                                            "two",
                                            "three",
                                            "four",
                                            "five",
                                            "six",
                                            "seven",
                                            "eight",
                                            "nine" };
    std::size_t firstPos;
    std::size_t secondPos;
    std::size_t firstNum;
    std::size_t secondNum;
    uint32_t total = 0;

    for (const auto& line : lines) {
        firstPos = line.find_first_of(digits);
        secondPos = line.find_last_of(digits);

        firstNum = line[firstPos] - '0';
        secondNum = line[secondPos] - '0';

        if (firstPos == std::string::npos) {
            firstPos = std::numeric_limits<std::size_t>::max();
        }

        if (secondPos == std::string::npos) {
            secondPos = 0;
        }

        for (std::size_t index = 0; index < numbers.size(); ++index) {
            std::size_t foundFirstIndex = line.find(numbers[index]);
            std::size_t foundSecondIndex = line.rfind(numbers[index]);

            if (foundFirstIndex != std::string::npos) {
                if (foundFirstIndex < firstPos) {
                    firstNum = index;
                    firstPos = foundFirstIndex;
                }

                if (foundSecondIndex > secondPos) {
                    secondNum = index;
                    secondPos = foundSecondIndex;
                }
            }
        }

        total += firstNum * 10;
        total += secondNum;
    }

    std::cout << "Sum of all calibration values is " << total << std::endl;

    return 0;
}