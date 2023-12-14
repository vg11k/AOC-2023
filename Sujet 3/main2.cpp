#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void feedAllPosition(std::vector<string> lines, std::vector<std::vector<string>*> * allNumbers, std::vector<std::vector<int>*> * allNumbersPositions) {

    string symbols = "*@=-$%#+&/";

    for(int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
        
        string lineToStudy = lines.at(lineIndex);
        int parserPosition = 0;


        std::vector<string> * numbersAsStrings = new std::vector<string>();
        std::vector<int> * numbersPositions = new std::vector<int>();


        allNumbers->push_back(numbersAsStrings);
        allNumbersPositions->push_back(numbersPositions);


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
                    numbersAsStrings->push_back(data);
                    numbersPositions->push_back(parserPosition);
                }
               parserPosition += dataLength;
            }
            else {
                parserPosition += 1;
            }
        }

    }
}

vector<int> * getGearsNumbers(std::vector<string> lines, int lineIndex, std::vector<std::vector<string>*> * allNumbers, std::vector<std::vector<int>*> * allNumbersPositions) {
    vector<int> * result = new vector<int>();

    string currentLine = lines.at(lineIndex);

    std::vector<string>* allNumbersPrevLine = NULL;
    std::vector<string>* allNumbersLine = allNumbers->at(lineIndex);
    std::vector<string>* allNumbersNextLine = NULL;

    std::vector<int>* allNumberPositionPrevLine = NULL;
    std::vector<int>* allNumberPositionLine = allNumbersPositions->at(lineIndex);   
    std::vector<int>* allNumberPositionNextLine = NULL;   

    if(lineIndex > 0) {
        allNumbersPrevLine = allNumbers->at(lineIndex - 1);
        allNumberPositionPrevLine = allNumbersPositions->at(lineIndex - 1); 
    }

    if(lineIndex < lines.size() - 1) {
        allNumbersNextLine = allNumbers->at(lineIndex + 1);
        allNumberPositionNextLine = allNumbersPositions->at(lineIndex + 1); 
    }

    for(int cursor = 0; cursor < currentLine.length(); cursor++) {
        if(currentLine[cursor] == '*') {

            vector<int> elligibleNumbers;

            for(int numIndex = 0; numIndex < allNumbersLine->size(); numIndex++) {
                string numberAsString = allNumbersLine->at(numIndex);
                int numberPosition = allNumberPositionLine->at(numIndex);

                if((numberPosition >= cursor -1 && numberPosition <= cursor + 1) ||
                    (numberPosition + numberAsString.length() >= cursor && numberPosition + numberAsString.length() <= cursor + 1))
                {
                    elligibleNumbers.push_back(std::stoi(numberAsString));
                }
            }

            if(allNumbersPrevLine != NULL) {
                for(int numIndex = 0; numIndex < allNumbersPrevLine->size(); numIndex++) {
                    string numberAsString = allNumbersPrevLine->at(numIndex);
                    int numberPosition = allNumberPositionPrevLine->at(numIndex);

                    if((numberPosition >= cursor -1 && numberPosition <= cursor + 1) ||
                        (numberPosition + numberAsString.length() >= cursor && numberPosition + numberAsString.length() <= cursor + 1))
                    {
                        elligibleNumbers.push_back(std::stoi(numberAsString));
                    }
                    
                }
            }

            if(allNumbersNextLine != NULL) {
                for(int numIndex = 0; numIndex < allNumbersNextLine->size(); numIndex++) {
                    string numberAsString = allNumbersNextLine->at(numIndex);
                    int numberPosition = allNumberPositionNextLine->at(numIndex);

                    if((numberPosition >= cursor -1 && numberPosition <= cursor + 1) ||
                        (numberPosition + numberAsString.length() >= cursor && numberPosition + numberAsString.length()<= cursor + 1))
                    {
                        elligibleNumbers.push_back(std::stoi(numberAsString));
                    }
                }
            }

            if(elligibleNumbers.size() == 2) {
                result->push_back(elligibleNumbers.at(0) * elligibleNumbers.at(1));
            }
        }
    }

    return result;
}

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

    std::vector<std::vector<std::string>*> * allNumbers = new std::vector<std::vector<std::string>*>();
    std::vector<std::vector<int>*> * allNumbersPositions = new std::vector<std::vector<int>*>();

    feedAllPosition(allLines, allNumbers, allNumbersPositions);

    for(int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        vector<int> * gearsNumbers = getGearsNumbers(allLines, lineIndex, allNumbers, allNumbersPositions);
        std::cout << lineIndex +1 << " has " << gearsNumbers->size() << " numbers : ";
        for(int nindex = 0; nindex < gearsNumbers->size(); nindex++) {
            int number = gearsNumbers->at(nindex);
            std::cout << number << " ";
            result += number;
        }
        std::cout << endl;
        delete gearsNumbers;
    }
    

    /*
    for(int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        //int lineIndex = 120;
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
*/
    std::cout << "result : " << endl;
    std::cout << result << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}