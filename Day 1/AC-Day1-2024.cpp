#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <string>

int main() {
    
    const std::string& filename = "Data.txt";

    std::vector<int> col1;
    std::vector<int> col2;

    std::ifstream file(filename);
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Variables to read integers
    int num1, num2;

    // Read file line by line
    while (file >> num1 >> num2) {
        col1.push_back(num1);
        col2.push_back(num2);
    }

    // Check for reading errors
    if (file.bad()) {
        throw std::runtime_error("Error occurred while reading the file");
    }

    file.close();

    // Sort files so we can compare elements
    std::sort(col1.begin(),col1.end());
    std::sort(col2.begin(),col2.end());

    int totalDistance = 0;
    for (int i=0; i<col1.size(); i++){
        totalDistance += abs(col1[i]-col2[i]);
    }
    
    // Print total distances
    std::cout << totalDistance << std::endl;

    //Second part
    std::unordered_map<int,int> dic;

    //Add elements from col2 to a dictionary and update their frequencies.
    for (auto el : col2){
        dic[el]++;
    }

    int similarity=0;

    // Using col2 here and col1 before would not change the result
    for (auto el : col1){
        if (dic.find(el)!=dic.end()){
            similarity+= dic[el]*el;
        }
    }
    
    // Print similarity
    std::cout << similarity << std::endl;
    return 0;
}