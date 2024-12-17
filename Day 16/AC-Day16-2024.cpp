#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <set>
#include <stdexcept>
#include <cctype>
#include <iomanip>

enum Direction {EAST, NORTH, WEST, SOUTH};

const std::vector<int> dr {0, -1,  0, 1};
const std::vector<int> dc {1,  0, -1, 0};

struct State{
    int posR, posC, dir, cost;
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

int main() {

    // Read input
    const std::string& filename = "Data.txt";

    std::vector<std::vector<char>> map;

    std::ifstream file(filename);
    // Variable to read each line
    std::string line1;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Save initial and final position
    std::pair<int, int> initialPos;
    std::pair<int, int> endPos;

    // Read file
    int r=0;
    while (std::getline(file, line1)) {
        int c=0;
        std::stringstream ss(line1);
        char el;
        std::vector<char> vec;
        while (ss >> el) {
            vec.push_back(el);
            if (el=='S'){
                initialPos = {r, c};
            }
            if (el=='E'){
                endPos = {r, c};
            }
            ++c;
        }
        map.push_back(vec);
        ++r;
    }

    file.close();

    // Part 1

    int nrows = map.size();
    int ncols = map[0].size();

    std::priority_queue<State,std::vector<State>,std::greater<State>> q;
    std::vector<std::vector<std::vector<int>>> scores(nrows, std::vector<std::vector<int>>(ncols, std::vector(4,INT_MAX)));

    State s1 = {initialPos.first, initialPos.second, EAST, 0};
    State s2 = {initialPos.first, initialPos.second, NORTH, 1000};
    scores[initialPos.first][initialPos.second][0]=0;
    scores[initialPos.first][initialPos.second][1]=1000;
    q.push(s1);    
    q.push(s2);

    while(!q.empty()){
        auto currentState = q.top();
        auto [posR, posC, dir, cost] = currentState;
        
        q.pop();

        for (int k=0; k<4; ++k){ 
            // No backtrack as it increases cost
            if((k+2)%4 == dir){
                continue;
            }
            // Ignore blocked directions
            if(map[posR+dr[k]][posC+dc[k]] == '#'){
                continue;
            }
            int turnCost = dir == k ? 0 : 1000;
                // Only advance if cost is smaller. Otherwise abandon this path
                if(scores[posR][posC][dir] + turnCost + 1 < scores[posR+dr[k]][posC+dc[k]][k]){
                    scores[posR+dr[k]][posC+dc[k]][k] = scores[posR][posC][dir] + turnCost + 1;
                    q.push({posR+dr[k], posC+dc[k], k, scores[posR+dr[k]][posC+dc[k]][k]});
                }
        }
    }

    // We can only arrive from south or west, since the final destination is on a corner
    int minFinalCost = std::min(scores[endPos.first][endPos.second][EAST], scores[endPos.first][endPos.second][NORTH]);
    std::cout << minFinalCost << std::endl;

    // Part 2

    // We will start from the end    
    std::stack <State> stk;
    for (int k=0; k<4; ++k){
        if (scores[endPos.first][endPos.second][k]==minFinalCost){
            stk.push({endPos.first, endPos.second, k, minFinalCost});
        }
    }

    int seats=0;

    while (!stk.empty()){
        auto [posR, posC, dir, cost] = stk.top();
        stk.pop();
        map[posR][posC] = 'O';
        
        for (int k=0; k<4; ++k){
            // Ignore blocked directions
            if(map[posR-dr[dir]][posC-dc[dir]] == '#'){
                continue;
            }

            int turnCost = k==dir ? 0 : 1000;
            // If the previous state has a compatible cost, then it was part of a path
            if(scores[posR-dr[dir]][posC-dc[dir]][k]+1+turnCost == cost){
                stk.push({posR-dr[dir], posC-dc[dir], k, cost-1-turnCost});
            }            
        }
    }

    for (auto row : map){
        for (auto c : row){
            if (c=='O'){
                // We count here because locations that are part of two paths would be counted twice
                seats++;
            }
        }
    }

    std::cout << seats << std::endl;

    return 0;
}