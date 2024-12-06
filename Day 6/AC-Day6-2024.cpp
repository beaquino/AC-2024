#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include <string>

std::unordered_map<char, std::pair<int,int>> directions = {
    {'^', {-1,0}}, // Go up
    {'v', {1,0}}, // Go down
    {'<', {0,-1}}, // Go left
    {'>', {0,1}} // Go right
};

void turnRight(char& c){
    if (c=='^'){
        c='>';
        return;
    }

    if (c=='>'){
        c='v';
        return;
    }

    if (c=='v'){
        c='<';
        return;
    }

    if (c=='<'){
        c='^';
        return;
    }

    return;
}

bool reachEnd(const int r, const int c, const char d, const std::vector<std::vector<char>>& map){
    // Going up and on the first row
    if (d=='^' && r==0){
        return true;
    }

    // Going down and on the last row
    if (d=='v' && r==map.size()-1){
        return true;
    }

    // Going left and on the first column
    if (d=='<' && c==0){
        return true;
    }
    
    // Going right and on the last column
    if (d=='>' && c==map[0].size()-1){
        return true;
    }

    return false;
}

int doWalk(int r, int c, char d, std::vector<std::vector<char>>& map){
    int totalSpots=0;
    int dx = 0;
    int dy = -1;
    bool firstCounted=false;
    while(!reachEnd(r,c,d, map)){
        //Next move will hit an obstacle?
        if (map[r+dy][c+dx]=='#'){
            //Then turn right
            turnRight(d);
            dy = directions[d].first;
            dx = directions[d].second;
            continue;
        }
        // If we are here we are free to proceed
        
        // First increment totalSpots if we have not visited it and mark it as visited
        if (map[r][c]!='x'){
            if(map[r][c]=='^' && !firstCounted){
                ++totalSpots;
                firstCounted=true;
            }
            else{
                ++totalSpots;
                map[r][c]='x';
            }
        }

        // Then update the new position
        r+=dy;
        c+=dx;
    }
    ++totalSpots;
    map[r][c]='x';
    return totalSpots;
}

int numberOfLoops(const int r0, const int c0, const char d0, std::vector<std::vector<char>>& map, const std::vector<std::vector<char>>& originalMap){
    int numLoops=0;
    int numXs = map.size()*map[0].size();

    // This is incredibly ugly, but works.
    for (int row=0; row<map.size(); ++row){
        for (int col=0; col<map[0].size(); ++col){
            // We ignore locations where the guard never steps in
            if (map[row][col]!='x'){
                continue;
            }
            // If we are here this is part of the path. Let's put a tentative obstacle here
            map[row][col]='o';
            bool obstacleFound = false;
            // Similar to doWalk
            int r = r0;
            int c = c0;
            char d = d0;
            int dx = 0;
            int dy = -1;
            int it = numXs;
            while(!reachEnd(r,c,d, originalMap)){
                // If we have run more squares than the entire grid, then this is clearly a loop.
                // Maybe this is too conservative and is slowing down performance. Worst case this is O(n^4).
                if (it==0){
                    ++numLoops;
                    break;
                }
                //Next move will hit an obstacle?
                if (originalMap[r+dy][c+dx]=='#' || map[r+dy][c+dx]=='o'){
                    //Then turn right
                    turnRight(d);
                    dy = directions[d].first;
                    dx = directions[d].second;
                    continue;
                }
                // If we are here we are free to proceed
                // Then update the new position
                r+=dy;
                c+=dx;
                --it;
            }
            //Return to the previous value
            map[row][col]='x';
        }
    }
    return numLoops;
}

int main() {
    
    // Read input
    const std::string& filename = "Data.txt";

    std::vector<std::vector<char>> guardMap;

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
            guardMap.push_back(row);
        }
    }
    
    file.close();

    // First Part
    int r0 = 0;
    int c0 = 0;
    bool found = false;

    // We need to find the starting position first
    for (int row=0; row<guardMap.size(); ++row){
        for (int col=0; col<guardMap[0].size(); ++col){
            if (guardMap[row][col]=='^'){
                r0 = row;
                c0 = col;
                found = true;
                break;
            }
        }
        if (found){
            break;
        }
    }

    int r = r0;
    int c = c0;
    char d = '^';
    int totalSpots = doWalk(r, c, d, guardMap);

    std::cout << totalSpots << std::endl;

    // Second Part

    std::vector<std::vector<char>> originalGuardMap = guardMap;

    int numLoops = numberOfLoops(r0, c0, d, guardMap, originalGuardMap);

    std::cout << numLoops << std::endl;

    return 0;
}