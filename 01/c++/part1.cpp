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
    auto lines = readFileToVector();
    std::string numbers="1234567890";
    std::size_t firstPos;
    std::size_t secondPos;
    uint32_t firstNum;
    uint32_t secondNum;
    uint32_t total = 0;

    for (const auto& line : lines) {
        firstPos = line.find_first_of(numbers);
        secondPos = line.find_last_of(numbers);

        firstNum = line[firstPos] - '0';
        secondNum = line[secondPos] - '0';

        total += firstNum * 10;
        total += secondNum;
    }

    std::cout << "Sum of all calibration values is " << total << std::endl;

    return 0;
}
