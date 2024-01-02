#include <cstdint>
#include <iostream>
#include <algorithm>
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

typedef struct lens {
    std::string name;
    uint64_t focalLength;

    lens(const std::string& name,
         const uint64_t focalLength) : name(name),
                                       focalLength(focalLength) {};
} Lens;

typedef struct {
    std::vector<Lens*> lenses;
} Box;

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

    Box** boxes = new Box*[256];

    for (std::size_t index = 0; index < 256; ++index) {
        boxes[index] = new Box();
    }

    for (const auto& data : allData) {
        char operation = '=';
        if (data.find(operation) == std::string::npos) {
            operation = '-';
        }

        std::string lens = data.substr(0, data.find(operation));

        uint64_t focalLength = 0;

        if (operation == '=') {
            focalLength = std::stoull(data.substr(data.find(operation) + 1));
        }

        uint64_t box = 0;

        for (const auto& character : lens) {
            box += character;
            box *= 17;
            box %= 256;
        }

        if (operation == '=') {
            bool lensFound = false;

            for (auto& lensCandidate : boxes[box]->lenses) {
                if (lensCandidate->name.compare(lens) == 0) {
                    lensCandidate->focalLength = focalLength;
                    lensFound = true;

                    break;
                }
            }

            if (!lensFound) {
                boxes[box]->lenses.push_back(new Lens(lens, focalLength));
            }
        } else {
            boxes[box]->lenses.erase(std::remove_if(boxes[box]->lenses.begin(), boxes[box]->lenses.end(), [&lens] (const Lens* lensCandidate) {
                return lensCandidate->name.compare(lens) == 0;
            }), boxes[box]->lenses.end());
        }
    }

    // Calculate focusing power
    uint64_t focussingPower = 0;
    for (std::size_t boxIndex = 0; boxIndex < 256; ++boxIndex) {
        for (std::size_t lensIndex = 0; lensIndex < boxes[boxIndex]->lenses.size(); ++lensIndex) {
            focussingPower += (boxIndex + 1) * (lensIndex + 1) * boxes[boxIndex]->lenses[lensIndex]->focalLength;
        }
    }

    std::cout << "Total focussing power = " << focussingPower << std::endl;
}
