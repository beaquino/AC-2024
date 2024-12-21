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

bool canBeFormed(const std::string& s, const std::vector<std::string>& wordList){
    bool res = false;

    for (auto str : wordList){
        if (str.size()==s.size() && str == s){
            return true;
        }
        if(str.size()>s.size()){
            continue;
        }
        if (str == s.substr(0,str.size())){
            res = canBeFormed(s.substr(str.size()), wordList);
            if (res){
                return true;
            }
        }
    }
    return res;
}

long long int uniqueFormations(const std::string& s, const std::vector<std::string>& wordList){
    std::unordered_map<std::string, long long int> cache;
    std::function<long long int(const std::string)> uniqueFormationsRecursive;

    uniqueFormationsRecursive = [&cache, &uniqueFormationsRecursive, &wordList](const std::string current) -> long long int {
        long long int res=0;
        if (current.size()==0){
            return 1;
        }

        if (cache.find(current) != cache.end()) {
            return cache[current];
        }
        for (auto str : wordList){
            if (str.size() <= current.size() && str == current.substr(0,str.size())){
                res += uniqueFormationsRecursive(current.substr(str.size()));
            }
        }
        cache[current] = res;
        return res;
    };

    return uniqueFormationsRecursive(s);
}

int main() {

    // Read input
    const std::string& filename1 = "Data1.txt";

    std::vector<std::string> words;
    std::ifstream file1(filename1);

    // Variable to read each line
    std::string line1;
        
    // Check if file opened successfully
    if (!file1.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename1);
    }

    // Read file
    while (std::getline(file1, line1)) {
        std::stringstream ss(line1);
        std::string word;
        
        // Parse each word separated by comma
        while (std::getline(ss, word, ',')) {
            // Remove only the trailing space if it exists
            if (!word.empty() && word.back() == ' ') {
                word.pop_back();
            } 
            if (!word.empty() && word.front() == ' ') {
                word.erase(word.begin());
            }           
            words.push_back(word);
        }
    }

    file1.close();

    // Read input
    const std::string& filename2 = "Data2.txt";

    std::vector<std::string> sequences;
    std::ifstream file2(filename2);

    // Variable to read each line
    std::string line2;
        
    // Check if file opened successfully
    if (!file2.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename2);
    }
    
    while (std::getline(file2, line2)) {
        sequences.push_back(line2);
    }

    file2.close();

    // Part 1

    int counter1 = 0;
    for (auto s : sequences){
        if (canBeFormed(s, words)){
            ++counter1;
        }
    }

    std::cout << counter1 << std::endl;

    // Part 2

    long long int counter2 = 0;
    for (auto s : sequences){
        counter2 += uniqueFormations(s, words);
    }

    std::cout << counter2 << std::endl;

    return 0;
}