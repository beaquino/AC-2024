#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <string>

int main() {

    // Read input
    const std::string& filename = "Data.txt";

    // Ordering does not matter that much and problem does not scale well using linked lists or other sequential container
    std::unordered_map<long long int, long long int> stonesMap;

    std::ifstream file(filename);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read file
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int el;
        while (ss >> el) {
            stonesMap[el]+=1;
        }
    }

    file.close();
    
    for (auto [k, el] : stonesMap){
            std::cout << k << " ";
        }
    std::cout << std::endl;
    
    std::unordered_map<long long int, long long int> stonesMapTemp;

    int nBlinks=75;
    for (int i=0; i<nBlinks; ++i){
        for(auto [k, v] : stonesMap){
            if(k==0){
                stonesMapTemp[1]+=v;
                continue;
            }
            auto str = std::to_string(k);
            if(str.size()%2==0){
                stonesMapTemp[std::stoll(str.substr(0, str.size()/2))]+=v;
                stonesMapTemp[std::stoll(str.substr(str.size()/2))]+=v;
                continue;
            }
            stonesMapTemp[k*2024]+=v;
        }
        stonesMap.clear();
        stonesMap = stonesMapTemp;
        stonesMapTemp.clear();
    }
    
    long long int numberStones=0;

    for(auto [k, v]: stonesMap){
        numberStones+=v;
    }

    std::cout << numberStones << std::endl;

    return 0;
}