#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <cmath>
#include <set>
#include <stdexcept>

// E, S, W, N
const std::vector<int> dr = {0, 1, 0, -1};
const std::vector<int> dc = {1, 0, -1, 0};

bool isValidPos(const std::vector<std::vector<char>>& m, int r, int c, int n){
    return r>=0 && r<n && c>=0 && c<n;
}

int main() {

    // Read input
    const std::string& filename = "Data.txt";

    std::vector<int> row;
    std::vector<int> col;

    std::ifstream file(filename);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read file
    while (std::getline(file, line)) {
        
        size_t pos = line.find(',');
        row.push_back(stoi(line.substr(0, pos)));
        col.push_back(stoi(line.substr(pos+1)));
    }

    file.close();

    const int n = 71;

    std::vector<std::vector<char>> memory(n, std::vector<char>(n, '.'));
    std::vector<std::vector<int>> cost(n, std::vector<int>(n, 0));

    for (int i=0; i<row.size(); ++i){
        //std::cout << row[i] << "," << col[i] << std::endl;
    }
    
    // Part 1

    // Make 1 byte fall
    for(int i=0; i<1024; ++i){
        memory[row[i]][col[i]]='#';
    }

    // Dijkstra
    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> q;
    std::map<std::pair<int,int>,int> costMap;

    q.push({0, 0, 0});

    while(!q.empty()){
        auto [cost, r, c]  = q.top();
        costMap[{r,c}]=cost;
        q.pop();
        for (int i=0; i<4; ++i){
            auto nr = r+dr[i];
            auto nc = c+dc[i];
            if(!isValidPos(memory, nr, nc, n)){
                continue;
            }
            if(memory[nr][nc]=='#'){
                continue;
            }
            if(costMap.find({nr,nc}) == costMap.end() || costMap[{nr,nc}]>cost+1){
                costMap[{nr,nc}]=cost+1;
                q.push({cost+1, nr, nc});
            }
        }
    }

    std::cout << "Minimum number of steps is: " << costMap[{n-1,n-1}] << std::endl;

    // Part 2
    
    std::set<std::pair<int,int>> visited;

    // Let's start from 1024 since we know there is an exit
    for(int i=1025; i<n*n; ++i){
        // Make another bit fall
        memory[row[i]][col[i]]='#';
        std::stack<std::pair<int,int>> stk;
        stk.push({0,0});

        // Perform BFS and mark each location as visited.
        while(!stk.empty()){
            auto [r, c] = stk.top();
            stk.pop();
            visited.insert({r,c});
            for (int i=0; i<4; ++i){
                auto nr = r+dr[i];
                auto nc = c+dc[i];
                if(!isValidPos(memory, nr, nc, n)){
                    continue;
                }
                if(memory[nr][nc]=='#'){
                    continue;
                }
                if(visited.find({nr,nc}) == visited.end()){
                    stk.push({nr, nc});
                }
            }
        }

        // If the final position was not visited, the path is blocked
        if (visited.find({70,70})==visited.end()){
            std::cout << "First bit that blocks the path is in position: " << row[i] << "," << col[i] << std::endl;
            break;
        }
        visited.clear();
    }

    return 0;
}