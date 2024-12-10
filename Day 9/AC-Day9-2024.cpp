#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <string>

int main() {

    // Read input
    const std::string& filename = "Data.txt";

    std::vector<int> diskMap;

    std::ifstream file(filename);
    // Variable to read each line
    std::string line;
        
    // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read file
    while (std::getline(file, line)) {
        for (const char& c : line) {
            diskMap.push_back(int(c-'0'));
        }
    }
    
    file.close();

    // Part 1
    std::vector<int> decodedMap;
    // We need these two maps of start block index- block size to make Part 2 easier    
    std::map<int, int> dataStartSize;
    std::map<int, int> spaceStartSize;
    int id=0;
    int startPos = 0;

    for (int i=0; i<diskMap.size(); ++i){
        if(i%2==0){
            // Even is size of file
            startPos = decodedMap.size();
            for (int j=0; j<diskMap[i]; ++j){
                // Let's write id for occupied spaces
                decodedMap.push_back(id);
            }
            dataStartSize[startPos]=decodedMap.size()-startPos;
            ++id;
        }        
        else{
            //Odd is size of space
            startPos = decodedMap.size();
            for (int j=0; j<diskMap[i]; ++j){
                decodedMap.push_back(-1);
            }
            spaceStartSize[startPos]=decodedMap.size()-startPos;
        }
    }

    // Making a copy since we need decodedMap intact for the next part
    std::vector decodedMapP1 = decodedMap;
    auto it1 = decodedMapP1.begin();
    auto it2 = decodedMapP1.end()-1;

    while (it1<it2){
        // Advance first pointer until we find a space
        if(*it1!=-1){
            ++it1;
            continue;
        }
        // Make sure second pointer is pointing to valid data
        while(*it2==-1){
            --it2;
        }
        // Swap position
        std::swap(*it1, *it2);
    }

    long long int checksum=0;

    for (int i=0; i<decodedMapP1.size(); ++i){
        if(decodedMapP1[i]==-1){
            break;
        }
        checksum+=i*decodedMapP1[i];
    }    

    std::cout << checksum << std::endl;

    // Part 2

    auto pf = dataStartSize.end();
    --pf;
    // We sweep all data blocks backwards
    while(pf!=dataStartSize.begin()){
        
        // Find enough space at the beggining
        for (auto [start, size] : spaceStartSize){
            // If the empty space is after the data, ignore
            if (start>pf->first){
                break;
            }
            // If there is enough space in the empty block, we allocate
            if (size>=pf->second){
                //Swap
                for(int i=0; i<pf->second; ++i){
                    std::swap(decodedMap[start+i], decodedMap[pf->first+i]);
                }
                //Update Space
                spaceStartSize.erase(start);
                if (size-pf->second>0){
                    spaceStartSize[start+pf->second] = size-pf->second;
                }
                
                break;
            }
        }
        --pf;
    }

    long long int checksum2=0;

    for (int i=0; i<decodedMap.size(); ++i){
        if(decodedMap[i]==-1){
            continue;
        }
        checksum2+=i*decodedMap[i];
    }    

    std::cout << checksum2 << std::endl;

    return 0;
}