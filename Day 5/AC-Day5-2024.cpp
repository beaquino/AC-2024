#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include <string>

bool isValidLine (const std::vector<int>& line, const std::unordered_map<int, std::vector<int>>& requirements){
    std::set<int> inserted;
    for (auto el : line){
        //Element has requirements. Check if none of the required numbers were inserted before.
        if(requirements.find(el)!=requirements.end()){
            for (auto r : requirements.at(el)){
                //Trying to insert an element that should've been inserted before.
                if (inserted.find(r)!=inserted.end()){
                    return false;
                }
            }    
        }
        //Either element has no requirements or all requirements were fulfilled. Insert the element.
        inserted.insert(el);
    }
    return true;
}

void fixOrder (std::vector<int>& seq, const std::unordered_map<int, std::vector<int>>& inverseRequirements){
    int p=0;

    while (p!=seq.size()){
        //Is this element in the correct position? Let's check its requirements
        if(inverseRequirements.find(seq[p])!=inverseRequirements.end()){
            //Investigate all requirements
            for (auto r : inverseRequirements.at(seq[p])){
                auto it = std::find(seq.begin(),seq.end(),r);
                //If r is not present in the sequence or it comes first we can proceed
                if (it == seq.end() || it-seq.begin()<p){
                    continue;
                }
                //If we're here, r exists in the sequence and is later in the vector! Make a swap to fix!
                int aux = seq[p];
                seq[p] = *it;
                *it = aux;
                // Go back to the beggining, since there may be more requirements.
                p=-1;
                break;
            }
        }
        ++p;
    }
}

int main() {
    
    // Read input
    const std::string& filename1 = "Data1.txt";

    std::ifstream file1(filename1);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file1.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename1);
    }

    std::vector<int> column1;
    std::vector<int> column2;
    // Read file
    while (std::getline(file1, line)) {
        std::stringstream ss(line);
        int val1, val2;
        char p;

        if (ss >> val1 >> p >> val2) {
            column1.push_back(val1);
            column2.push_back(val2);
        }
    }
    
    file1.close();

    // Read input
    const std::string& filename2 = "Data2.txt";

    std::ifstream file2(filename2);
        
    // Check if file opened successfully
    if (!file2.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename1);
    }

    std::vector<std::vector<int>> sequences;
    // Read file
    while (std::getline(file2, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<int> tempLine;

        while (std::getline(ss, token, ',')) {
            tempLine.push_back(std::stoi(token));
        }
        sequences.push_back(tempLine);
    }
    
    file2.close();

    // Part 1

    std::unordered_map<int, std::vector<int>> requirements;

    for (int i=0; i<column1.size(); ++i){
        std::vector<int>& req = requirements[column1[i]];
        req.push_back(column2[i]);
    }

    int totalSum1= 0;
    for (auto seq : sequences){
        if (isValidLine(seq, requirements)){
            totalSum1 += seq[(seq.size()-1)/2];
        }
    }

    std::cout << "Total Sum of middle elements of correct sequences is: " << totalSum1 << std::endl;

    // Part 2

    // Need the first map reversed for the reordering
    std::unordered_map<int, std::vector<int>> inverseRequirements;

    for (int i=0; i<column2.size(); ++i){
        std::vector<int>& req = inverseRequirements[column2[i]];
        req.push_back(column1[i]);
    }

    int totalSum2 = 0;
    for (auto seq : sequences){
        if (!isValidLine(seq, requirements)){
            fixOrder(seq, inverseRequirements);
            totalSum2 += seq[(seq.size()-1)/2];
        }
    }

    std::cout << "Total sum of middle pages after reordering wrong sequences is: " << totalSum2 << std::endl;

    return 0;
}