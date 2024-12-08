#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include <string>

bool tryOperators(const long long int& res, const std::vector<long long int>& op, std::vector<long long int>::const_iterator end){
    if (op.begin()+1==end){
        if(op[0]==res){
            return true;
        }
        return false;
    }

    long long int last = *(end-1);

    bool trySum = tryOperators(res-last, op, end-1);

    if (trySum){
        return true;
    }

    bool tryProduct = (res % last ==0) ? tryOperators(res/last, op, end-1) : false;

    return tryProduct;
}

long long int concatNumbers(const long long int& a, const long long int& b){
    long long int factor=1;
    long long int tmp = b;
    while (tmp/10>0){
        tmp/=10;
        factor*=10;
    }
    return a*factor*10+b;
}

bool tryMoreOperators(const long long int& res, const std::vector<long long int>& op){
    std::queue<long long int> candidates;
    candidates.push(op[0]);
    long long int temp=0;
    for (int i=1; i<op.size(); ++i){
        int size = candidates.size();
        while (size>0){
            temp = candidates.front();
            candidates.pop();
            if (temp+op[i]<=res){
                candidates.push(temp+op[i]);
            }
            if (temp*op[i]<=res){
                candidates.push(temp*op[i]);
            }
            if (concatNumbers(temp, op[i])<=res){
                candidates.push(concatNumbers(temp, op[i]));
            }
            --size;
        }
    }

    while(!candidates.empty()){
        temp = candidates.front();
        if (temp==res){
            return true;
        }
        candidates.pop();
    }
    return false;
}

struct Equation{
    long long int result;
    std::vector<long long int> operands;
};


int main() {

    // Read input
    const std::string& filename = "Data.txt";

    std::ifstream file(filename);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read file
    char c;
    long long int num;
    std::vector<Equation> listEquations;

    while (std::getline(file, line)) {
        std::istringstream iss(line);

        Equation eq;

        iss >> eq.result >> c;
        
        while (iss >> num) {
            eq.operands.push_back(num);
        }

        listEquations.push_back(eq);
    }
    
    file.close();
    
    long long int p1 = 0;
    long long int p2 = 0;

    // Part 1
    for (auto eq : listEquations){
        if(tryOperators(eq.result, eq.operands, eq.operands.end())){
            p1+=eq.result;
        }        
    }

    std::cout << p1 << std::endl;

    // Part 2
    for (auto eq : listEquations){
        if(tryMoreOperators(eq.result, eq.operands)){
            p2+=eq.result;
        }
    }

    std::cout << p2 << std::endl;

    return 0;
}