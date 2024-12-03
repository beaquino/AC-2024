#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <stdexcept>
#include <string>

int main() {
    
    // Read input
    const std::string& filename = "Data.txt";

    std::ifstream file(filename);
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;
    std::string fullCommands="";
    // Read file into a single string
     while (std::getline(file, line)) {
        fullCommands+=line;
    }
    file.close();

    // First Part
    
    // Regex pattern for mul(DDD,DDD)
    const std::regex pattern("mul\\((\\d{1,3}),(\\d{1,3})\\)");

    auto matches_begin = std::sregex_iterator(fullCommands.begin(), fullCommands.end(), pattern);
    auto matches_end = std::sregex_iterator();

    int finalResult1 = 0;

    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
        std::smatch match = *i;
        
        finalResult1 += std::stoi(match.str(1))*std::stoi(match.str(2));  // Multiply the two numbers
    }

    std::cout << "Part 1: " << finalResult1 << std::endl;


    // Second Part
    const std::regex pattern2("mul\\((\\d{1,3}),(\\d{1,3})\\)"  // mul(DDD,DDD) pattern
                              "|do\\(\\)"                       // or "do()"
                              "|don't\\(\\)"                    // or "don't()"
    );

    int finalResult2 = 0;
    bool enabled = true;

    auto matches_begin2 = std::sregex_iterator(fullCommands.begin(), fullCommands.end(), pattern2);
    auto natches_end2 = std::sregex_iterator();

    for (std::sregex_iterator i = matches_begin2; i != natches_end2; ++i) {
        std::smatch match = *i;

        if(match.str(0)=="do()"){
            enabled=true;
            continue;
        }
        if(match.str(0)=="don't()"){
            enabled=false;
            continue;
        }
        
        // We are in the mul(DDD,DDD) pattern
        if (enabled){
            finalResult2 += std::stoi(match.str(1))*std::stoi(match.str(2));  // Multiply the two numbers if enabled
        }
    }

    std::cout << "Part 2: " << finalResult2 << std::endl;

    return 0;
}