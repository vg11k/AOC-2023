#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

struct Node {
    Node(string s, long x, long y) : value(s), posX(x), posY(y) {}
    string value;
    long posX;
    long posY;
};


void displayMap(vector<vector<Node*>*>& nodeMap) {
    for (int y = 0; y < nodeMap.size(); y++) {
        for (int x = 0; x < nodeMap.at(y)->size(); x++) {
            Node* n = nodeMap.at(y)->at(x);
            cout << n->value;
        }
        cout << endl;
    }
}

void expandMap(vector<vector<Node*>*>& nodeMap, vector<Node*> &allNodes, vector<int> &linesSum, vector<int> &columnsSum) {

    for (int y = 0; y < nodeMap.size(); y++) {
        columnsSum.push_back(0);
    }

    for (int x = 0; x < nodeMap.at(0)->size(); x++) {
        linesSum.push_back(0);
    }

    for (int y = 0; y < nodeMap.size(); y++) {
        for (int x = 0; x < nodeMap.at(y)->size(); x++) {
            if (nodeMap.at(y)->at(x)->value != ".") {
                linesSum.at(y)++;
                columnsSum.at(x)++;
            }
        }
    }

    //naive solution to feed result 1 and grow map. Same as expansion 1

    //insert empty lines
   /* for (int lineIndex = linesSum.size() - 1; lineIndex > -1; lineIndex--) {
        if (linesSum.at(lineIndex) == 0) {
            vector<Node*>* emptyVector = new vector<Node*>();
            for (int i = 0; i < columnsSum.size(); i++)
            {
                Node* n = new Node(".", 0, 0);
                allNodes.push_back(n);
                emptyVector->push_back(n);
            }
            nodeMap.insert(nodeMap.begin() + lineIndex, emptyVector);
        }
    }

    //insert empty columns
    for (int y = 0; y < nodeMap.size(); y++) {
        for (int columnIndex = columnsSum.size() - 1; columnIndex > -1; columnIndex--) {
            if (columnsSum.at(columnIndex) == 0) {
                Node* n = new Node(".", 0, 0);
                allNodes.push_back(n);
                nodeMap.at(y)->insert(nodeMap.at(y)->begin() + columnIndex, n);
            }
        }
    }


    for (int y = 0; y < nodeMap.size(); y++) {
        for (int x = 0; x < nodeMap.at(y)->size(); x++) {
            nodeMap.at(y)->at(x)->posX = x;
            nodeMap.at(y)->at(x)->posY = y;
        }
    }*/

}

int main(char* args[]) {

    cout << "Hello World !" << endl;

    clock_t start, end;

    ifstream infile("input.txt");
    string line;
    long long result = 0;
    long long result2 = 0;
    vector<string> allLines;

    start = clock();

    while (getline(infile, line)) {
        allLines.push_back(line);
    }


    vector<Node*> allNodes;
    vector<Node*> galaxyNodes;

    int galaxyCounter = 0;

    vector<vector<Node*>*> originalMap;
    for (long lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        string currentLine = allLines.at(lineIndex);
        vector<Node*>* line = new vector<Node*>();
        originalMap.push_back(line);
        for (long characIndex = 0; characIndex < currentLine.length(); characIndex++) {
            Node* n = new Node(currentLine.substr(characIndex, 1), characIndex, lineIndex);
            allNodes.push_back(n);
            line->push_back(n);

            if (n->value == "#") {
                n->value = to_string(galaxyCounter);
                galaxyCounter++;
                galaxyNodes.push_back(n);
            }
        }
    }

    vector<int> linesSum;
    vector<int> columnsSum;
    
    expandMap(originalMap, allNodes, linesSum, columnsSum);

    int expansion = 1000000;

    vector<vector<int>*> distanceGraphMatrix;
    vector<vector<long>*> megadistanceGraphMatrix;

    for (int i = 0; i < galaxyNodes.size(); i++) {
        vector<int> * line = new vector<int>();
        vector<long>* lline = new vector<long>();
        distanceGraphMatrix.push_back(line);
        megadistanceGraphMatrix.push_back(lline);
        for (int j = 0; j < galaxyNodes.size(); j++) {
            Node* galaxyi = galaxyNodes.at(i);
            Node* galaxyj = galaxyNodes.at(j);
            int d = 0;
            long dl = 0;
            if (i != j) {
                d = abs(galaxyi->posX - galaxyj->posX) + abs(galaxyi->posY - galaxyj->posY);
                dl = d;
                for (int l = 0; l < linesSum.size(); l++) {
                    if (linesSum.at(l) == 0) {
                        if ((l > galaxyi->posY && l < galaxyj->posY) || (l < galaxyi->posY && l > galaxyj->posY)) {
                            dl += expansion-1;
                            d += 1;
                        }
                    }
                }
                for (int c = 0; c < columnsSum.size(); c++) {
                    if (columnsSum.at(c) == 0) {
                        if ((c > galaxyi->posX && c < galaxyj->posX) || (c < galaxyi->posX && c > galaxyj->posX)) {
                            dl += expansion-1;
                            d += 1;
                        }
                    }
                }
            }

            line->push_back(d);
            lline->push_back(dl);
        }
    }

    for (int i = 0; i < distanceGraphMatrix.size(); i++) {
        for (int j = 0; j < i; j++) {
            result += distanceGraphMatrix.at(i)->at(j);
            result2 += megadistanceGraphMatrix.at(i)->at(j);
        }
    }


    displayMap(originalMap);

    //cleaning stuffs
    for (int i = 0; i < allNodes.size(); i++)
        delete allNodes.at(i);

    end = clock();

    cout << "result part1 : " << result << endl;
    cout << "result part1 : " << result2 << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}