#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <cctype>

int main() {

    // Read input
    const std::string& filename = "Data.txt";

    std::vector<int> vectorA; // Button A X offset
    std::vector<int> vectorB; // Button A Y offset
    std::vector<int> vectorC; // Button B X offset
    std::vector<int> vectorD; // Button B Y offset
    std::vector<int> vectorE; // Prize X coordinate
    std::vector<int> vectorF; // Prize Y coordinate

    // String to store each line from the file
    std::string line;

    std::ifstream file(filename);

     // Check if file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }


    // Read the file line by line
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        // Parse Button A
        if (line.find("Button A:") != std::string::npos) {
            size_t xPos = line.find("X+");
            size_t yPos = line.find("Y+");
            
            if (xPos != std::string::npos && yPos != std::string::npos) {
                int xValue = std::stoi(line.substr(xPos + 2, yPos - xPos - 3));
                int yValue = std::stoi(line.substr(yPos + 2));
                
                vectorA.push_back(xValue);
                vectorC.push_back(yValue);
            }
        }
        // Parse Button B
        else if (line.find("Button B:") != std::string::npos) {
            size_t xPos = line.find("X+");
            size_t yPos = line.find("Y+");
            
            if (xPos != std::string::npos && yPos != std::string::npos) {
                int xValue = std::stoi(line.substr(xPos + 2, yPos - xPos - 3));
                int yValue = std::stoi(line.substr(yPos + 2));
                
                vectorB.push_back(xValue);
                vectorD.push_back(yValue);
            }
        }
        // Parse Prize
        else if (line.find("Prize:") != std::string::npos) {
            size_t xPos = line.find("X=");
            size_t yPos = line.find("Y=");
            
            if (xPos != std::string::npos && yPos != std::string::npos) {
                int xValue = std::stoi(line.substr(xPos + 2, yPos - xPos - 2));
                int yValue = std::stoi(line.substr(yPos + 2));
                
                vectorE.push_back(xValue);
                vectorF.push_back(yValue);
            }
        }
    }

    file.close();

    int a, b, c, d;
    long long int e,f;
    int det;
    std::vector<long long int> tokens = {0, 0};
    std::vector<long long int> calibration = {0, 10000000000000};

    for (int part=0; part<2; ++part){
        for (int i=0; i<vectorA.size(); ++i){
            a = vectorA[i];
            b = vectorB[i];
            c = vectorC[i];
            d = vectorD[i];
            e = vectorE[i]+calibration[part];
            f = vectorF[i]+calibration[part];

            // Find the determinant
            //
            // | a b |
            // | c d |
            //
            // det = ad-bc

            det = a*d-b*c;

            // Solution is:
            //
            // [ a b ][x] = [e]
            // [ c d ][y] = [f]
            //
            //
            // det*x = |e b|
            //         |f d|
            //
            // det* y = |a e|
            //          |c f|

            long long int nx = d*e-b*f;
            long long int ny = a*f-c*e;

            if (det!=0){
                long long int x = nx/det;
                long long int y = ny/det;

                if(nx%det==0 && ny%det==0 && x>=0 && y>=0){
                    tokens[part]+=3*x+y;
                }   
            }else{
                if(e%a==0 && f%b==0 && e/a==f/b){
                    if(a<c){
                        tokens[part]+=3*a+b;
                    }
                    else{
                        tokens[part]+=3*c+d;
                    }
                }
            }
        }

        std::cout << tokens[part] << std::endl;
    }

    return 0;
}