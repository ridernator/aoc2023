#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
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

enum class Direction {
    N,
    E,
    S,
    W
};

void takeStep(const std::vector<std::string>& data,
              uint32_t& x,
              uint32_t& y,
              Direction& direction) {
    switch (direction) {
        case Direction::N: {
            switch (data[y][x]) {
                case '|': {
                    --y;

                    break;
                }

                case '7': {
                    direction = Direction::W;
                    --x;

                    break;
                }

                case 'F': {
                    direction = Direction::E;
                    ++x;

                    break;
                }

                default: {
                    std::cerr << "Bad direction" << std::endl;
                    std::exit(EXIT_FAILURE);

                    break;
                }
            }

            break;
        }

        case Direction::E: {
            switch (data[y][x]) {
                case '-': {
                    ++x;

                    break;
                }

                case '7': {
                    direction = Direction::S;
                    ++y;

                    break;
                }

                case 'J': {
                    direction = Direction::N;
                    --y;

                    break;
                }

                default: {
                    std::cerr << "Bad direction" << std::endl;
                    std::exit(EXIT_FAILURE);

                    break;
                }
            }

            break;
        }

        case Direction::S: {
            switch (data[y][x]) {
                case '|': {
                    ++y;

                    break;
                }

                case 'J': {
                    direction = Direction::W;
                    --x;

                    break;
                }

                case 'L': {
                    direction = Direction::E;
                    ++x;

                    break;
                }

                default: {
                    std::cerr << "Bad direction" << std::endl;
                    std::exit(EXIT_FAILURE);

                    break;
                }
            }

            break;
        }

        case Direction::W: {
            switch (data[y][x]) {
                case '-': {
                    --x;

                    break;
                }

                case 'L': {
                    direction = Direction::N;
                    --y;

                    break;
                }

                case 'F': {
                    direction = Direction::S;
                    ++y;

                    break;
                }

                default: {
                    std::cerr << "Bad direction" << std::endl;
                    std::exit(EXIT_FAILURE);

                    break;
                }
            }

            break;
        }
    }
}

int main() {
    auto data = readFileToVector();
    auto dataCopy = readFileToVector();

    uint32_t sx = 0;
    uint32_t sy = 0;

    for (std::size_t y = 0; y < data.size(); ++y) {
        for (std::size_t x = 0; x < data[0].size(); ++x) {
            if (data[y][x] == 'S') {
                sx = x;
                sy = y;

                break;
            }
        }
    }

    Direction direction[2] = {Direction::N, Direction::S};
    uint32_t x[2] = {0, 0};
    uint32_t y[2] = {0, 0};

    std::size_t initialIndex = 0;
    if (sy != 0) {
        if ((data[sy - 1][sx] == 'F') ||
            (data[sy - 1][sx] == '7') ||
            (data[sy - 1][sx] == '|')) {
            direction[initialIndex] = Direction::N;
            x[initialIndex] = sx;
            y[initialIndex] = sy - 1;
            ++initialIndex;
        }
    }

    if (sy != data.size() - 1) {
        if ((data[sy + 1][sx] == 'J') ||
            (data[sy + 1][sx] == 'L') ||
            (data[sy + 1][sx] == '|')) {
            direction[initialIndex] = Direction::S;
            x[initialIndex] = sx;
            y[initialIndex] = sy + 1;
            ++initialIndex;
        }
    }

    if (sx != 0) {
        if ((data[sy][sx - 1] == 'F') ||
            (data[sy][sx - 1] == 'L') ||
            (data[sy][sx - 1] == '-')) {
            direction[initialIndex] = Direction::W;
            x[initialIndex] = sx - 1;
            y[initialIndex] = sy;
            ++initialIndex;
        }
    }

    if (sx != data[0].size() - 1) {
        if ((data[sy][sx + 1] == 'J') ||
            (data[sy][sx + 1] == '7') ||
            (data[sy][sx + 1] == '-')) {
            direction[initialIndex] = Direction::E;
            x[initialIndex] = sx + 1;
            y[initialIndex] = sy;
        }
    }

    switch (direction[0]) {
        case Direction::N: {
            switch (direction[1]) {
                case Direction::N: {
                    std::cout << "Bad pairing" << std::endl;
                    std::exit(EXIT_FAILURE);
                    break;
                }

                case Direction::E: {
                    data[sy][sx] = 'L';
                    break;
                }

                case Direction::S: {
                    data[sy][sx] = '|';
                    break;
                }

                case Direction::W: {
                    data[sy][sx] = 'J';
                    break;
                }
            }

            break;
        }

        case Direction::E: {
            switch (direction[1]) {
                case Direction::N: {
                    data[sy][sx] = 'L';
                    break;
                }

                case Direction::E: {
                    std::cout << "Bad pairing" << std::endl;
                    std::exit(EXIT_FAILURE);
                    break;
                }

                case Direction::S: {
                    data[sy][sx] = 'F';
                    break;
                }

                case Direction::W: {
                    data[sy][sx] = '-';
                    break;
                }
            }

            break;
        }

        case Direction::S: {
            switch (direction[1]) {
                case Direction::N: {
                    data[sy][sx] = '|';
                    break;
                }

                case Direction::E: {
                    data[sy][sx] = 'F';
                    break;
                }

                case Direction::S: {
                    std::cout << "Bad pairing" << std::endl;
                    std::exit(EXIT_FAILURE);
                    break;
                }

                case Direction::W: {
                    data[sy][sx] = '7';
                    break;
                }
            }

            break;
        }

        case Direction::W: {
            switch (direction[1]) {
                case Direction::N: {
                    data[sy][sx] = 'J';
                    break;
                }

                case Direction::E: {
                    data[sy][sx] = '-';
                    break;
                }

                case Direction::S: {
                    data[sy][sx] = '7';
                    break;
                }

                case Direction::W: {
                    std::cout << "Bad pairing" << std::endl;
                    std::exit(EXIT_FAILURE);
                    break;
                }
            }

            break;
        }
    }

    dataCopy[sy][sx] = 'X';

    while (!((x[0] == x[1]) && (y[0] == y[1]))) {
        dataCopy[y[0]][x[0]] = 'X';
        dataCopy[y[1]][x[1]] = 'X';

        takeStep(data, x[0], y[0], direction[0]);
        takeStep(data, x[1], y[1], direction[1]);
    }

    dataCopy[y[0]][x[0]] = 'X';

    for (std::size_t y = 0; y < data.size(); ++y) {
        for (std::size_t x = 0; x < data[0].size(); ++x) {
            if (dataCopy[y][x] != 'X') {
                data[y][x] = '.';
            }
        }
    }

    uint32_t enclosed = 0;
    for (const auto& row : data) {
        uint32_t inOuter = 0;
        char lastVert = '|';

        for (const auto& datum : row) {
            switch (datum) {
                case '|':
                case 'F':
                case 'L': {
                    inOuter = (inOuter + 1) % 2;
                    lastVert = datum;

                    break;
                }

                case 'J': {
                    if ((lastVert != 'F')) {
                        inOuter = (inOuter + 1) % 2;
                    }

                    lastVert = datum;

                    break;
                }

                case '7': {
                    if ((lastVert != 'L')) {
                        inOuter = (inOuter + 1) % 2;
                    }

                    lastVert = datum;

                    break;
                }

                case '-': {
                    break;
                }

                default: {
                    enclosed += inOuter;

                    break;
                }
            }
        }
    }

    std::cout << "Number of enclosed tiles is " << enclosed << std::endl;
}
