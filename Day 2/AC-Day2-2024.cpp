#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <string>

bool isRowSafe (const std::vector<int>& vec){
    int diff = 0;
    bool inc = false;
    bool dec = false;
    for (int i=1; i<vec.size(); ++i){
            diff = vec[i] - vec[i-1];
            if (diff>3 || diff < -3 || diff==0){
                return false;
            }
            if (i==1){
                inc = diff>0? true : false;
                dec = !inc;
                continue;
            }
            if ((inc && diff < 0) || (dec && diff > 0)){
                return false;
            }
    } 
    return true;
}

bool isRowSafeRemovingLevel (const std::vector<int>& vec){
    if (isRowSafe(vec)){
        return true;
    }

    std::vector<int> aux;
    // Ugly O(n^2) :(
    for(int i=0; i<vec.size(); ++i){    
        for (int j=0; j<vec.size(); ++j){
            if (j!=i){
                aux.push_back(vec[j]);
            }
        }
        if (isRowSafe(aux)){
            return true;
        }
        aux.clear();
    }
    return false;
}

int main() {
    
    const std::string& filename = "Data.txt";

    std::vector<std::vector<int>> data;

    std::ifstream file(filename);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read file
     while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int number;

        // Read line and push it to an std::vector<int>
        while (iss >> number) {
            row.push_back(number);
        }

        data.push_back(row);
    }
    file.close();

    int totalSafe=0;
    
    for (int r=0; r<data.size(); ++r){
        if (isRowSafe(data[r])){
            ++totalSafe;
        }
    }
    
    std::cout << totalSafe << std::endl;

    int totalSafeWithOneRemoval = 0;

    for (int r=0; r<data.size(); ++r){
        if (isRowSafeRemovingLevel(data[r])){
            ++totalSafeWithOneRemoval;
        }
    }

    std::cout << totalSafeWithOneRemoval << std::endl;
    
    return 0;
}
