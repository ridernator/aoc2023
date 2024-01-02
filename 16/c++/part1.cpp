#include <cinttypes>
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

enum class Direction {
    N,
    E,
    S,
    W
};

void loop(const int64_t startX,
          const int64_t startY,
          const Direction startDirection,
          const std::vector<std::string>& grid,
          std::map<Direction, bool**>& already) {
    int64_t x = startX;
    int64_t y = startY;
    Direction direction = startDirection;

    while (true) {
        if ((x < 0) ||
            (y < 0) ||
            ((std::size_t) x == grid[0].size()) ||
            ((std::size_t) y == grid.size()) ||
            (already[direction][y][x])) {
            break;
        }

        already[direction][y][x] = true;

        switch (grid[y][x]) {
            case '/': {
                switch (direction) {
                    case Direction::N: {
                        direction = Direction::E;

                        break;
                    }

                    case Direction::E: {
                        direction = Direction::N;

                        break;
                    }

                    case Direction::S: {
                        direction = Direction::W;

                        break;
                    }

                    case Direction::W: {
                        direction = Direction::S;

                        break;
                    }
                }

                break;
            }

            case '\\': {
                switch (direction) {
                    case Direction::N: {
                        direction = Direction::W;

                        break;
                    }

                    case Direction::E: {
                        direction = Direction::S;

                        break;
                    }

                    case Direction::S: {
                        direction = Direction::E;

                        break;
                    }

                    case Direction::W: {
                        direction = Direction::N;

                        break;
                    }
                }

                break;
            }

            case '-': {
                switch (direction) {
                    case Direction::N:
                    case Direction::S: {
                        loop(x + 1, y, Direction::E, grid, already);

                        direction = Direction::W;

                        break;
                    }

                    default: {

                        break;
                    }
                }

                break;
            }

            case '|': {
                switch (direction) {
                    case Direction::E:
                    case Direction::W: {
                        loop(x, y + 1, Direction::S, grid, already);

                        direction = Direction::N;

                        break;
                    }

                    default: {

                        break;
                    }
                }

                break;
            }

            default: {
                break;
            }
        }

        switch (direction) {
            case Direction::N: {
                --y;

                break;
            }

            case Direction::E: {
                ++x;

                break;
            }

            case Direction::S: {
                ++y;

                break;
            }

            case Direction::W: {
                --x;

                break;
            }
        }
    }
}

int main() {
    auto grid = readFileToVector();
    const std::size_t maxX = grid[0].size();
    const std::size_t maxY = grid.size();

    std::map<Direction, bool**> already;

    bool** northAlready = new bool*[maxY];
    bool** eastAlready = new bool*[maxY];
    bool** southAlready = new bool*[maxY];
    bool** westAlready = new bool*[maxY];
    for (std::size_t index = 0; index < maxY; ++index) {
        *(northAlready + index) = new bool[maxX];
        *(eastAlready + index) = new bool[maxX];
        *(southAlready + index) = new bool[maxX];
        *(westAlready + index) = new bool[maxX];
        for (std::size_t index2 = 0; index2 < maxX; ++index2) {
            *(*(northAlready + index) + index2) = false;
            *(*(eastAlready + index) + index2) = false;
            *(*(southAlready + index) + index2) = false;
            *(*(westAlready + index) + index2) = false;
        }
    }
    already[Direction::N] = northAlready;
    already[Direction::E] = eastAlready;
    already[Direction::S] = southAlready;
    already[Direction::W] = westAlready;

    loop(0, 0, Direction::E, grid, already);

    uint64_t energizedTiles = 0;
    for (std::size_t index = 0; index < maxY; ++index) {
        for (std::size_t index2 = 0; index2 < maxX; ++index2) {
            if ((*(*(northAlready + index) + index2)) ||
                (*(*(eastAlready + index) + index2)) ||
                (*(*(southAlready + index) + index2)) ||
                (*(*(westAlready + index) + index2))) {
                ++energizedTiles;
            }
        }
    }

    std::cout << "Number of energized tiles = " << energizedTiles << std::endl;
}
