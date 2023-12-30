#include <algorithm>
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

int main() {
    const auto& rows = readFileToVector();
    std::vector<std::vector<std::vector<int64_t>>> data;

    // Read data
    for (const auto& row : rows) {
        std::istringstream rowSs(row);
        std::string tempString;
        std::vector<std::vector<int64_t>> datum;

        datum.push_back({});
        while (std::getline(rowSs, tempString, ' ')) {
            datum[0].push_back(std::stoll(tempString));
        }

        data.push_back(datum);
    }

    // Add extra rows
    for (auto& datum : data) {
        std::size_t index = 0;
        while (!std::all_of(datum[index].begin(), datum[index].end(), [] (const int64_t& value) { return value == 0; })) {
            ++index;
            std::vector<int64_t> nextRow;

            for (std::size_t i = 0; i < datum[index - 1].size() - 1; ++i) {
                nextRow.push_back(datum[index - 1][i + 1] - datum[index - 1][i]);
            }

            datum.push_back(nextRow);
        }
    }

    for (const auto& row : data[2]) {
        for (const auto& value : row) {
            std::cout << " " << value;
        }
        std::cout << std::endl;
    }

    int64_t sum = 0;

    for (auto& datum : data) {
        for (uint32_t index = datum.size() - 1; index > 0; --index) {
            std::cout << "Index: " << index << std::endl;
            datum[index - 1].push_back(datum[index - 1][datum[index - 1].size() - 1] + datum[index][datum[index].size() - 1]);
        }

        sum += datum[0][datum[0].size() - 1];
    }
    for (const auto& row : data[2]) {
        for (const auto& value : row) {
            std::cout << " " << value;
        }
        std::cout << std::endl;
    }

    std::cout << "Sum of values is " << sum << std::endl;
}
