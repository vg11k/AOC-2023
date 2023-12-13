#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

string getTargetValuesFrom(string input) {

    string firstCar = "";
    string lastCar = "";
    string output = "";

    for(int i = 0; i < input.length(); i++) {
        char c = input[i];
        if(c > 47 && c < 58) { //ascii values of numbers
            if(firstCar == "") {
                firstCar = c;
                lastCar = c;
            }
            else {
                lastCar = c;
            }
        }
    }
    output = firstCar + lastCar;
    return output;
}


int main(char * args[]) {
    cout << "Hello World ! " << endl;

    std::ifstream infile("input.txt");
    std::string line;
    int result = 0;
    
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        string input;
        if (!(iss >> input)) { break; } // error
        string output = getTargetValuesFrom(input);
        cout << input << " ==> " << output << endl;
        result += std::stoi( output );
    }

    cout << "result : " << result << endl;

    cout << "Bye World" << endl;
    return 0;
}