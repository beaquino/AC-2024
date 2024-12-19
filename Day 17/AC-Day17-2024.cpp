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

long long int findNumber(const std::vector<int>& v){
        long long int result=0;
        for (int i=0; i<v.size(); ++i){
            result+=v[i]*pow(8,15-i);
        }
        return result;
}

bool compareVectorsIndex(const std::vector<int>& v1, const std::vector<int>& v2, int l){
    if(v1.size()!=v2.size()){
        return false;
    }
    for(int i=0; i<l; ++i){
        if(v1[v1.size()-1-i]!=v2[v1.size()-1-i]){
            return false;
        }
    }
    return true;
}

void computer(std::vector<long long int> ABC, const std::vector<int>& program, std::vector<int>& output, bool earlyBreak){
    long long int operand;
    long long int counter=0;
    double den;
    for (int ip = 0; ip<program.size();){
        switch (program[ip]){
            // adv
            case 0:
                counter = (program[ip+1]<4) ? program[ip+1] : ABC[program[ip+1]-4];
                ABC[0] = ABC[0] >> counter;
                ip+=2;
                break;
            case 1:
                ABC[1] = ABC[1] ^ program[ip+1];
                ip+=2;
                break;
            case 2:
                operand = (program[ip+1]<4) ? program[ip+1] : ABC[program[ip+1]-4];
                ABC[1] = operand % 8;
                ip+=2;
                break;
            case 3:
                if(ABC[0]!=0){
                    ip = program[ip+1];
                }
                else{
                    ip+=2;
                }
                break;
            case 4:
                ABC[1] = ABC[1] ^ ABC[2];
                ip+=2;
                break;
            case 5:
                operand = (program[ip+1]<4) ? program[ip+1] : ABC[program[ip+1]-4];
                output.push_back(operand % 8);
                if (earlyBreak && (output.back() != program[output.size()-1])){
                    return;
                }
                ip+=2;
                break;
            case 6:
                counter = (program[ip+1]<4) ? program[ip+1] : ABC[program[ip+1]-4];
                ABC[1] = ABC[0] >> counter;
                ip+=2;
                break;
            case 7:
                counter = (program[ip+1]<4) ? program[ip+1] : ABC[program[ip+1]-4];
                ABC[2] = ABC[0] >> counter;;
                ip+=2;
                break;
            default:
                ip+=2;
                break;
        }
    }
}

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

    // Read file
    while (std::getline(file, line1)) {
        std::stringstream ss(line1);
        char el;
        std::vector<char> vec;
        while (ss >> el) {
            vec.push_back(el);
        }
        map.push_back(vec);
    }

    file.close();

    // Personal data removed
    long long int inputA = 0;
    std::vector<int> program = {};

    std::vector<long long int> ABC = {inputA, 0, 0};   
   
    // Part 1

    std::vector<int> output;
    
    computer(ABC, program, output, false);

    std::cout << "A: " << ABC[0] << " B: " << ABC[1] << " C: " << ABC[2] << std::endl;
    std::cout << "Output: ";
    for (auto el : output){
        std::cout << el << " ";
    }
    std::cout << std::endl;

    // Part 2

    // Program: {2,4,1,3,7,5,4,0,1,3,0,3,5,5,3,0}
    //1 B = A%8
    //3 B = B^3  
    //5 C = A >> B
    //7 A = A ^ B
    //9 B = B^3 
    //11 A = A >> 3 
    //13 output = B % 8
    //15 goto 3

    // We can think of this program operating on 3 bits at a time, so we will use octal base decomposition

    ABC = {0,0,0};
    output.clear();
    std::vector<int> digitCandidates;
    long long int candidateNumber, solution;
    std::priority_queue<long long int, std::vector<long long int>, std::greater<long long int>> q;
    std::unordered_map<long long int, std::vector<int>> numberDigitMap;

    digitCandidates.push_back(3);
    candidateNumber = findNumber(digitCandidates);
    q.push(candidateNumber);
    numberDigitMap[candidateNumber] = digitCandidates;
    bool found = false;

    while(!q.empty() && !found){
        candidateNumber = q.top();
        q.pop();
        digitCandidates = numberDigitMap[candidateNumber];
        for (int i=0; i<8; ++i){
            long long int newNumber = candidateNumber + i*pow(8,15-digitCandidates.size());
            ABC[0] = newNumber;
            computer(ABC,program,output,false);
            if(compareVectorsIndex(output, program, digitCandidates.size()+1)){
                if(digitCandidates.size()==15){
                    found = true;
                    solution = newNumber;
                    std::cout << "Solution: " << solution << std::endl;
                    break;
                }
                q.push(newNumber);
                digitCandidates.push_back(i);
                numberDigitMap[candidateNumber] = digitCandidates;
                digitCandidates.pop_back();
            }
            output.clear();
        }
    }

    output.clear();
    ABC[0] = solution;
    computer(ABC,program,output,false);
    std::cout << "Sequence: ";
    for (auto el : output){
        std::cout << el << " ";
    }
    std::cout << std::endl;

    return 0;
}