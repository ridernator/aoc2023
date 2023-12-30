#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
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

typedef struct node {
    bool isEnd = false;
    node* left;
    node* right;
} Node;

typedef struct {
    uint64_t start = 0;
    uint64_t length = 0;
} LoopInfo;

bool allAtEnd(const std::vector<Node*>& nodes) {
    return std::all_of(nodes.begin(), nodes.end(), [] (const Node* node) { return node->isEnd; });
}

int main() {
    const auto& rows = readFileToVector();
    std::map<std::string, Node*> nodes;

    const std::string& instructions = rows[0];

    for (std::size_t index = 2; index < rows.size(); ++index) {
        char name[4];
        char left[4];
        char right[4];

        sscanf(rows[index].c_str(), "%3s = (%3s, %3s)", name, left, right);

        if (nodes.find(name) == nodes.end()) {
            nodes[name] = new Node();
        }

        if (nodes.find(left) == nodes.end()) {
            nodes[left] = new Node();
        }

        if (nodes.find(right) == nodes.end()) {
            nodes[right] = new Node();
        }

        nodes[name]->left = nodes[left];
        nodes[name]->right = nodes[right];

        if (name[2] == 'Z') {
            nodes[name]->isEnd = true;
        }
    }

    std::vector<Node*> currentNodes;
    for (const auto& node : nodes) {
        if (node.first[2] == 'A') {
            currentNodes.push_back(node.second);
        }
    }

    // Find loop infos
    std::vector<LoopInfo*> loopInfos;
    for (auto& node : currentNodes) {
        std::size_t instructionCounter = 0;
        uint64_t stepIndex = 0;
        LoopInfo* loopInfo = new LoopInfo();

        while (true) {
            if (node->isEnd) {
                if (loopInfo->start == 0) {
                    loopInfo->start = stepIndex;
                } else {
                    loopInfo->length = stepIndex - loopInfo->start;
                    loopInfos.push_back(loopInfo);

                    break;
                }
            }

            switch (instructions[instructionCounter]) {
                case 'L': {
                    node = node->left;

                    break;
                }

                case 'R': {
                    node = node->right;

                    break;
                }
            }

            if (++instructionCounter == instructions.size()) {
                instructionCounter = 0;
            }

            ++stepIndex;
        }
    }

    uint64_t lcm = 1;
    for (const auto& loopInfo : loopInfos) {
        lcm = std::lcm(lcm, loopInfo->length);
    }


    std::cout << "Number of steps was " << lcm << std::endl;

    for (const auto& node : nodes) {
        delete node.second;
    }

    for (const auto& loopInfo : loopInfos) {
        delete loopInfo;
    }
}


