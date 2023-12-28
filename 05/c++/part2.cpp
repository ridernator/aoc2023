#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
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
    uint64_t destination;
    uint64_t source;
    uint64_t range;
} Mapping;

void populateMapping(const std::vector<std::string>& rules,
                     const std::size_t startLine,
                     std::vector<Mapping*>& mappings) {
    for (std::size_t index = startLine; index < rules.size(); ++index) {
        if (rules[index].empty()) {
            break;
        }

        Mapping* mapping = new Mapping();

        sscanf(rules[index].c_str(), "%lu %lu %lu", &mapping->destination, &mapping->source, &mapping->range);

        mappings.push_back(mapping);
    }
}

uint64_t mapValue(const uint64_t input,
                  const std::vector<Mapping*>& mappings) {
    for (const auto& mapping : mappings) {
        if ((mapping->source <= input) &&
            (mapping->source + mapping->range > input)) {
            return mapping->destination + (input - mapping->source);
        }
    }

    return input;
}

void deleteMapping(const std::vector<Mapping*> mappings) {
    for (const auto& mapping : mappings) {
        delete mapping;
    }
}

int main() {
    std::vector<uint64_t> seeds;
    std::string tempString;
    
    std::vector<Mapping*> seedToSoilMappings;
    std::vector<Mapping*> soilToFertilizerMappings;
    std::vector<Mapping*> fertilizerToWaterMappings;
    std::vector<Mapping*> waterToLightMappings;
    std::vector<Mapping*> lightToTemperatureMappings;
    std::vector<Mapping*> temperatureToHumidityMappings;
    std::vector<Mapping*> humidityToLocationMappings;

    uint64_t lowestLocation = std::numeric_limits<uint64_t>::max();

    auto rules = readFileToVector();

    // Read seeds
    std::istringstream seedsRow(rules[0].substr(rules[0].find_first_of(":") + 2));
    while (std::getline(seedsRow, tempString, ' ')) {
        seeds.push_back(std::stoull(tempString));
    }

    for (std::size_t index = 0; index < rules.size(); ++index) {
        if (rules[index].find("seed-to-soil") != std::string::npos) {
            populateMapping(rules, index + 1, seedToSoilMappings);
        } else if (rules[index].find("soil-to-fertilizer") != std::string::npos) {
            populateMapping(rules, index + 1, soilToFertilizerMappings);
        } else if (rules[index].find("fertilizer-to-water") != std::string::npos) {
            populateMapping(rules, index + 1, fertilizerToWaterMappings);
        } else if (rules[index].find("water-to-light") != std::string::npos) {
            populateMapping(rules, index + 1, waterToLightMappings);
        } else if (rules[index].find("light-to-temperature") != std::string::npos) {
            populateMapping(rules, index + 1, lightToTemperatureMappings);
        } else if (rules[index].find("temperature-to-humidity") != std::string::npos) {
            populateMapping(rules, index + 1, temperatureToHumidityMappings);
        } else if (rules[index].find("humidity-to-location") != std::string::npos) {
            populateMapping(rules, index + 1, humidityToLocationMappings);
        } 
    }
            
    uint64_t soil;
    uint64_t fertilizer;
    uint64_t water;
    uint64_t light;
    uint64_t temperature;
    uint64_t humidity;
    uint64_t location;

    for (std::size_t index = 0; index < seeds.size(); index += 2) {
        for (uint64_t subIndex = 0; subIndex < seeds[index + 1]; ++subIndex) {
            soil = mapValue(seeds[index] + subIndex, seedToSoilMappings);
            fertilizer = mapValue(soil, soilToFertilizerMappings);
            water = mapValue(fertilizer, fertilizerToWaterMappings);
            light = mapValue(water, waterToLightMappings);
            temperature = mapValue(light, lightToTemperatureMappings);
            humidity = mapValue(temperature, temperatureToHumidityMappings);
            location = mapValue(humidity, humidityToLocationMappings);

            lowestLocation = std::min(lowestLocation, location);
        }
    }

    std::cout << "Lowest location is " << lowestLocation << std::endl;

    deleteMapping(seedToSoilMappings);
    deleteMapping(soilToFertilizerMappings);
    deleteMapping(fertilizerToWaterMappings);
    deleteMapping(waterToLightMappings);
    deleteMapping(lightToTemperatureMappings);
    deleteMapping(temperatureToHumidityMappings);
    deleteMapping(humidityToLocationMappings);
}
