#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Node {

private :
    string value;
    long posX;
    long posY;
    long distance;

public:
    Node(string v, long a, long b) : value(v), posX(a), posY(b) { distance = -1; }
    Node() {}
    string getValue() { return value; }
    long getx() { return posX; }
    long gety() { return posY; }
    long getDistance() { return distance; }
    void setDistance(long d) { distance = d; }
};


int main(char* args[]) {

    cout << "Hello World !" << endl;

    clock_t start, end;

    ifstream infile("input.txt");
    string line;
    long long result = 0;
    vector<string> allLines;

    start = clock();

    while (getline(infile, line)) {
        allLines.push_back(line);
    }

    string animal = "S";
    Node * animalNode;

    vector<Node*> allNodes;

    vector<vector<Node*>*> mapOfNodes;
    for (long lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        vector<Node*> * lineOfNodes = new vector<Node*>();
        mapOfNodes.push_back(lineOfNodes);
        string currentLine = allLines.at(lineIndex);
        for (long characIndex = 0; characIndex < currentLine.length(); characIndex++) {
            Node * node = new Node(currentLine.substr(characIndex, 1), characIndex, lineIndex);
            lineOfNodes->push_back(node);
            if (node->getValue() == "S") {
                animalNode = node;
            }
        }
    }

    animalNode->setDistance(0);
    vector<Node*> pathTaken;
    pathTaken.push_back(animalNode);


    bool stop = false;

    vector<Node*> currentNodesToFlood;
    currentNodesToFlood.push_back(animalNode);

    while (currentNodesToFlood.size() > 0) {


        vector<Node*> nextFloodIteration;

        for (int i = 0; i < currentNodesToFlood.size(); i++) {

            Node * cNode = currentNodesToFlood.at(i);

            long ay = cNode->gety();
            long ax = cNode->getx();


            if (ay > 0) {
                Node * topNode = mapOfNodes.at(ay - 1)->at(ax);
                if (topNode->getDistance() == -1) {
                    string topvalue = topNode->getValue();
                    if (topvalue == "|" || topvalue == "7" || topvalue == "F") {
                        topNode->setDistance(cNode->getDistance() + 1);
                        nextFloodIteration.push_back(topNode);
                    }
                }
            }

            if (ay < mapOfNodes.size() - 1) {
                Node * botNode = mapOfNodes.at(ay + 1)->at(ax);
                if (botNode->getDistance() == -1) {
                    string botValue = botNode->getValue();
                    if (botValue == "|" || botValue == "L" || botValue == "J") {
                        botNode->setDistance(cNode->getDistance() + 1);
                        nextFloodIteration.push_back(botNode);
                    }
                }
            }

            if (ax > 0) {
                Node * rightNode = mapOfNodes.at(ay)->at(ax - 1);
                if (rightNode->getDistance() == -1) {
                    string rightValue = rightNode->getValue();
                    if (rightValue == "-" || rightValue == "F" || rightValue == "L") {
                        rightNode->setDistance(cNode->getDistance() + 1);
                        nextFloodIteration.push_back(rightNode);
                    }
                }
            }

            if (ax < mapOfNodes.at(0)->size() - 1) {
                Node * leftNode = mapOfNodes.at(ay)->at(ax + 1);
                if (leftNode->getDistance() == -1) {
                    string leftValue = leftNode->getValue();
                    if (leftValue == "-" || leftValue == "7" || leftValue == "J") {
                        leftNode->setDistance(cNode->getDistance() + 1);
                        nextFloodIteration.push_back(leftNode);
                    }
                }
            }

        }

        currentNodesToFlood.clear();
        for (int i = 0; i < nextFloodIteration.size(); i++)
            currentNodesToFlood.push_back(nextFloodIteration.at(i));

        if(currentNodesToFlood.size() > 0) result++;
    }

    bool displayTheMap = false;
    if (displayTheMap) {
        for (int y = 0; y < mapOfNodes.size(); y++) {
            for (int x = 0; x < mapOfNodes.at(y)->size(); x++) {
                Node* n = mapOfNodes.at(y)->at(x);
                if (n->getDistance() == -1) cout << ".";
                else cout << n->getDistance();
            }
            cout << endl;
        }
    }

    for (int i = 0; i < allNodes.size(); i++) {
        delete allNodes.at(i);
    }

    for (int i = 0; i < mapOfNodes.size(); i++) {
        delete mapOfNodes.at(i);
    }

    end = clock();

    cout << "result part : " << result << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}