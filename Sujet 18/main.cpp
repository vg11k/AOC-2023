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
    int x;
    int y;
    string color;
    bool digged;
    bool flooded;

    Hole(bool b) : digged(b){color = "NONE"; flooded = false;}
};

struct DigInput {
    Direction d;
    int value;
    string color;
    DigInput(Direction a, int b, string c) : d(a), value(b), color(c){}
};


class HoleGrid {
    private:
    vector<vector<Hole*>*> grid;

    public:
    HoleGrid(vector<DigInput*> digs) {

        vector<Hole*> * startingLine = new vector<Hole*>();
        Hole * startingHole = new Hole(true);
        startingLine->push_back(startingHole);
        grid.push_back(startingLine);

        int positionX = 0;
        int positionY = 0;
        for(int digIndex = 0; digIndex < digs.size(); digIndex++) {
            DigInput * dig = digs.at(digIndex);
            int remainingHole = dig->value;
            for(int holeIndex = 0; holeIndex < remainingHole; holeIndex++) {
                Hole * hole;
                if(dig->d == Left) {
                    
                    if(positionX == 0) {
                        for(int lineIndex = 0; lineIndex < grid.size(); lineIndex++) {
                            vector<Hole*> * holeLine = grid.at(lineIndex);
                            holeLine->insert(holeLine->begin(), new Hole(false));
                        }
                    }
                    else positionX--;

                }
                else if(dig->d == Right) {
                    
                    if(positionX == grid.at(0)->size() - 1) {

                        for(int lineIndex = 0; lineIndex < grid.size(); lineIndex++) {
                            vector<Hole*> * holeLine = grid.at(lineIndex);
                            holeLine->push_back(new Hole(false));
                        }
                    }
                    positionX++;

                }
                else if(dig->d == Top) {

                    if(positionY == 0) {
                        vector<Hole*> * line = new vector<Hole*>();
                        for(int holeIndex = 0; holeIndex < grid.at(0)->size(); holeIndex++) {
                            line->push_back(new Hole(false));
                        }
                        grid.insert(grid.begin(), line);
                    }
                    else positionY--;
                }
                else if(dig->d == Bot) {

                    if(positionY == grid.size() - 1) {
                        vector<Hole*> * line = new vector<Hole*>();
                        for(int holeIndex = 0; holeIndex < grid.at(0)->size(); holeIndex++) {
                            line->push_back(new Hole(false));
                        }
                        grid.push_back(line);
                    }
                    positionY++;
                }

                hole = grid.at(positionY)->at(positionX);
                hole->digged = true;
                hole->color = dig->color;
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

        cout <<"flood complete"<< endl;

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
                if(h->flooded == true) { cout << "#" ; }
                else cout << ".";
            }
            cout << endl;
        }
        cout << endl << endl;
    }

    int howManyDig() {
        int result = 0;
        for(int i = 0; i < grid.size(); i++) {
            for(int j = 0; j < grid.at(i)->size(); j++) {
                Hole * h = grid.at(i)->at(j);
                if(h->digged == true) result ++;
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

    cout << "read ok" << endl;
    HoleGrid holeGrid(digs);
    cout << "init ok" << endl;

    holeGrid.floodNdig();
    //holeGrid.display();

    result = holeGrid.howManyDig();

    end = clock();

    std::cout << "result : " << result << endl;
    std::cout << end - start << " ms to compute." << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}