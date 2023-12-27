#include <cctype>
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

bool isSymbol(const char value) {
    if (std::isdigit(value)) {
        return false;
    } else if (value == '.') {
        return false;
    } else {
        return true;
    }
}

bool isPartNumber(const std::vector<std::string>& rows,
                  const std::size_t row,
                  const std::size_t col,
                  const std::size_t length) {
    std::size_t minRow = row;
    std::size_t maxRow = row + 1;
    std::size_t minCol = col;
    std::size_t maxCol = col + length;

    if (row != 0) {
        --minRow;
    }

    if (col != 0) {
        --minCol;
    }

    if (row + 1 == rows.size()) {
        --maxRow;
    }

    if (col + length == rows[0].size()) {
        --maxCol;
    }
    
    for (std::size_t rowIndex = minRow; rowIndex <= maxRow; ++rowIndex) {
        for (std::size_t colIndex = minCol; colIndex <= maxCol; ++colIndex) {
            if (isSymbol(rows[rowIndex][colIndex])) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    auto rows = readFileToVector();
    uint32_t sum = 0;

    for (std::size_t rowIndex = 0; rowIndex < rows.size(); ++rowIndex) {
        const auto& row = rows[rowIndex];

        for (std::size_t colIndex = 0; colIndex < row.size(); ++colIndex) {
            if (std::isdigit(row[colIndex])) {
                size_t length = 0;
                while ((std::isdigit(row[colIndex + length])) && (colIndex + length < row.size())) {
                    ++length;                    
                }

                if (isPartNumber(rows, rowIndex, colIndex, length)) {
                    sum += std::stoul(row.substr(colIndex, length));
                }

                colIndex += length;
            }
        }
    }

    std::cout << "Sum of part numbers is " << sum << std::endl;
}
