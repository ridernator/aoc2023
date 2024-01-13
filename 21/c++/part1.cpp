#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
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

struct Pos {
    std::size_t x;
    std::size_t y;

    Pos(const std::size_t& x,
        const std::size_t& y) : x(x),
                                y(y) {}
};

void printGrid(const std::vector<std::string>& originalData,
               const std::vector<std::vector<std::vector<uint32_t>>>& data,
               const uint32_t step) {
    std::cout << std::endl;

    for (std::size_t y = 0; y < originalData.size(); ++y) {
        for (std::size_t x = 0; x < originalData[y].size(); ++x) {
            if (originalData[y][x] == '#') {
                std::cout << '#';
            } else {
                if (std::find(data[y][x].begin(), data[y][x].end(), step) != data[y][x].end()) {
                    std::cout << 'O';
                } else {
                    std::cout << '.';
                }
            }
        }

        std::cout << std::endl;
    }
}

void addNext(std::vector<Pos*>& nexts,
             const std::size_t x,
             const std::size_t y) {
    if (std::none_of(nexts.begin(), nexts.end(), [&x, &y] (const Pos* pos) {
        return ((pos->x == x) && (pos->y == y));
    })) {
        nexts.push_back(new Pos(x, y));
    }
}

int main() {
    uint32_t numSteps = 64;
    auto originalData = readFileToVector();

    std::vector<std::vector<std::vector<uint32_t>>> data;
    std::vector<Pos*> current;
    std::vector<Pos*> nexts;

    for (std::size_t y = 0; y < originalData.size(); ++y) {
        data.push_back({});

        for (std::size_t x = 0; x < originalData[y].size(); ++x) {
            data[y].push_back({});
        }
    }

    for (std::size_t y = 0; y < originalData.size(); ++y) {
        for (std::size_t x = 0; x < originalData[y].size(); ++x) {
            if (originalData[y][x] == 'S') {
                current.push_back(new Pos(x, y));

                std::cout << "Start point is " << x << "," << y << std::endl;

                break;
            }
        }
    }

    for (uint32_t step = 0; step <= numSteps; ++step) {
        for (auto& pos : current) {
            if (pos->x != 0) {
                if (originalData[pos->y][pos->x - 1] != '#') {
                    addNext(nexts, pos->x - 1, pos->y);
                }
            }

            if (pos->x != originalData[0].size() - 1) {
                if (originalData[pos->y][pos->x + 1] != '#') {
                    addNext(nexts, pos->x + 1, pos->y);
                }
            }

            if (pos->y != 0) {
                if (originalData[pos->y - 1][pos->x] != '#') {
                    addNext(nexts, pos->x, pos->y - 1);
                }
            }

            if (pos->y != originalData.size() - 1) {
                if (originalData[pos->y + 1][pos->x] != '#') {
                    addNext(nexts, pos->x, pos->y + 1);
                }
            }

            data[pos->y][pos->x].push_back(step);
            delete pos;
        }

        current = nexts;
        nexts.clear();

        //printGrid(originalData, data, step);
    }

    for (auto& pos : current) {
        delete pos;
    }

    uint32_t count = 0;

    for (std::size_t y = 0; y < data.size(); ++y) {
        for (std::size_t x = 0; x < data[y].size(); ++x) {
            if (std::find(data[y][x].begin(), data[y][x].end(), numSteps) != data[y][x].end()) {
                ++count;
            }
        }
    }

    std::cout << "Can reach " << count << " plots" << std::endl;
}
