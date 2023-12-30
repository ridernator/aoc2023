#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
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

typedef struct {
    uint64_t x;
    uint64_t y;
} Galaxy;

int main() {
    auto data = readFileToVector();
    std::vector<Galaxy*> galaxies;

    for (std::size_t y = 0; y < data.size(); ++y) {
        for (std::size_t x = 0; x < data[0].size(); ++x) {
            if (data[y][x] == '#') {
                Galaxy* galaxy = new Galaxy();
                galaxy->x = x;
                galaxy->y = y;

                galaxies.push_back(galaxy);
            }
        }
    }

    std::vector<uint64_t> emptyRows;
    for (std::size_t index = 0; index < data.size(); ++index) {
        if (data[index].find('#') == std::string::npos) {
            emptyRows.push_back(index);
        }
    }

    std::vector<uint64_t> emptyCols;
    for (std::size_t col = 0; col < data[0].size(); ++col) {
        bool empty = true;

        for (std::size_t index = 0; index < data.size(); ++index) {
            if (data[index][col] == '#') {
                empty = false;

                break;
            }
        }

        if (empty) {
            emptyCols.push_back(col);
        }
    }

    uint64_t distanceSum = 0;

    for (std::size_t index1 = 0; index1 < galaxies.size(); ++index1) {
        for (std::size_t index2 = index1 + 1; index2 < galaxies.size(); ++index2) {
            uint64_t minXGalaxy = std::min(galaxies[index1]->x, galaxies[index2]->x);
            uint64_t maxXGalaxy = std::max(galaxies[index1]->x, galaxies[index2]->x);
            uint64_t minYGalaxy = std::min(galaxies[index1]->y, galaxies[index2]->y);
            uint64_t maxYGalaxy = std::max(galaxies[index1]->y, galaxies[index2]->y);

            distanceSum += maxXGalaxy - minXGalaxy;
            distanceSum += maxYGalaxy - minYGalaxy;

            for (uint64_t index = minXGalaxy; index < maxXGalaxy; ++index) {
                if (std::find(emptyCols.begin(), emptyCols.end(), index) != emptyCols.end()) {
                    distanceSum += 999999;
                }
            }

            for (uint64_t index = minYGalaxy; index < maxYGalaxy; ++index) {
                if (std::find(emptyRows.begin(), emptyRows.end(), index) != emptyRows.end()) {
                    distanceSum += 999999;
                }
            }
        }
    }

    std::cout << "Sum of shortest distances is " << distanceSum << std::endl;

    for (const auto& galaxy : galaxies) {
        delete galaxy;
    }
}
