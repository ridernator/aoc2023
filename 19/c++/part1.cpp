#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <map>
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

enum class Category {
    X = 'x',
    M = 'm',
    A = 'a',
    S = 's'
};

enum class Condition {
    GT = '>',
    LT = '<'
};

typedef struct {
    Category category;
    Condition condition;
    uint64_t value;
    std::string ruleSet;
} Rule;

typedef struct {
    std::vector<Rule*> rules;
    std::string finally;
} RuleSet;

typedef struct {
    uint64_t x;
    uint64_t m;
    uint64_t a;
    uint64_t s;
} Part;

int main() {
    auto rows = readFileToVector();

    auto emptyRow = std::find(rows.begin(), rows.end(), "");

    // Parse rules
    std::map<std::string, RuleSet*> ruleSets;
    for (auto iterator = rows.begin(); iterator != emptyRow; ++iterator) {
        std::string tempRow = *iterator;
        RuleSet* ruleSet = new RuleSet();
        std::string name = tempRow.substr(0, tempRow.find('{'));
        tempRow = tempRow.substr(tempRow.find('{') + 1);

        while (tempRow.find(':') != std::string::npos) {
            Rule* rule = new Rule();
            char category;
            char condition;
            uint64_t value;
            sscanf(tempRow.c_str(), "%c%c%lu", &category, &condition, &value);

            rule->category = (Category) category;
            rule->condition = (Condition) condition;
            rule->value = value;
            rule->ruleSet = tempRow.substr(tempRow.find(':') + 1, tempRow.find(',') - (tempRow.find(':') + 1));

            ruleSet->rules.push_back(rule);

            tempRow = tempRow.substr(tempRow.find(',') + 1);
        }

        ruleSet->finally = tempRow.substr(0, tempRow.size() - 1);

        ruleSets[name] = ruleSet;
    }

    // Parse parts
    std::vector<Part*> parts;
    for (auto iterator = emptyRow + 1; iterator != rows.end(); ++iterator) {
        Part* part = new Part();
        sscanf(iterator->c_str(), "{x=%lu,m=%lu,a=%lu,s=%lu}", &part->x, &part->m, &part->a, &part->s);

        parts.push_back(part);
    }

    // Run rules on parts
    uint64_t partRatings = 0;
    for (const auto& part : parts) {
        RuleSet* ruleSet = ruleSets["in"];
        bool accepted = false;
        bool rejected = false;
        bool newRuleSet;

        while (true) {
            newRuleSet = false;

            for (const auto& rule : ruleSet->rules) {
                uint64_t value = 0;

                switch (rule->category) {
                    case Category::X: {
                        value = part->x;

                        break;
                    }

                    case Category::M: {
                        value = part->m;

                        break;
                    }

                    case Category::A: {
                        value = part->a;

                        break;
                    }

                    case Category::S: {
                        value = part->s;

                        break;
                    }
                }

                switch (rule->condition) {
                    case Condition::GT: {
                        if (value > rule->value) {
                            if (rule->ruleSet.compare("R") == 0) {
                                rejected = true;
                            } else if (rule->ruleSet.compare("A") == 0) {
                                accepted = true;
                            } else {
                                ruleSet = ruleSets[rule->ruleSet];
                                newRuleSet = true;
                            }
                        }

                        break;
                    }

                    case Condition::LT: {
                        if (value < rule->value) {
                            if (rule->ruleSet.compare("R") == 0) {
                                rejected = true;
                            } else if (rule->ruleSet.compare("A") == 0) {
                                accepted = true;
                            } else {
                                ruleSet = ruleSets[rule->ruleSet];
                                newRuleSet = true;
                            }
                        }

                        break;
                    }
                }

                if ((accepted) || (rejected)) {
                    break;
                }
            }

            if (accepted) {
                partRatings += (part->x + part->m + part->a + part->s);

                break;
            } else if (rejected) {
                break;
            } else if (!newRuleSet) {
                if (ruleSet->finally.compare("A") == 0) {
                    partRatings += (part->x + part->m + part->a + part->s);

                    break;
                } else if (ruleSet->finally.compare("R") == 0) {
                    break;
                } else {
                    ruleSet = ruleSets[ruleSet->finally];
                }
            }
        }
    }

    std::cout << "Total part ratings = " << partRatings << std::endl;

    for (const auto& part : parts) {
        delete part;
    }

    for (const auto& ruleSet : ruleSets) {
        for (const auto& rule : ruleSet.second->rules) {
            delete rule;
        }

        delete ruleSet.second;
    }
}

