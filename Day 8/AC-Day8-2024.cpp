#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <stdexcept>
#include <string>

bool isValidPos(int r, int c, int mr, int mc){
    return r>=0 && c>=0 && r<mr && c<mc;
}

int main() {

    // Read input
    const std::string& filename = "Data.txt";

    std::vector<std::vector<char>> antenaMap;

    std::ifstream file(filename);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read file
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        
        // Only add non-empty rows
        if (!row.empty()) {
            antenaMap.push_back(row);
        }
    }
    
    file.close();

    int nrows = antenaMap.size();
    int ncols = antenaMap[0].size();

    std::unordered_map<char, std::vector<std::pair<int,int>>> antenaPos;

    for (int r=0; r<nrows; ++r){
        for (int c=0; c<ncols; ++c){
            if (antenaMap[r][c]!='.'){
                antenaPos[antenaMap[r][c]].push_back({r,c});
            }
        }
    }

    // Part 1
    std::set<std::pair<int,int>> answer1;
    for (auto [key,vec]: antenaPos){
        for (int i=0; i<vec.size()-1; ++i){
            for (int j=i+1; j<vec.size(); j++){
                std::pair<int,int> newPos1 = {2*vec[i].first-vec[j].first, 2*vec[i].second-vec[j].second};
                std::pair<int,int> newPos2 = {2*vec[j].first-vec[i].first, 2*vec[j].second-vec[i].second};
                
                if (isValidPos(newPos1.first, newPos1.second, nrows, ncols)){
                    answer1.insert(newPos1);
                }
                if (isValidPos(newPos2.first, newPos2.second, nrows, ncols)){
                    answer1.insert(newPos2);
                }
                
            }
        }
    }

    std::cout << answer1.size() << std::endl;

    // Part 2
    std::set<std::pair<int,int>> answer2;
    for (auto [key,vec]: antenaPos){
        // Insert antenae's position if there is more than one
        if (vec.size()>1){
            for (const auto& pair : vec){
                answer2.insert(pair);
            }
        }
        for (int i=0; i<vec.size()-1; ++i){
            for (int j=i+1; j<vec.size(); j++){
                int f1 = 2;
                int f2 = 1;
                std::pair<int,int> newPos1 = {f1*vec[i].first-f2*vec[j].first, f1*vec[i].second-f2*vec[j].second};
                
                while(isValidPos(newPos1.first, newPos1.second, nrows, ncols)){
                    answer2.insert(newPos1);
                    ++f1;
                    ++f2;
                    newPos1 = {f1*vec[i].first-f2*vec[j].first, f1*vec[i].second-f2*vec[j].second};
                }

                f1=2;
                f2=1;
                std::pair<int,int> newPos2 = {f1*vec[j].first-f2*vec[i].first, f1*vec[j].second-f2*vec[i].second};

                while(isValidPos(newPos2.first, newPos2.second, nrows, ncols)){
                    answer2.insert(newPos2);
                    ++f1;
                    ++f2;
                    newPos2 = {f1*vec[j].first-f2*vec[i].first, f1*vec[j].second-f2*vec[i].second};
                }
            }
        }
    }

    std::cout << answer2.size() << std::endl;

    return 0;
}