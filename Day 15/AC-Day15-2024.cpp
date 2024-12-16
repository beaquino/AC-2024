#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <set>
#include <stdexcept>
#include <cctype>

std::vector<std::pair<int,int>> directions = {
    {-1, 0}, // Up
    {1, 0},  // Down
    {0, -1}, // Left
    {0, 1}   // Right
};

std::pair<int,int> findDir(const char& d){
    if (d=='^'){
        return directions[0];
    }
    if (d=='v'){
        return directions[1];
    }
    if (d=='<'){
        return directions[2];
    }
    if (d=='>'){
        return directions[3];
    }
    // Should not reach here.
    return directions[0];
}

bool moveBox(const std::pair<int,int>& pos, const std::pair<int,int>& d, std::vector<std::vector<char>>& map){
    std::pair<int,int> newPos = {pos.first+d.first, pos.second+d.second};
    if (map[newPos.first][newPos.second]=='.'){
        map[pos.first][pos.second] = '.';
        map[newPos.first][newPos.second] = 'O';
        return true;
    }
    auto cursor = pos;
    while (map[cursor.first][cursor.second]!='.'){
        if (map[cursor.first][cursor.second]=='#'){
            return false;
        }
        cursor.first+=d.first;
        cursor.second+=d.second;
    }
    map[cursor.first][cursor.second]='O';
    map[pos.first][pos.second]='.';
    return true;
}

bool moveBigBox(const std::pair<int,int>& pos, const std::pair<int,int>& d, std::vector<std::vector<char>>& map){
    
    char current = map[pos.first][pos.second];

    // Horizontal push

    if (d.second == -1 || d.second == 1){
        auto cursor = pos;
        while (map[cursor.first][cursor.second]!='.'){
            if(map[cursor.first][cursor.second]=='#'){
                return false;
            }
            // Step twice horizontally to skip the closing bracket
            cursor.second+=2*d.second;
        }

        // If we are here we found space to move and we will write the oposite symbol of what we started.
        map[cursor.first][cursor.second] = current == ']' ? '[' : ']';
        while(cursor!=pos){
            auto prev = cursor;
            cursor.second-=d.second;
            map[cursor.first][cursor.second] = map[prev.first][prev.second]==']' ? '[' : ']';
        }
        map[pos.first][pos.second]='.';
        return true;
    }

    // Vertical push
    
    std::pair<int,int> lBox;
    std::pair<int,int> rBox;
    if (current=='['){
        lBox = {pos.first, pos.second};
        rBox = {lBox.first, lBox.second+1};
    }else{
        rBox = {pos.first, pos.second};
        lBox = {rBox.first, rBox.second-1};
    }

    
    if (d.first == -1 || d.first == 1){
        std::stack<std::pair<int,int>> stk;
        std::stack<std::pair<int,int>> stkTemp;
        std::stack<std::pair<int,int>> stkToMove;
        std::set<std::pair<int,int>> s;
        stkTemp.push(lBox);
        stk = stkTemp;
        s.insert(lBox);
        // We need to process all boxes in one layer before starting the next layer
        while(!stkTemp.empty()){
            while(!stkTemp.empty()){
                stkTemp.pop();
            }
        
            while(!stk.empty()){
                auto boxPos = stk.top();
                stk.pop();
                
                // Box is free to move. Both left and right sides of the box is unobstructed.
                if (map[boxPos.first+d.first][boxPos.second+d.second]=='.' && map[boxPos.first+d.first][boxPos.second+d.second+1]=='.'){
                    stkToMove.push(boxPos);
                    continue;
                }

                // If any side of the box is obstructed return false. We cannot move
                if (map[boxPos.first+d.first][boxPos.second+d.second]=='#' || map[boxPos.first+d.first][boxPos.second+d.second+1]=='#'){
                    return false;
                }

                // If there is one box aligned on top/bottom push it
                if (map[boxPos.first][boxPos.second]==map[boxPos.first+d.first][boxPos.second+d.second]){
                    // We can't push the same box into the stack twice!
                    stkTemp.push({boxPos.first+d.first, boxPos.second+d.second});
                    stkToMove.push(boxPos);
                    continue;
                }

                // If there is a ] on top/bottom of the [, push it to the stack.
                //  @
                //  []   V
                // []x   
                if (map[boxPos.first+d.first][boxPos.second+d.second] == map[boxPos.first][boxPos.second+1]){
                    // We can't push the same box into the stack twice!
                    if (s.find({boxPos.first+d.first, boxPos.second+d.second-1}) == s.end()){
                        s.insert({boxPos.first+d.first, boxPos.second+d.second-1});
                        stkTemp.push({boxPos.first+d.first, boxPos.second+d.second-1});
                    }
                }

                // If there is a [ on top/bottom of the ], push it to the stack.
                //  @
                //  []   V
                //  x[]   
                if (map[boxPos.first+d.first][boxPos.second+d.second+1] == map[boxPos.first][boxPos.second]){
                    if (s.find({boxPos.first+d.first, boxPos.second+d.second+1}) == s.end()){
                        s.insert({boxPos.first+d.first, boxPos.second+d.second+1});
                        stkTemp.push({boxPos.first+d.first, boxPos.second+d.second+1});
                    }
                }

                stkToMove.push(boxPos);
            }
            stk = stkTemp;
        }

        while(!stkToMove.empty()){
            auto boxPos = stkToMove.top();
            stkToMove.pop();
            map[boxPos.first+d.first][boxPos.second+d.second] = map[boxPos.first][boxPos.second];
            map[boxPos.first+d.first][boxPos.second+d.second+1] = map[boxPos.first][boxPos.second+1];
            map[boxPos.first][boxPos.second] = '.';
            map[boxPos.first][boxPos.second+1] = '.';
        }
        map[lBox.first][lBox.second] = '.';
        map[rBox.first][rBox.second] = '.';
        return true;
    }

    return false;
}

bool moveRobot(const std::pair<int,int>& pos, const std::pair<int,int>& d, std::vector<std::vector<char>>& map, bool bigBox){
    std::pair<int,int> newPos = {pos.first+d.first, pos.second+d.second};
    if (map[newPos.first][newPos.second]=='#'){
        return false;
    }
    if (map[newPos.first][newPos.second]=='.'){
        map[pos.first][pos.second] = '.';
        map[newPos.first][newPos.second] = '@';
        return true;
    }
    
    //Then we found either 'O' for small boxes or '[' or ']' for bigger boxes.
    bool move = bigBox ? moveBigBox(newPos, d, map) : moveBox(newPos, d, map);
    if (move){
        map[pos.first][pos.second] = '.';
        map[newPos.first][newPos.second] = '@';
    }
    return move;

}

int main() {

    // Read input
    const std::string& filename1 = "Data1.txt";

    std::vector<std::vector<char>> initialWharehouseMap;

    std::ifstream file1(filename1);
    // Variable to read each line
    std::string line1;
        
    // Check if file opened successfully
    if (!file1.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename1);
    }

    // Save initial position
    std::pair<int, int> initialPos;

    // Read file
    int r=0;
    while (std::getline(file1, line1)) {
        int c=0;
        std::stringstream ss(line1);
        char el;
        std::vector<char> vec;
        while (ss >> el) {
            vec.push_back(el);
            if (el=='@'){
                initialPos = {r, c};
            }
            ++c;
        }
        initialWharehouseMap.push_back(vec);
        ++r;
    }

    file1.close();

    // Read input
    const std::string& filename2 = "Data2.txt";

    std::vector<char> instructions;

    std::ifstream file2(filename2);
        
    // Check if file opened successfully
    if (!file2.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename2);
    }

    // Variable to read each line
    std::string line2;

    // Read file    
    while (std::getline(file2, line2)) {
        std::stringstream ss(line2);
        char el;
        while (ss >> el) {
            instructions.push_back(el);
        }
    }

    file2.close();

    // Part 1

    std::vector<std::vector<char>> wharehouseMap = initialWharehouseMap;

    std::pair<int, int> pos = initialPos;
    for (const auto& m : instructions){
        auto d = findDir(m);
        if (moveRobot(pos, d, wharehouseMap, false)){
            pos.first+=d.first;
            pos.second+=d.second;
        };
    }

    int gpsCoords1 = 0;

    for (int r=0; r<wharehouseMap.size(); ++r){
        for (int c=0; c<wharehouseMap[0].size(); ++c){
            if(wharehouseMap[r][c]=='O'){
                gpsCoords1+=100*r+c;
            }
        }
    }

    std::cout << gpsCoords1 << std::endl;
    // Part 2

    std::vector<std::vector<char>> extendedMap;

    for (int r=0; r<initialWharehouseMap.size(); ++r){
        std::vector<char> row;
        for (int c=0; c<initialWharehouseMap[0].size(); ++c){
            char current = initialWharehouseMap[r][c];
            if (current=='@'){
                row.push_back(current);
                row.push_back('.');
                continue;
            }
            if(current == '.' || current=='#'){
                row.push_back(current);
                row.push_back(current);
                continue;
            }
            if(current=='O'){
                row.push_back('[');
                row.push_back(']');
                continue;
            }
        }
        extendedMap.push_back(row);
    }

    int gpsCoords2 = 0;

    pos = initialPos;
    pos.second = 2*pos.second;

    for (const auto& m : instructions){
        auto d = findDir(m);
        if (moveRobot(pos, d, extendedMap, true)){
            pos.first+=d.first;
            pos.second+=d.second;
        }
    }    

    for (int r=0; r<extendedMap.size(); ++r){
        for (int c=0; c<extendedMap[0].size(); ++c){
            if(extendedMap[r][c]=='['){
                gpsCoords2+=100*r+c;
            }
        }
    }

    std::cout << gpsCoords2 << std::endl;

    return  0;
}