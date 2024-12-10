#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <stack>
#include <map>
#include <stdexcept>
#include <string>

bool isValidPos(int r, int c, int nrow, int ncol){
    return r>=0 && c>=0 && r<nrow && c<ncol;
}



int main() {

    // Read input
    const std::string& filename = "Data.txt";

    std::vector<std::vector<int>> trailMap;

    std::ifstream file(filename);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read file
    
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (const char& c : line) {
            row.push_back(int(c-'0'));
        }
        trailMap.push_back(row);
    }
    
    for (auto row : trailMap){
        for(auto el: row){
            std::cout<< el << " ";
        }
        std::cout << std::endl;
    }

    file.close();

    // Part 1

    std::vector<std::pair<int,int>> dir={
        {-1,0}, // Up
        {1,0},  // Down
        {0,-1}, // Left
        {0, 1}  // Right
    };

    int nrow = trailMap.size();
    int ncol = trailMap[0].size();
    std::map<std::vector<int>, int> trailRatings;
    int totalCount = 0;
    
    for (int r=0; r<nrow; ++r){
        for (int c=0; c<ncol; ++c){
            // Check only positions that have 0
            if (trailMap[r][c]==0){
                std::stack<std::pair<int,int>> stk;
                stk.push({r,c});
                std::set<std::pair<int,int>> set9;
                // Set up a dfs algorithm
                while(!stk.empty()){
                    auto [sr, sc] = stk.top();
                    stk.pop();
                    // If this is a 9 we are done with this trail
                    if (trailMap[sr][sc]==9){
                        // Insert the indexes on a set for part 1
                        set9.insert({sr,sc});
                        // Insert start and end positions on a map for part 2
                        std::vector<int> beginEndPos = {r,c,sr,sc};
                        if(trailRatings.find(beginEndPos)==trailRatings.end()){
                            trailRatings[beginEndPos] = 1;
                        }
                        else{
                            ++trailRatings[beginEndPos];
                        }
                        continue;
                    }
                    for(auto [dr, dc] : dir){
                        // Push all valid neighbours into the stack
                        if(isValidPos(sr+dr, sc+dc, nrow, ncol) && trailMap[sr+dr][sc+dc]==trailMap[sr][sc]+1){
                            stk.push({sr+dr, sc+dc});
                        }
                    }
                }
                totalCount+=set9.size();
            }
        }
    }

    std::cout << totalCount << std::endl;
    
    // Part 2

    int totalCount2 = 0;

    for (auto [x, val] : trailRatings){
        totalCount2+=val;
    }

    std::cout << totalCount2 << std::endl;

    return 0;
}