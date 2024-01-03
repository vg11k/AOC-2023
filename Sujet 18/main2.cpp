#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;



static const string filename = "input.txt";
//static const string filename = "example.txt";


enum Direction { Top, Bot, Left, Right};

struct Hole {
    long long x;
    long long y;
    long long realx;
    long long realy;
    long long sx;
    long long sy;
    string color;
    bool digged;
    bool flooded;
    int inputIndex;

    Hole(bool b) : digged(b){color = "NONE"; flooded = false;}
    Hole() {digged = false; flooded = false; inputIndex = -1;}
};

struct DigInput {
    Direction d;
    long value;
    string color;
    DigInput(Direction a, int b, string c) : d(a), value(b), color(c){}
};


class HoleGrid {
    private:
    vector<vector<Hole*>*> grid;

    public:
    HoleGrid(vector<DigInput*> digs) {

        vector<Hole*> * startingLine = new vector<Hole*>();

        long positionX = 0;
        long positionY = 0;

        long up = 0;
        long down = 0;
        long left = 0;
        long right = 0;

        //lets find the real grid size ans our position in it

        for(int digIndex = 0; digIndex < digs.size(); digIndex++) {
            DigInput * dig = digs.at(digIndex);

            if(dig->d == Left) {
                positionX-= dig->value;
            }
            else if(dig->d == Right) {
                positionX+= dig->value;
            }
            else if(dig->d == Top) {
                positionY-= dig->value;
            }
            else if(dig->d == Bot) {
                positionY+= dig->value;
            }

            if(positionY < up) up = positionY;
            if(positionY > down) down = positionY;
            if(positionX < left) left = positionX;
            if(positionX > right) right = positionX;
        }

        long long startingX = left * -1;
        long long startingY = up * -1;
        long long sizeX = startingX + right;
        long long sizeY = startingY + down;

        //with the true size & true starting position known, lets put every input dig at its true coordinates

        vector<long long> digRealCoordsX;
        vector<long long> digRealCoordsY;

        positionX = startingX;
        positionY = startingY;

        for(int digIndex = 0; digIndex < digs.size(); digIndex++) {
            DigInput * dig = digs.at(digIndex);

            if(dig->d == Left) {
                positionX-= dig->value;
            }
            else if(dig->d == Right) {
                positionX+= dig->value;
            }
            else if(dig->d == Top) {
                positionY-= dig->value;
            }
            else if(dig->d == Bot) {
                positionY+= dig->value;
            }

            digRealCoordsX.push_back(positionX);
            digRealCoordsY.push_back(positionY);
        }

        //the real grid is more than 3000.000.000.000.000
        //it can't fit in memory or acceptable computing time
        //so lets build a lighter grid with weights instead

        //each rows and columns are fused in local pools
        //but each line and columns with at least one hole digged in input are not fused
        // (the insert ones)

        vector<long long> extendedDigX = sortAndRemoveDoublonsAndInsertOnes(digRealCoordsX);
        vector<long long> extendedDigY = sortAndRemoveDoublonsAndInsertOnes(digRealCoordsY);

        vector<Hole*> originalHoles;

        //setup a lighter grid with each hole with a weight xsize * ysize instead of 1*1

        for(int i = 0; i < extendedDigY.size(); i++) {
            vector<Hole*>* lineHole = new vector<Hole*>();
            for(int j = 0; j < extendedDigX.size(); j++) {
                Hole * hole = new Hole();

                long long coordY = extendedDigY.at(i);
                long long coordX = extendedDigX.at(j);

                hole->realy = coordY;
                hole->realx = coordX;
                hole->x = j;
                hole->y = i;

                if(i == extendedDigY.size() - 1) hole->sy = sizeY - coordY + 1;
                else hole->sy = extendedDigY.at(i + 1) - coordY;
                if(j == extendedDigX.size() - 1) hole->sx = sizeX - coordX + 1;
                else hole->sx = extendedDigX.at(j + 1) - coordX;

                int index = -1;
                for(int k = 0; k < digRealCoordsX.size() && index < 0; k++) {
                    if(digRealCoordsX.at(k) == coordX && digRealCoordsY.at(k) == coordY) index = k;
                }

                if(index > -1) {
                    hole->digged = true;
                    hole->inputIndex = index + 1;
                    if(hole->inputIndex == digs.size()) hole->inputIndex = 0;
                    originalHoles.push_back(hole);
                }
                lineHole->push_back(hole);
            }
            grid.push_back(lineHole);
        }


        //draw holes lines on the lighter grid

        for(int holeIndex = 0; holeIndex < originalHoles.size(); holeIndex++) {
            Hole * hole = originalHoles.at(holeIndex);
            DigInput * input = digs.at(hole->inputIndex);

            if(input->d == Bot) {
                long long stillToDig = input->value;
                if(hole->y + 1 < grid.size()) {
                    Hole * nextHole = grid.at(hole->y + 1)->at(hole->x);
                    while(stillToDig > 0) {
                        stillToDig -= nextHole->sy;
                        nextHole->digged = true;
                        if(nextHole->y + 1 < grid.size())
                            nextHole = grid.at(nextHole->y + 1)->at(nextHole->x);
                    }
                }
            }
            else if(input->d == Top) {
                long long stillToDig = input->value;

                if(hole->y - 1 > 0) {
                    Hole * nextHole = grid.at(hole->y - 1)->at(hole->x);
                    while(stillToDig > 0) {
                        stillToDig -= nextHole->sy;
                        nextHole->digged = true;
                        if(nextHole->y - 1 > 0)
                            nextHole = grid.at(nextHole->y - 1)->at(nextHole->x);
                    }
                }
            }
            else if(input->d == Right) {
                long long stillToDig = input->value;
                if(hole->x+1 < grid.at(hole->y)->size()) {
                    Hole * nextHole = grid.at(hole->y)->at(hole->x+1);
                    while(stillToDig > 0) {
                        stillToDig -= nextHole->sx;
                        nextHole->digged = true;

                        if(nextHole->x+1 < grid.at(0)->size())
                            nextHole = grid.at(nextHole->y)->at(nextHole->x+1);
                    }
                }
            }
            else if(input->d == Left) {
                long long stillToDig = input->value;
                if(hole->x - 1 > 0) {
                    Hole * nextHole = grid.at(hole->y)->at(hole->x-1);
                    while(stillToDig > 0) {
                        stillToDig -= nextHole->sx;
                        nextHole->digged = true;
                        if(nextHole->x-1 > 0)
                            nextHole = grid.at(nextHole->y)->at(nextHole->x-1);
                    }
                }
            }
        }

        for(int i = 0; i < grid.size(); i++) {
            for(int j = 0; j < grid.at(i)->size(); j++) {
                Hole * hole = grid.at(i)->at(j);
                hole->x = j;
                hole->y = i;
            }
        }
    }

    vector<long long> sortAndRemoveDoublonsAndInsertOnes(vector<long long> inputVec) {
        vector<long long> sortNNoDoublVec;

        sortNNoDoublVec.push_back(inputVec.at(0));
        for(int i = 1; i < inputVec.size(); i++) {
            int position = -1;
            bool doublon = false;
            for(int j = 0; j < sortNNoDoublVec.size() && position == -1 && !doublon; j++) {
                if(inputVec.at(i) == sortNNoDoublVec.at(j)) doublon = true;
                if(inputVec.at(i) < sortNNoDoublVec.at(j)) position = j;
            }

            if(!doublon) {
                if(position < 0) sortNNoDoublVec.push_back(inputVec.at(i));
                else sortNNoDoublVec.insert(sortNNoDoublVec.begin() + position, inputVec.at(i));
            }
        }

        vector<long long> result;

        for(int i = 0; i < sortNNoDoublVec.size() - 1; i++) {
            long long a = sortNNoDoublVec.at(i);
            long long b = sortNNoDoublVec.at(i + 1);
            result.push_back(a);
            if(a+1 != b) result.push_back(a+1);
        }
        result.push_back(sortNNoDoublVec.back());

        return result;
    }

    void floodNdig() {

        //setup another grid, extended, to flood from outside

        vector<vector<Hole*>*> exGrid;
        vector<Hole*> * floodHoles = new vector<Hole*>();
        vector<Hole*> delHoles;

        for(int i = 0; i < grid.size(); i++) {
            vector<Hole*>* exLine = new vector<Hole*>();

            Hole * headHole = new Hole(false);
            exLine->push_back(headHole);

            for(int j = 0; j < grid.at(i)->size(); j++) {
                exLine->push_back(grid.at(i)->at(j));
            }

            Hole * queueHole = new Hole(false);
            exLine->push_back(queueHole);

            delHoles.push_back(headHole);
            delHoles.push_back(queueHole);
            exGrid.push_back(exLine);
        }

        vector<Hole*> * headLine = new vector<Hole*>();
        vector<Hole*> * queueLine = new vector<Hole*>();
        int queueIndex = exGrid.size() + 1;

        for(int i = 0; i < exGrid.at(0)->size(); i++){
            Hole* head = new Hole(false);
            Hole* queue = new Hole(false);
            delHoles.push_back(head);
            delHoles.push_back(queue);
            headLine->push_back(head);
            queueLine->push_back(queue);
        }

        exGrid.insert(exGrid.begin(), headLine);
        exGrid.push_back(queueLine);

        for(int i = 0; i < delHoles.size(); i++) {
            floodHoles->push_back(delHoles.at(i));
            floodHoles->at(i)->flooded = true;
        }

        for(int i = 0; i < exGrid.size(); i++) {
            for(int j = 0; j < exGrid.at(i)->size(); j++) {
                Hole * h = exGrid.at(i)->at(j);
                h->x = j;
                h->y = i;
            }
        }

        //flood from outside

        while(!floodHoles->empty()) {

            vector<Hole*> * nextFlood = new vector<Hole*>();

            for(int i = 0; i < floodHoles->size(); i++) {

                Hole * floodingHole = floodHoles->at(i);
                int x = floodingHole->x;
                int y = floodingHole->y;

                if(x > 1) {
                    Hole * leftHole = exGrid.at(y)->at(x-1);
                    if(!leftHole->flooded && !leftHole->digged) {
                        leftHole->flooded = true;
                        nextFlood->push_back(leftHole);
                    }
                }

                if(x < exGrid.at(1)->size() - 1) {
                    Hole * rightHole = exGrid.at(y)->at(x + 1);
                    if(!rightHole->flooded && !rightHole->digged) {
                        rightHole->flooded = true;
                        nextFlood->push_back(rightHole);
                    }
                }

                if(y > 0) {
                    Hole * topHole = exGrid.at(y-1)->at(x);
                    if(!topHole->flooded && !topHole->digged) {
                        topHole->flooded = true;
                        nextFlood->push_back(topHole);
                    }
                }

               if(y < exGrid.size()-1) {
                    Hole * botHole = exGrid.at(y+1)->at(x);
                    if(!botHole->flooded && !botHole->digged) {
                        botHole->flooded = true;
                        nextFlood->push_back(botHole);
                    }
                }
                            
            }

            delete floodHoles;
            floodHoles = nextFlood;
        }
        delete floodHoles;

        //dig everything not flooded
        for(int i = 0; i < grid.size(); i++) {
            for(int j = 0; j < grid.at(i)->size(); j++) {
                Hole * hole = grid.at(i)->at(j);
                hole->y = i;
                hole->x = j;
                if(!hole->flooded) hole->digged = true;
            }
        }

        //cleaning
        for(int i = 0; i < exGrid.size(); i++) {
            delete exGrid.at(i);
        }
        for(int i = 0; i < delHoles.size(); i++)
            delete delHoles.at(i);
    }

    void display() {
        for(int i = 0; i < grid.size(); i++) {
            for(int j = 0; j < grid.at(i)->size(); j++) {
                Hole * h = grid.at(i)->at(j);
                if(h->digged == true) { cout << "#" ; }
                else cout << ".";
            }
            cout << endl;
        }
        cout << endl << endl;
    }

    long long howManyDig() {
        long long result = 0;
        for(int i = 0; i < grid.size(); i++) {
            for(int j = 0; j < grid.at(i)->size(); j++) {
                Hole * h = grid.at(i)->at(j);
                if(h->digged == true) result += h->sx * h->sy;
            }
        }
        return result;
    }
};



int main(char * args[]) {

    std::cout<< "Hello World !" << endl;

    bool debug = false;

    clock_t start, end;

    ifstream infile(filename);
    string line;
    long long result = 0;
    vector<string> allLines;

    start = clock();

    while(getline(infile, line)) {
        allLines.push_back(line);
    }

    vector<DigInput*> digs;

    for(int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        string line = allLines.at(lineIndex);

        istringstream iss(line);
        string s;
        getline(iss, s, ' ');
        Direction d;
        if(s == "R") d = Right;
        else if (s == "D") d = Bot;
        else if (s == "U") d = Top;
        else if (s == "L") d = Left;
        else cout << "input wtf : " << s << endl;

        getline(iss, s, ' ');
        int i = stoi(s);
        getline(iss, s, ' ');
        string color = s.substr(1,7);
        digs.push_back(new DigInput(d, i, color));
    }

    for(int i = 0; i < digs.size(); i++) {
        string hexa = digs.at(i)->color.substr(1,5);
        string dir = digs.at(i)->color.substr(6);
        Direction d;
        if(dir == "0") d = Right;
        else if (dir == "1") d = Bot;
        else if (dir == "2") d = Left;
        else if (dir == "3") d = Top;

        std::stringstream stream;
        stream << hexa;
        long out = 0;
        stream >> std::hex >> out;

        digs.at(i)->d = d;
        digs.at(i)->value = out;
    }

    HoleGrid holeGrid(digs);
    holeGrid.floodNdig();
    //holeGrid.display();

    result = holeGrid.howManyDig();

    end = clock();

    std::cout << "result : " << result << endl;
    std::cout << end - start << " ms to compute." << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}