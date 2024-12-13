#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <cctype>

std::vector<std::pair<int, int>> directions={
    {0, 1},  // Right
    {0, -1}, // Left
    {1, 0},  // Down
    {-1, 0}  // Up
};

std::vector<std::pair<int, int>> corner{
    {-1, -1}, // NW 
    {-1, 1},  // NE
    {1, -1},  // SW
    {1, 1}    // SE
};

bool validPos(int row, int col, int nrow, int ncol){
    return row>=0 && row<=nrow-1 & col>=0 && col<=ncol-1;
}

int main() {

    // Read input
    const std::string& filename = "Data.txt";

    // Ordering does not matter that much and problem does not scale well using linked lists or other sequential container
    std::vector<std::vector<char>> gardenPlots;

    std::ifstream file(filename);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read file
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        char el;
        std::vector<char> vec;
        while (ss >> el) {
            vec.push_back(el);
        }
        gardenPlots.push_back(vec);
    }

    file.close();

    // Part 1

    std::stack<std::pair<int,int>> stk;
    int nrow = gardenPlots.size();
    int ncol = gardenPlots[0].size();
    int nFences = 0;
    int nCorners = 0;
    int nPlots = 0;
    int totalPrice = 0;
    int totalPrice2 = 0;
    char current;

    for(int r=0; r<nrow; ++r){
        for(int c=0; c<ncol; ++c){
            // Get the current upper case letter
            current = gardenPlots[r][c];
            nFences = 0;
            nCorners = 0;
            nPlots = 0;
            if(islower(current)){
                continue;
            }
            // If not visited push and check neighbours
            stk.push({r,c});
            // Mark this plot as visited by lowering the letter
            gardenPlots[r][c] = tolower(gardenPlots[r][c]);
            while(!stk.empty()){
                // Get the element on top of the stack
                auto [nr, nc] = stk.top();
                stk.pop();
                // Count this plot
                ++nPlots;

                // Push all directions
                for(int d=0; d<4; ++d){
                    auto [dr, dc] = directions[d];
                    // Next position is not outside the grid and has the same letter as the current one
                    if(validPos(nr+dr, nc+dc, nrow, ncol) && toupper(gardenPlots[nr+dr][nc+dc])==current){
                        // If we have not visited it we push it to the stack
                        if(isupper(gardenPlots[nr+dr][nc+dc])){
                            stk.push({nr+dr,nc+dc});
                            // Mark this plot as visited by lowering the letter
                            gardenPlots[nr+dr][nc+dc] = tolower(gardenPlots[nr+dr][nc+dc]);
                        }
                    }
                    else{
                        // If this direction does not have a valid position or has a different letter, we need a fence.
                        ++nFences;
                    }
                }
                // Number of sides in a plot = Number of corners! 
                
                // Check if this is a corner
                //
                // o -    - o    A o    o A    
                // A o or o A or o - or - o
                //
                // OR
                //
                // A o    o A    A A    A A
                // A A or A A or A o or o A
                //
                //
                // 'o' is either a different letter or out of the range. '-' does not matter


                for (int c=0; c<4; ++c){
                    auto [dr, dc] = corner[c];
                    if((!validPos(nr+dr,nc,nrow,ncol) || toupper(gardenPlots[nr+dr][nc])!=current) && (!validPos(nr,nc+dc,nrow,ncol) || toupper(gardenPlots[nr][nc+dc])!=current)){
                        ++nCorners;
                    }

                    if(validPos(nr+dr,nc,nrow,ncol) && toupper(gardenPlots[nr+dr][nc])==current && validPos(nr,nc+dc,nrow,ncol) && toupper(gardenPlots[nr][nc+dc])==current && toupper(gardenPlots[nr+dr][nc+dc])!=current){
                        ++nCorners;
                    }
                }
            }

            // After we're done with a region compute its price
            totalPrice+=(nFences*nPlots);
            totalPrice2+=(nCorners*nPlots);
        }
    }

    std::cout << totalPrice << std::endl;

    // Part 2

    std::cout << totalPrice2 << std::endl;

    return 0;
}