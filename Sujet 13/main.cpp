#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

typedef pair<string, string> pstring;

static const int ARRAY_LENGTH = 100;

//static const string filename = "example.txt";
static const string filename = "input.txt";

class Pattern {
    private:
        pstring m_verticalPossibleReflexions[ARRAY_LENGTH];
        pstring m_horizontalPossibleReflexions[ARRAY_LENGTH];
        vector<string> * m_originalLines;
        int m_hlength;
        int m_vlength;
        long m_verticalReflexionScore;
        long m_horizontalReflexionScore;
        long m_verticalSmudgeScore;
        long m_horizontalSmudgeScore;
    public:
        Pattern() {m_originalLines = new vector<string>();}
        long getHorizontalReflexionScore() { return m_horizontalReflexionScore;}
        long getVerticalReflexionScore() { return m_verticalReflexionScore;}
        long getHorizontalSmudgeScore() { return m_horizontalSmudgeScore;}
        long getVerticalSmudgeScore() {return m_verticalSmudgeScore;}

        void addOriginalLine(string line) {
            m_originalLines->push_back(line); 
            m_vlength = m_originalLines->size(); 
            m_hlength = line.length();
        }
        void setUpReflexions() {
            for(int i = 0; i < m_vlength; i++) {
                string upString;
                string botString;
                for(int j = 0; j < i + 1 && i + 1 + j  < m_vlength; j++) {
                    upString += m_originalLines->at(i-j);
                    botString += m_originalLines->at(i+1+j);
                }
                if(botString != "" && upString !="")
                    m_verticalPossibleReflexions[i] = pstring(upString, botString);
            }

            string columns[100];
            for(int i = 0; i < m_hlength; i++) {
                string s = "";
                for(int j = 0; j < m_vlength; j++)
                    s = s + m_originalLines->at(j)[i];
                columns[i] = s;
            }

            for(int i = 0; i < m_hlength-1; i++) {
                string leftString;
                string rightString;
                for(int j = 0; j < i + 1 && i + 1 + j  < m_hlength; j++) {
                    leftString += columns[i-j];
                    rightString += columns[i+j+1];
                }
                if(leftString != "" && rightString !="")
                    m_horizontalPossibleReflexions[i] = pstring(leftString, rightString);
            }
        }

        void computeReflexionScore() {
            m_verticalReflexionScore = 0;
            for(int i = 0; i < m_vlength-1; i++) {
                pstring currentPair = m_verticalPossibleReflexions[i];
                if(currentPair.first == currentPair.second) m_verticalReflexionScore+=i+1;
            }

            m_horizontalReflexionScore = 0;
            for(int i = 0; i < m_hlength-1; i++) {
                pstring currentPair = m_horizontalPossibleReflexions[i];
                if(currentPair.first == currentPair.second)m_horizontalReflexionScore+=i+1;
            }
        }

        void computeSmudgeScore() {
            m_verticalSmudgeScore = 0;
            for(int i = 0; i < m_vlength-1; i++) {
                pstring currentPair = m_verticalPossibleReflexions[i];
                int nbSmudges = 0;
                for(int j = 0; j < currentPair.first.length() && nbSmudges < 2; j++) {
                    if(currentPair.first[j] != currentPair.second[j]) {
                        nbSmudges++;
                    }
                }
                if(nbSmudges == 1) m_verticalSmudgeScore+=i+1;
            }

            m_horizontalSmudgeScore = 0;
            for(int i = 0; i < m_hlength-1; i++) {
                pstring currentPair = m_horizontalPossibleReflexions[i];
                int nbSmudges = 0;
                for(int j = 0; j < currentPair.first.length() && nbSmudges < 2; j++) {
                    if(currentPair.first[j] != currentPair.second[j]) {
                        nbSmudges++;
                    }
                }

                if(nbSmudges == 1) m_horizontalSmudgeScore+=i+1;
            }


        }

};

int main(char * args[]) {

    cout<< "Hello World !" << endl;

    bool debug = false;

    clock_t start, end;

    ifstream infile(filename);
    string line;
    long long result1 = 0;
    long long result2 = 0;
    vector<string> allLines;

    start = clock();

    while(getline(infile, line)) {
        allLines.push_back(line);
    }

    vector<Pattern*> patterns;
    for(int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        Pattern * currentPattern;
        if(patterns.size() == 0 || allLines.at(lineIndex) == "") {
            currentPattern = new Pattern();
            patterns.push_back(currentPattern);
        }
        else currentPattern = patterns.back();
        if(allLines.at(lineIndex) != "")
            currentPattern->addOriginalLine(allLines.at(lineIndex));
    }


    for(int patternIndex = 0; patternIndex < patterns.size(); patternIndex++) {
        Pattern * pattern = patterns.at(patternIndex);
        pattern->setUpReflexions();
        pattern->computeReflexionScore();
        pattern->computeSmudgeScore();
        result1 += pattern->getHorizontalReflexionScore();
        result1 += pattern->getVerticalReflexionScore() * 100;
        result2 += pattern->getHorizontalSmudgeScore();
        result2 += pattern->getVerticalSmudgeScore() * 100;
    }

    end = clock();

    cout << "result1 : " << result1 << endl;
    cout << "result2 : " << result2 << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}