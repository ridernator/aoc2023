#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <set>
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

bool isGear(const char value) {
    return (value == '*');
}

typedef struct numberDescriptor {
    std::size_t row;
    std::size_t col;
    std::size_t length;

    bool operator<(const numberDescriptor& other) const {
        return row * 10000 + col * 100 + length < other.row * 10000 + other.col * 100 + other.length;
    }
} NumberDescriptor;

NumberDescriptor getNumberDescriptor(const std::vector<std::string>& rows,
                                     const std::size_t row,
                                     const std::size_t col) {
    NumberDescriptor descriptor{row, 0, 0};

    std::size_t colIndex = col;

    while (std::isdigit(rows[row][colIndex])) {
        if (colIndex-- == 0) {
            break;
        }
    }

    descriptor.col = ++colIndex;

    while (std::isdigit(rows[row][colIndex])) {
        ++colIndex;
    }

    descriptor.length = colIndex - descriptor.col;

    return descriptor;
}

uint32_t getValue(const std::vector<std::string>& rows,
                  const NumberDescriptor& descriptor) {
    return std::stoul(rows[descriptor.row].substr(descriptor.col, descriptor.length));
}


uint32_t getGearRatio(const std::vector<std::string>& rows,
                      const std::size_t row,
                      const std::size_t col) {
    std::set<NumberDescriptor> numbers;
    std::size_t minRow = row;
    std::size_t maxRow = row + 1;
    std::size_t minCol = col;
    std::size_t maxCol = col + 1;

    if (row != 0) {
        --minRow;
    }

    if (col != 0) {
        --minCol;
    }

    if (row + 1 == rows.size()) {
        --maxRow;
    }

    if (col + 1 == rows[0].size()) {
        --maxCol;
    }
    
    for (std::size_t rowIndex = minRow; rowIndex <= maxRow; ++rowIndex) {
        for (std::size_t colIndex = minCol; colIndex <= maxCol; ++colIndex) {
            if (std::isdigit(rows[rowIndex][colIndex])) {
                numbers.insert(getNumberDescriptor(rows, rowIndex, colIndex));
            }
        }
    }

    if (numbers.size() == 2) {
        uint32_t ratio = 1;

        for (const auto& number : numbers) {
            ratio *= getValue(rows, number);
        }

        return ratio;
    }

    return 0;
}

int main() {
    auto rows = readFileToVector();
    uint32_t sum = 0;

    for (std::size_t rowIndex = 0; rowIndex < rows.size(); ++rowIndex) {
        for (std::size_t colIndex = 0; colIndex < rows[rowIndex].size(); ++colIndex) {
            if (isGear(rows[rowIndex][colIndex])) {
                sum += getGearRatio(rows, rowIndex, colIndex);
            }
        }
    }

    std::cout << "Sum of gear ratios is " << sum << std::endl;
}
