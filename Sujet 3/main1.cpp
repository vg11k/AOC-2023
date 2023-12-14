#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<int> * getValidNumbers(std::vector<string> lines, int lineIndex) {
    
    string lineToStudy = lines.at(lineIndex);
    int parserPosition = 0;
    vector<int> * results = new vector<int>();
    string symbols = "*@=-$%#+&/";

    while(parserPosition < lineToStudy.length()) {
        string data = lineToStudy.substr(parserPosition, lineToStudy.substr(parserPosition).find("."));

        if(data != "") {
            
            int dataLength = data.length();

            //commence par un symbole
            if(symbols.find(data[0]) != std::string::npos) {
                data = data.substr(0,1);
                dataLength = 1;
            }

            //fini par un symbole
            if(data.length() > 1 && symbols.find(data[data.length() - 1]) != std::string::npos) {
                data = data.substr(0, data.length() - 1);
                dataLength = data.length();
            }

            //contient un symbole
            for(int symbolIndex = 0; symbolIndex < symbols.length() && data.length() > 1; symbolIndex++) {
                if(data.find(symbols[symbolIndex]) != std::string::npos) {
                    data = data.substr(0, data.find(symbols[symbolIndex]));
                    dataLength = data.length();
                }
            }

            //si un nombre a ete trouve
            if(dataLength > 1 || (dataLength == 1 && symbols.find(data[0]) == std::string::npos)) { 
                bool isNumberValid = false;
                int parserPositionPlusDataLength = parserPosition + data.length();

                //si il y a potentiellement quelque chose a gauche
                if(parserPosition > 0) { 
                    if(symbols.find(lineToStudy[parserPosition - 1]) != std::string::npos) {
                        isNumberValid = true;
                    }
                    //diagonale haute
                    if(lineIndex > 0 && 
                        symbols.find(lines.at(lineIndex - 1)[parserPosition - 1]) != std::string::npos) 
                    {
                        isNumberValid = true;
                    }
                    //diagonale basse
                    if(lineIndex < lines.size() - 1 && 
                        symbols.find(lines.at(lineIndex + 1)[parserPosition - 1]) != std::string::npos) 
                    {
                        isNumberValid = true;
                    }
                }

                for(int dataCursor = 0; dataCursor < data.length(); dataCursor++) {
                    //au-dessus
                    if(lineIndex > 0 && 
                        symbols.find(lines.at(lineIndex - 1)[parserPosition + dataCursor]) != std::string::npos) 
                    {
                        isNumberValid = true;
                    }

                    //au-dessous
                    if(lineIndex < lines.size() - 1 && 
                        symbols.find(lines.at(lineIndex + 1)[parserPosition + dataCursor]) != std::string::npos) 
                    {
                        isNumberValid = true;
                    }
                }

                //si il y a potentiellement quelque chose a droite
                if(parserPositionPlusDataLength < lineToStudy.length() - 1) { 
                    if(symbols.find(lineToStudy[parserPositionPlusDataLength]) != std::string::npos)  {
                        isNumberValid = true;
                    }
                    //diagonale haute
                    if(lineIndex > 0 && 
                        symbols.find(lines.at(lineIndex - 1)[parserPositionPlusDataLength]) != std::string::npos) 
                    {
                        isNumberValid = true;
                    }
                    //diagonale basse
                    if(lineIndex < lines.size() - 1 && 
                        symbols.find(lines.at(lineIndex + 1)[parserPositionPlusDataLength]) != std::string::npos)
                    {

                        isNumberValid = true;
                    }
                }

                if(isNumberValid) {
                    results->push_back(std::stoi(data));
                }
            }
            parserPosition += dataLength;
        }
        else {
            parserPosition += 1;
        }
    }
    return results;
}


int main(char * args[]) {

    std::cout<< "Hello World !" << endl;

    std::ifstream infile("input.txt");
    std::string line;
    int result = 0;
    std::vector<std::string> allLines;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        string input;
        if (!(iss >> input)) { break; } // error
        allLines.push_back(line);
    }

    for(int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        std::string localLine = allLines.at(lineIndex);
        vector<int> * numbersVector = getValidNumbers(allLines, lineIndex);
        std::cout << lineIndex +1 << " has " << numbersVector->size() << " numbers : ";
        for(int nindex = 0; nindex < numbersVector->size(); nindex++) {
            int number = numbersVector->at(nindex);
            std::cout << number << " ";
            result += number;
        }
        delete numbersVector;

        std::cout << endl;
    }

    std::cout << "result : " << endl;
    std::cout << result << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}