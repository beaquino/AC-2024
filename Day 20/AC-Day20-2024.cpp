#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <cmath>
#include <set>
#include <stdexcept>
#include <functional>

const std::vector dr{0, 1, 0, -1}; 
const std::vector dc{1, 0, -1, 0}; // E, S, W, N

bool isValidPos(int r, int c, int nrows, int ncols){
    return r>=0 && c>=0 && r<nrows && c<ncols;
}

int raceTime(const std::vector<std::vector<char>>& raceTrack, std::vector<std::pair<int,int>>& path, std::vector<std::vector<int>>& costMatrix){
    const int nrows = raceTrack.size();
    const int ncols = raceTrack[0].size();
    std::pair<int,int> S, E;
    int finalCost = INT_MAX;

    for (int r=0; r<nrows; ++r){
        for (int c=0; c<ncols; ++c){
            if(raceTrack[r][c]=='S'){
                S = {r,c};
            }
            if(raceTrack[r][c]=='E'){
                E = {r,c};
            }
        }
    }

    std::priority_queue<std::tuple<int,int,int>, std::vector<std::tuple<int,int,int>>, std::greater<std::tuple<int,int,int>>> q;
    costMatrix[S.first][S.second] = 0;
    q.push({0, S.first, S.second});


    while(!q.empty()){
        auto [cost, r, c] = q.top();
        q.pop();
        if (r==E.first && c==E.second){
            finalCost = cost;
            break;
        }
        for (int d=0; d<4; ++d){
            int nR = r+dr[d];
            int nC = c+dc[d];
            if(isValidPos(nR, nC, nrows, ncols) && raceTrack[nR][nC]!='#' && cost+1<costMatrix[nR][nC]){
                q.push({cost+1, nR, nC});
                costMatrix[nR][nC] = cost+1;
            }
        }
    }

    std::stack<std::tuple<int,int, int>> stk;
    stk.push({finalCost, E.first, E.second});

    while(!stk.empty()){
        auto [cost, r, c] = stk.top();
        path.push_back({r,c});
        if (S.first==r && S.second == c){
            break;
        }
        stk.pop();
        for (int d=0; d<4; ++d){
            int nR = r+dr[d];
            int nC = c+dc[d];
            if(isValidPos(nR, nC, nrows, ncols) && costMatrix[nR][nC] != INT_MAX && costMatrix[nR][nC] == cost-1){
                stk.push({cost-1, nR, nC});
                break;
            }
        }
    }

    return finalCost;
}

int main() {

    // Read input
    const std::string& filename = "Data.txt";

    std::vector<std::vector<char>> raceTrack;
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
        raceTrack.push_back(row);
    }

    file.close();

    // Part 1

    std::vector<std::pair<int,int>> listOfObstacles;
    const int nrows = raceTrack.size();
    const int ncols = raceTrack[0].size();

    for (int r=0; r<nrows; ++r){
        for (int c=0; c<ncols; ++c){
            if(r!=0 && c!=0 && r!=nrows-1 && c!=ncols-1 && raceTrack[r][c]=='#'){
                listOfObstacles.push_back({r,c});
            }
        }
    }

    std::vector<std::vector<int>> costMatrixFair(nrows, std::vector<int>(ncols, INT_MAX));
    int countCheats = 0;
    std::vector<std::pair<int,int>> fairPath;
    
    int fairRaceTime = raceTime(raceTrack, fairPath, costMatrixFair);
    for (auto [r, c] : listOfObstacles){
        raceTrack[r][c] = '.';
        std::vector<std::pair<int,int>> path;
        std::vector<std::vector<int>> costMatrix(nrows, std::vector<int>(ncols, INT_MAX));
        countCheats += fairRaceTime-raceTime(raceTrack, path, costMatrix) >= 100 ? 1 : 0;
        raceTrack[r][c] = '#';
    }

    std::cout << countCheats << std::endl;

    // Part 2

    std::reverse(fairPath.begin(),fairPath.end());
    int countNewCheats=0;
    int nSteps = 20;
    int save = 100;
    auto it = fairPath.begin();
    for (auto [pr, pc] : fairPath){
        if (fairRaceTime - costMatrixFair[pr][pc] < save){
            ++it;
            continue;
        }
        for (int i =-nSteps; i<=nSteps; ++i){
            for (int j=-nSteps; j<=nSteps ; j++){
                if(abs(i)+abs(j)>nSteps){
                    continue;
                }
                if (std::find(it, fairPath.end(), std::make_pair(i+pr,j+pc)) != fairPath.end()){
                    int diff = costMatrixFair[i+pr][j+pc] - (costMatrixFair[pr][pc] + abs(i) + abs(j));
                    countNewCheats += diff >= save ? 1 : 0;
                }
            }
        }
        ++it;
    }

    std::cout << countNewCheats << std::endl;

    return 0;
}