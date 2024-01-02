#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
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

typedef struct position {
    std::size_t x;
    std::size_t y;

    position(const std::size_t x,
             const std::size_t y) : x(x),
                                    y(y) {};
} Position;

void roll(bool** data,
          std::vector<Position*>& rocks,
          const std::size_t maxX,
          const std::size_t maxY) {
    std::size_t back;

    std::sort(rocks.begin(), rocks.end(), [] (const Position* a,
                                              const Position* b) {
        return a->y < b->y;
    });

    // North
    for (auto& rock : rocks) {
        for (back = rock->y; back > 0; --back) {
            if (data[back - 1][rock->x]) {
                break;
            }
        }

        if (back != rock->y) {
            data[rock->y][rock->x] = false;
            data[back][rock->x] = true;
            rock->y = back;
        }
    }

    std::sort(rocks.begin(), rocks.end(), [] (const Position* a,
                                              const Position* b) {
        return a->x < b->x;
    });

    // West
    for (auto& rock : rocks) {
        for (back = rock->x; back > 0; --back) {
            if (data[rock->y][back - 1]) {
                break;
            }
        }

        if (back != rock->x) {
            data[rock->y][rock->x] = false;
            data[rock->y][back] = true;
            rock->x = back;
        }
    }

    std::sort(rocks.begin(), rocks.end(), [] (const Position* a,
                                              const Position* b) {
        return a->y > b->y;
    });

    // South
    for (auto& rock : rocks) {
        for (back = rock->y; back < maxY - 1; ++back) {
            if (data[back + 1][rock->x]) {
                break;
            }
        }

        if (back != rock->y) {
            data[rock->y][rock->x] = false;
            data[back][rock->x] = true;
            rock->y = back;
        }
    }

    std::sort(rocks.begin(), rocks.end(), [] (const Position* a,
                                              const Position* b) {
        return a->x > b->x;
    });

    // East
    for (auto& rock : rocks) {
        for (back = rock->x; back < maxX - 1; ++back) {
            if (data[rock->y][back + 1]) {
                break;
            }
        }

        if (back != rock->x) {
            data[rock->y][rock->x] = false;
            data[rock->y][back] = true;
            rock->x = back;
        }
    }
}

uint64_t calculateHash(const std::vector<Position*>& rocks) {
    uint64_t hash = 0;

    for (const auto& rock : rocks) {
        hash += rock->x;
        hash += 10000 * rock->y;
    }

    return hash;
}

int main() {
    auto dataS = readFileToVector();
    std::size_t maxX = dataS[0].size();
    std::size_t maxY = dataS.size();
    bool** data = new bool*[maxY];
    std::vector<Position*> rocks;

    // Populate rocks and data
    for (std::size_t y = 0; y < maxY; ++y) {
        data[y] = new bool[maxX];

        for (std::size_t x = 0; x < maxX; ++x) {
            if (dataS[y][x] == 'O') {
                rocks.push_back(new Position(x, y));
                data[y][x] = true;
            } else if (dataS[y][x] == '#') {
                data[y][x] = true;
            } else {
                data[y][x] = false;
            }
        }
    }

    // Find loop
    std::map<uint64_t, uint64_t> hashes;
    bool cycleFound = false;
    for (uint64_t i = 0; i < 1000000000; ++i) {
        if (!cycleFound) {
            hashes[i] = calculateHash(rocks);

            for (uint64_t hashIndex = 0; hashIndex < i; ++hashIndex) {
                if (hashes[hashIndex] == hashes[i]) {
                    uint64_t iCopy = i;

                    while (i < 1000000000 - (iCopy - hashIndex)) {
                        i += iCopy - hashIndex;
                    }

                    cycleFound = true;
                    break;
                }
            }
        }

        roll(data, rocks, maxX, maxY);
    }

    // Calculate load
    uint64_t sum = 0;
    for (const auto& rock : rocks) {
        sum += maxY - rock->y;
    }

    std::cout << "Total load is " << sum << std::endl;
}
