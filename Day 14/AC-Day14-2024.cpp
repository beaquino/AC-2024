#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <string>

int nrows = 103;
int ncols = 101;

std::vector<double> calculateVariance(const std::vector<std::pair<int,int>>& pos) {

    std::vector<double> sum = {0.0, 0.0};
    for (const auto& [r, c] : pos) {
        sum[0] += r;
        sum[1] += c;
    }
    std::vector<double> mean = {sum[0]/pos.size(), sum[1]/pos.size()}; 

    std::vector<double> squaredDeviationsSum = {0.0, 0.0};
    for (const auto& [r, c] : pos) {
        squaredDeviationsSum[0] += pow(r - mean[0], 2);
        squaredDeviationsSum[1] += pow(c - mean[1], 2);
    }

    squaredDeviationsSum[0]/=pos.size();
    squaredDeviationsSum[1]/=pos.size();
    
    return squaredDeviationsSum;
}

void printGrid(const std::vector<std::pair<int,int>>& pv){
        for(int r=0; r<nrows; ++r){
            for(int c=0; c<ncols; ++c){
                int cnt = std::count(pv.begin(), pv.end(), std::pair<int,int>(r,c));
                if(cnt!=0){
                    std::cout << cnt;
                }
                else{
                    std::cout << ".";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
}

void printFinalGrid(const std::vector<std::pair<int,int>>& pv){
        for(int r=0; r<nrows; ++r){
            for(int c=0; c<ncols; ++c){
                if ((r==(nrows-1)/2) || (c==(ncols-1)/2)){
                    std::cout << " ";
                    continue;
                }
                int cnt = std::count(pv.begin(), pv.end(), std::pair<int,int>(r,c));
                if(cnt!=0){
                    std::cout << cnt;
                }
                else{
                    std::cout << ".";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
}

int main() {

    std::vector<std::pair<int, int>> pInitial;
    std::vector<std::pair<int, int>> v;

    // Read input
    const std::string& filename = "Data.txt";

    // Open the input file
    std::ifstream file(filename);
    
    // Check if the file is opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // String to store each line from the file
    std::string line;

    // Read the file line by line
    while (std::getline(file, line)) {

        // Create a stringstream from the line
        std::istringstream iss(line);
        std::string p_token, v_token;

        // Parse p and v parts
        if (std::getline(iss, p_token, ' ') && std::getline(iss, v_token)) {
            // Parse p coordinates
            if (p_token.substr(0,2) == "p=") {
                std::istringstream p_ss(p_token.substr(2));
                int x, y;
                char comma;
                if (p_ss >> x >> comma >> y) {
                    pInitial.emplace_back(y, x);
                }
            }

            // Parse v coordinates
            if (v_token.substr(0,2) == "v=") {
                std::istringstream v_ss(v_token.substr(2));
                int x, y;
                char comma;
                if (v_ss >> x >> comma >> y) {
                    v.emplace_back(y, x);
                }
            }
        }
    }

    file.close();

    int nSteps = 10000;
    int firstQuadrant = 0;
    int secondQuadrant = 0;
    int thirdQuadrant = 0;
    int fourthQuadrant = 0;
    std::vector<std::pair<int, int>> p = pInitial;

    // Part 1
    

    for (int i=0; i<p.size(); ++i){
        p[i].first=(p[i].first+nSteps*v[i].first) % nrows;
        if(p[i].first<0){
            p[i].first+=nrows;
        }
        p[i].second=(p[i].second+nSteps*v[i].second) % ncols;
        if(p[i].second<0){
            p[i].second+=ncols;
        }
        if((p[i].first<(nrows-1)/2) && (p[i].second<(ncols-1)/2)){
            ++firstQuadrant;
            continue;
        }
        if((p[i].first<(nrows-1)/2) && (p[i].second>(ncols-1)/2)){
            ++secondQuadrant;
            continue;
        }
        if((p[i].first>(nrows-1)/2) && (p[i].second>(ncols-1)/2)){
            ++thirdQuadrant;
            continue;
        }
        if((p[i].first>(nrows-1)/2) && (p[i].second<(ncols-1)/2)){
            ++fourthQuadrant;
            continue;
        }
    }

    std::cout << firstQuadrant*secondQuadrant*thirdQuadrant*fourthQuadrant << std::endl;

    // Part 2

    p = pInitial;
    std::vector<double> variance;

    for(int n=0; n<nSteps; ++n){
        for (int i=0; i<p.size(); ++i){
            p[i].first=(p[i].first+v[i].first) % nrows;
            if(p[i].first<0){
                p[i].first+=nrows;
            }
            p[i].second=(p[i].second+v[i].second) % ncols;
            if(p[i].second<0){
                p[i].second+=ncols;
            }
        }
        // Visually confirm the christmas tree. Change 10000 for the minimum variance element position.
        if (n==10000){
            printGrid(p);
            std::cout << std::endl;
        }
        auto temp = calculateVariance(p);
        variance.push_back(temp[0]+temp[1]);
    }

    auto it = std::min_element(variance.begin(), variance.end());
    int d = std::distance(variance.begin(),it);
    std::cout << *it << " " << d;

    return 0;
}