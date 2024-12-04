#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <stdexcept>
#include <string>

//This needs DRY badly :/
bool checkBackwards(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    return idx>=3 && matrix[ridx][idx-1]=='M' && matrix[ridx][idx-2]=='A' && matrix[ridx][idx-3]=='S';
}

bool checkForward(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    return idx<matrix[ridx].size()-3 && matrix[ridx][idx+1]=='M' && matrix[ridx][idx+2]=='A' && matrix[ridx][idx+3]=='S';
}

bool checkUpwards(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    return ridx>=3 && matrix[ridx-1][idx]=='M' && matrix[ridx-2][idx]=='A' && matrix[ridx-3][idx]=='S';
}

bool checkDownwards(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    return ridx<matrix.size()-3 && matrix[ridx+1][idx]=='M' && matrix[ridx+2][idx]=='A' && matrix[ridx+3][idx]=='S';
}

bool checkNW(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    return idx>=3 && ridx>=3 && matrix[ridx-1][idx-1]=='M' && matrix[ridx-2][idx-2]=='A' && matrix[ridx-3][idx-3]=='S';
}

bool checkSW(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    return idx>=3 && ridx<matrix.size()-3 && matrix[ridx+1][idx-1]=='M' && matrix[ridx+2][idx-2]=='A' && matrix[ridx+3][idx-3]=='S';
}

bool checkNE(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    return ridx>=3 && idx<matrix[0].size()-3 && matrix[ridx-1][idx+1]=='M' && matrix[ridx-2][idx+2]=='A' && matrix[ridx-3][idx+3]=='S';
}

bool checkSE(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    return idx<matrix[0].size()-3 && ridx<matrix.size()-3 && matrix[ridx+1][idx+1]=='M' && matrix[ridx+2][idx+2]=='A' && matrix[ridx+3][idx+3]=='S';
}

bool checkMAS(int ridx, int idx, const std::vector<std::vector<char>>& matrix){
    bool validPos = idx>=1 && ridx>=1 && idx<matrix[0].size()-1 && ridx<matrix.size()-1;
    if (!validPos){
        return false;
    }
    bool firstDiag = (matrix[ridx-1][idx-1]=='S' && matrix[ridx+1][idx+1]=='M') || (matrix[ridx-1][idx-1]=='M' && matrix[ridx+1][idx+1]=='S');
    bool secondDiag =(matrix[ridx-1][idx+1]=='S' && matrix[ridx+1][idx-1]=='M') || (matrix[ridx-1][idx+1]=='M' && matrix[ridx+1][idx-1]=='S');
    return firstDiag && secondDiag;
}

int main() {
    
    // Read input
    const std::string& filename = "Data.txt";

    std::vector<std::vector<char>> wordSearch;

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
            wordSearch.push_back(row);
        }
    }
    
    file.close();

    //First Part

    int totalCount=0;

    for (int row=0; row<wordSearch.size(); ++row){
        for (int c=0; c<wordSearch[0].size(); ++c){

            if (wordSearch[row][c] == 'X'){                
                // Check all 8 directions. Could be DRYer...

                //Back
                if(checkBackwards(row, c, wordSearch)){
                    ++totalCount;
                }

                //Forward
                if(checkForward(row, c, wordSearch)){
                    ++totalCount;
                }

                //Upwards
                if(checkUpwards(row, c, wordSearch)){
                    ++totalCount;
                }

                //Downwards
                if(checkDownwards(row, c, wordSearch)){
                    ++totalCount;
                }

                //NW
                if(checkNW(row, c, wordSearch)){
                    ++totalCount;
                }

                //SW
                if(checkSW(row, c, wordSearch)){
                    ++totalCount;
                }

                //NE
                if(checkNE(row, c, wordSearch)){
                    ++totalCount;
                }

                //SE
                if(checkSE(row, c, wordSearch)){
                    ++totalCount;
                }
            }
        }
    }
    
    std::cout << totalCount << std::endl;

    //Second Part

    int xmasCount=0;

    for (int row=0; row<wordSearch.size(); ++row){
        for (int c=0; c<wordSearch[0].size(); ++c){
            if (wordSearch[row][c] == 'A'){
                if (checkMAS(row, c, wordSearch)){
                    ++xmasCount;
                }
            }
        }
    }

    std::cout << xmasCount << std::endl;

    return 0;
}
