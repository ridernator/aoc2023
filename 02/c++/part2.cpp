#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

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
    uint32_t powerSum = 0;
    std::map<std::string, uint32_t> handfullColours;

    for (const auto& fullLine : lines) {
        handfullColours["red"] = 0;
        handfullColours["green"] = 0;
        handfullColours["blue"] = 0;

        std::string line = fullLine.substr(fullLine.find(":") + 1);
        
        std::string tempString;
        std::istringstream lineIss(line);
        std::vector<std::string> handfulls;

        while (std::getline(lineIss, tempString, ';')) {
            handfulls.push_back(tempString);
        }

        for (const auto& handfull : handfulls) {
            std::istringstream handfullIss(handfull);

            while (std::getline(handfullIss, tempString, ',')) {
                uint32_t number;
                char colour[8];
                
                sscanf(tempString.c_str(), " %u %s", &number, colour);
                handfullColours[colour] = std::max(number, handfullColours[colour]);
            }
        }

        powerSum += handfullColours["red"] * handfullColours["green"] * handfullColours["blue"];
    }

    std::cout << "Sum of powers is " << powerSum << std::endl;
}
