#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Node {

private:
    string value;
    long posX;
    long posY;
    long distance;
    bool nest;

public:
    Node(string v, long a, long b) : value(v), posX(a), posY(b) { distance = -1; nest = false; }
    Node(string v) : value(v){ distance = -1; nest = false; }
    Node() {}
    string getValue() { return value; }
    void setValue(string s) { value = s; }
    long getx() { return posX; }
    long gety() { return posY; }
    void setx(long x) { posX = x; }
    void sety(long y) { posY = y; }
    long getDistance() { return distance; }
    void setDistance(long d) { distance = d; }
    bool getNest() { return nest; }
    void setNest(bool b) { nest = b; }
};

Node* feedMap(vector<vector<Node*>*>& mapOfNodes, vector<string> allLines, vector<Node*>& allNodes) {
    Node* animalNode;
    for (long lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        vector<Node*>* lineOfNodes = new vector<Node*>();
        mapOfNodes.push_back(lineOfNodes);
        string currentLine = allLines.at(lineIndex);
        for (long characIndex = 0; characIndex < currentLine.length(); characIndex++) {
            Node* node = new Node(currentLine.substr(characIndex, 1), characIndex, lineIndex);
            allNodes.push_back(node);
            lineOfNodes->push_back(node);
            if (node->getValue() == "S") {
                animalNode = node;
            }
        }
    }
    return animalNode;
}


void feedMegamap(vector<vector<Node*>*>& megamap, vector<vector<Node*>*>& mapOfNodes, vector<Node*> &allNodes) {

    vector<Node*>* tampon = new vector<Node*>();
    Node* n;
    n = new Node(".");  tampon->push_back(n); allNodes.push_back(n);
    for (int i = 0; i < mapOfNodes.at(0)->size(); i++) {
        n = new Node("."); tampon->push_back(n); allNodes.push_back(n);
        n = new Node("."); tampon->push_back(n); allNodes.push_back(n);
    }
    megamap.push_back(tampon);
    for (int i = 0; i < mapOfNodes.size(); i++) {
        vector<Node*>* line = new vector<Node*>();
        n = new Node("."); line->push_back(n); allNodes.push_back(n);//tampon
        for (int j = 0; j < mapOfNodes.at(i)->size(); j++) {

            string val = mapOfNodes.at(i)->at(j)->getValue();
            if (mapOfNodes.at(i)->at(j)->getDistance() == -1) val = "."; //clear junks
            n = new Node(val); line->push_back(n); allNodes.push_back(n);
            if(val != ".")
                n->setDistance(mapOfNodes.at(i)->at(j)->getDistance());

            n = new Node("."); line->push_back(n); allNodes.push_back(n);
        }
        megamap.push_back(line);

        //tampon
        line = new vector<Node*>();
        n = new Node("."); line->push_back(n); allNodes.push_back(n);
        for (int j = 0; j < mapOfNodes.at(0)->size(); j++) {
            n = new Node("."); line->push_back(n); allNodes.push_back(n);
            n = new Node("."); line->push_back(n); allNodes.push_back(n);
        }
        megamap.push_back(line);
    }

    for (int i = 0; i < megamap.size(); i++) {
        vector<Node*>* line = megamap.at(i);
        for (int j = 0; j < line->size(); j++) {
            line->at(j)->setx(j);
            line->at(j)->sety(i);
        }
    }

    for (int i = 0; i < megamap.size(); i++) {
        for (int j = 0; j < megamap.at(i)->size(); j++) {

            bool oriental = true;
            bool vertical = true;

            string currentVal = megamap.at(i)->at(j)->getValue();
            if (currentVal == ".") {

                if (i > 0) {
                    string topVal = megamap.at(i - 1)->at(j)->getValue();
                    if (topVal != "|" && topVal != "7" && topVal != "F" && topVal != "S") vertical = false;
                }
                else vertical = false;

                if (i < megamap.size() - 1) {
                    string southVal = megamap.at(i + 1)->at(j)->getValue();
                    if (southVal != "|" && southVal != "L" && southVal != "J" && southVal != "S") vertical = false;
                }
                else vertical = false;


                if (j > 0) {
                    string leftVal = megamap.at(i)->at(j - 1)->getValue();
                    if (leftVal != "-" && leftVal != "L" && leftVal != "F" && leftVal != "S") oriental = false;
                }
                else oriental = false;

                if (j < megamap.at(i)->size() - 1) {
                    string rightVal = megamap.at(i)->at(j + 1)->getValue();
                    if (rightVal != "-" && rightVal != "J" && rightVal != "7" && rightVal != "S") oriental = false;
                }
                else vertical = false;

                if (vertical && oriental) cout << "WTF ?" << endl;
                else if (vertical) megamap.at(i)->at(j)->setValue("|");
                else if (oriental) megamap.at(i)->at(j)->setValue("-");

                if(vertical || oriental) megamap.at(i)->at(j)->setDistance(1);
            }
        }
    }

    for (int i = 0; i < megamap.size(); i++) {
        for (int j = 0; j < megamap.at(i)->size(); j++) {

        }
    }
}

long floodThePath(vector<vector<Node*>*> mapOfNodes, Node* animalNode) {

    long result = 0;
    vector<Node*> currentNodesToFlood;
    currentNodesToFlood.push_back(animalNode);

    while (currentNodesToFlood.size() > 0) {


        vector<Node*> nextFloodIteration;

        for (int i = 0; i < currentNodesToFlood.size(); i++) {

            Node* cNode = currentNodesToFlood.at(i);

            long ay = cNode->gety();
            long ax = cNode->getx();
            string val = cNode->getValue();


            if (ay > 0 && (val=="|" || val=="S" || val=="L" || val =="J")) {
                Node* topNode = mapOfNodes.at(ay - 1)->at(ax);
                if (topNode->getDistance() == -1) {
                    string topvalue = topNode->getValue();
                    if (topvalue == "|" || topvalue == "7" || topvalue == "F") {
                        topNode->setDistance(cNode->getDistance() + 1);
                        nextFloodIteration.push_back(topNode);
                    }
                }
            }

            if (ay < mapOfNodes.size() - 1 && (val == "|" || val == "S" || val == "7" || val == "F")) {
                Node* botNode = mapOfNodes.at(ay + 1)->at(ax);
                if (botNode->getDistance() == -1) {
                    string botValue = botNode->getValue();
                    if (botValue == "|" || botValue == "L" || botValue == "J") {
                        botNode->setDistance(cNode->getDistance() + 1);
                        nextFloodIteration.push_back(botNode);
                    }
                }
            }

            if (ax > 0 && (val=="-" || val=="J" || val=="7" || val=="S")) {
                Node* rightNode = mapOfNodes.at(ay)->at(ax - 1);
                if (rightNode->getDistance() == -1) {
                    string rightValue = rightNode->getValue();
                    if (rightValue == "-" || rightValue == "F" || rightValue == "L") {
                        rightNode->setDistance(cNode->getDistance() + 1);
                        nextFloodIteration.push_back(rightNode);
                    }
                }
            }

            if (ax < mapOfNodes.at(0)->size() - 1 && (val == "-" || val == "F" || val == "L" || val=="S")) {
                Node* leftNode = mapOfNodes.at(ay)->at(ax + 1);
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

        if (currentNodesToFlood.size() > 0) result++;
    }
    return result;
}

void floodMegamap(vector<vector<Node*>*> & megamap) {

    vector<Node*> currentNodesToFlood;

    
    for (int i = 0; i < megamap.at(0)->size(); i++) {
        currentNodesToFlood.push_back(megamap.at(0)->at(i));
        currentNodesToFlood.push_back(megamap.at(megamap.size() -1)->at(i));
    }


    for (int i = 1; i < megamap.size() - 1; i++) {
        currentNodesToFlood.push_back(megamap.at(i)->at(0));
        currentNodesToFlood.push_back(megamap.at(i)->at(megamap.at(i)->size() - 1));
    }


    for (int i = 0; i < currentNodesToFlood.size(); i++) {
        currentNodesToFlood.at(i)->setValue("X");
        currentNodesToFlood.at(i)->setDistance(1);
    }


    while(currentNodesToFlood.size() > 0) {

        vector<Node*> nextFloodIteration;

        for (int i = 0; i < currentNodesToFlood.size(); i++) {

            Node* cNode = currentNodesToFlood.at(i);

            long ay = cNode->gety();
            long ax = cNode->getx();

            if (ay > 0) {
                Node* topNode = megamap.at(ay - 1)->at(ax);
                if (topNode->getValue() == ".") {
                    topNode->setValue("X");
                    nextFloodIteration.push_back(topNode);
                }
            }

            if (ay < megamap.size() - 1) {
                Node* botNode = megamap.at(ay + 1)->at(ax);
                if (botNode->getValue() == ".") {
                    botNode->setValue("X");
                    nextFloodIteration.push_back(botNode);
                }
            }


            if (ax < megamap.at(0)->size() - 1) {
                Node* rightNode = megamap.at(ay)->at(ax + 1);
                if (rightNode->getValue() == ".") {
                    rightNode->setValue("X");
                    nextFloodIteration.push_back(rightNode);
                }
            }


            if (ax > 0) {
                Node* leftNode = megamap.at(ay)->at(ax - 1);
                if (leftNode->getValue() == ".") {
                    leftNode->setValue("X");
                    nextFloodIteration.push_back(leftNode);
                }
            }
        }
       
        currentNodesToFlood.clear();
        for (int i = 0; i < nextFloodIteration.size(); i++) {
            currentNodesToFlood.push_back(nextFloodIteration.at(i));
            nextFloodIteration.at(i)->setDistance(1);
        }
    }
}

void nestMapFromMegamap(vector<vector<Node*>*> &megamap, vector<vector<Node*>*> &mapOfNodes) {

    for (int i=0; i < mapOfNodes.size(); i++) {
        for (int j = 0; j < mapOfNodes.at(0)->size(); j++) {
            Node* node = mapOfNodes.at(i)->at(j);
            Node* megamapNode = megamap.at(i * 2 + 1)->at(j * 2 + 1);
            if (megamapNode->getValue() == ".") {
                node->setNest(true);
            }
        }
    }
}

void displayMap(vector<vector<Node*>*>& nodeMap) {
    for (int y = 0; y < nodeMap.size(); y++) {
        for (int x = 0; x < nodeMap.at(y)->size(); x++) {
            Node* n = nodeMap.at(y)->at(x);
            if (n->getDistance() != -1) cout << n->getValue();
            else if (n->getNest()) cout << "N";
            else cout << ".";
        }
        cout << endl;
    }
}

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


    vector<Node*> allNodes;
    vector<vector<Node*>*> mapOfNodes;
    Node* animalNode = feedMap(mapOfNodes, allLines, allNodes);

    animalNode->setDistance(0);
    result = floodThePath(mapOfNodes, animalNode);

    //part2
    
    long nbNest = 0;
    vector<vector<Node*>*> megamap;

    feedMegamap(megamap, mapOfNodes, allNodes);
    floodMegamap(megamap);
    nestMapFromMegamap(megamap, mapOfNodes);


    for (int y = 0; y < mapOfNodes.size(); y++) {
        for (int x = 0; x < mapOfNodes.at(y)->size(); x++) {
            Node* n = mapOfNodes.at(y)->at(x);
            if (n->getNest()) nbNest++;
        }
    }


    //displayMap(megamap);
    displayMap(mapOfNodes);


    //cleaning stuffs
    for (int i = 0; i < allNodes.size(); i++)
        delete allNodes.at(i);

    for (int i = 0; i < mapOfNodes.size(); i++) 
        delete mapOfNodes.at(i);

    for (int i = 0; i < megamap.size(); i++)
        delete megamap.at(i);


    end = clock();

    cout << "result part1 : " << result << endl;
    cout << "result part2 : " << nbNest << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}