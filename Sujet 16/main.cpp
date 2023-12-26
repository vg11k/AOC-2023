#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <map>
using namespace std;

static const string filename = "input.txt";
//static const string filename = "example.txt";

struct Tile {
    int x;
    int y;
    string movement;
    string tag;
    Tile(int a, int b, string s) : x(a), y(b), movement(s) { tag = to_string(x) + "-" + to_string(y) + "-" + movement; }
};

int main(char* args[]) {

    cout << "Hello World !" << endl;

    bool debug = false;

    clock_t start, end;

    ifstream infile(filename.c_str());
    string line;
    long long result1 = 0;
    long long result2 = 0;
    vector<string> allLines;

    start = clock();

    while (getline(infile, line)) {
        allLines.push_back(line);
    }



    vector<vector<string>*> inputGrid;
    vector<vector<bool>*> outputGrid;
    for (int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        vector<string>* line = new vector<string>();
        vector<bool>* bline = new vector<bool>();
        for (int cindex = 0; cindex < allLines.at(lineIndex).size(); cindex++) {
            line->push_back(allLines.at(lineIndex).substr(cindex, 1));
            bline->push_back(false);
        }
        inputGrid.push_back(line);
        outputGrid.push_back(bline);
    }

    int nbLines = inputGrid.size() - 1;
    int nbCols = inputGrid.at(0)->size() - 1;

    vector<Tile> inputTiles;

    for (int i = 0; i < nbLines; i++) {
        inputTiles.push_back(Tile(-1, i, ">"));
        inputTiles.push_back(Tile(nbCols + 1, i, "<"));
    }

    for (int i = 0; i < nbCols; i++) {
        inputTiles.push_back(Tile(i, -1, "v"));
        inputTiles.push_back(Tile(i, nbLines + 1, "^"));
    }


    for (int inputTileIndex = 0; inputTileIndex < inputTiles.size(); inputTileIndex++) {

        vector<vector<bool>*> outputGrid;
        for (int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
            vector<bool>* bline = new vector<bool>();
            for (int cindex = 0; cindex < allLines.at(lineIndex).size(); cindex++) {
                bline->push_back(false);
            }
            outputGrid.push_back(bline);
        }

        long long localResult = 0;


        vector<Tile>* activeTiles = new vector<Tile>();
        //activeTiles->push_back(Tile(-1, 0, ">")); //part 1
        activeTiles->push_back(inputTiles.at(inputTileIndex));

        int loopLimit = 110 * 110 * 10;
        int cpt = 0;

        map<string, int> history;

        for (int cpt = 0; cpt < loopLimit && activeTiles->size() > 0; cpt++) {

            vector<Tile>* newTiles = new vector<Tile>();

            for (int tileidx = 0; tileidx < activeTiles->size(); tileidx++) {
                Tile t = activeTiles->at(tileidx);
                if (cpt > 0)
                    outputGrid.at(t.y)->at(t.x) = true;

                if (history.find(t.tag) == history.end()) {
                    history[t.tag] = 0;
                }
                history[t.tag]++;

                int tx = t.x;
                int ty = t.y;

                if (t.movement == "^" && t.y > 0) {
                    ty--;
                    string targetValue = inputGrid.at(ty)->at(tx);

                    if (targetValue == "." || targetValue == "|") newTiles->push_back(Tile(tx, ty, "^"));
                    else if (targetValue == "-") {
                        newTiles->push_back(Tile(tx, ty, ">"));
                        newTiles->push_back(Tile(tx, ty, "<"));
                    }
                    else if (targetValue == "/") newTiles->push_back(Tile(tx, ty, ">"));
                    else if (targetValue == "\\") newTiles->push_back(Tile(tx, ty, "<"));
                    else cout << "WTF" << endl;


                }
                else if (t.movement == "v" && t.y < nbLines) {
                    ty++;
                    string targetValue = inputGrid.at(ty)->at(tx);

                    if (targetValue == "." || targetValue == "|") newTiles->push_back(Tile(tx, ty, "v"));
                    else if (targetValue == "-") {
                        newTiles->push_back(Tile(tx, ty, ">"));
                        newTiles->push_back(Tile(tx, ty, "<"));
                    }
                    else if (targetValue == "/") newTiles->push_back(Tile(tx, ty, "<"));
                    else if (targetValue == "\\") newTiles->push_back(Tile(tx, ty, ">"));
                    else cout << "WTF" << endl;

                }
                else if (t.movement == "<" && t.x > 0) {
                    tx--;
                    string targetValue = inputGrid.at(ty)->at(tx);

                    if (targetValue == "." || targetValue == "-") newTiles->push_back(Tile(tx, ty, "<"));
                    else if (targetValue == "|") {
                        newTiles->push_back(Tile(tx, ty, "^"));
                        newTiles->push_back(Tile(tx, ty, "v"));
                    }
                    else if (targetValue == "/") newTiles->push_back(Tile(tx, ty, "v"));
                    else if (targetValue == "\\") newTiles->push_back(Tile(tx, ty, "^"));
                    else cout << "WTF" << endl;

                }
                else if (t.movement == ">" && t.x < nbCols) {
                    tx++;
                    string targetValue = inputGrid.at(ty)->at(tx);

                    if (targetValue == "." || targetValue == "-") newTiles->push_back(Tile(tx, ty, ">"));
                    else if (targetValue == "|") {
                        newTiles->push_back(Tile(tx, ty, "^"));
                        newTiles->push_back(Tile(tx, ty, "v"));
                    }
                    else if (targetValue == "/") newTiles->push_back(Tile(tx, ty, "^"));
                    else if (targetValue == "\\") newTiles->push_back(Tile(tx, ty, "v"));
                    else cout << "WTF" << endl;
                }
            }


            for (int tileCpt = 0; tileCpt < newTiles->size(); tileCpt++) {
                if (history.find(newTiles->at(tileCpt).tag) != history.end()) {
                    if (history[newTiles->at(tileCpt).tag] > 1) {
                        newTiles->erase(newTiles->begin() + tileCpt);
                        tileCpt--;
                    }
                }
            }


            delete activeTiles;
            activeTiles = newTiles;

            if (cpt == loopLimit - 1) cout << "completed on loop max iteration" << endl;
        }




        //display
        /*
        for (int lineIndex = 0; lineIndex < outputGrid.size(); lineIndex++) {
            for (int cindex = 0; cindex < outputGrid.at(lineIndex)->size(); cindex++) {
                if (inputGrid.at(lineIndex)->at(cindex) != ".") cout << inputGrid.at(lineIndex)->at(cindex);
                else if (outputGrid.at(lineIndex)->at(cindex)) cout << "#";
                else cout << ".";
            }
            cout << endl;
        }
        cout << endl << endl;//*/

        delete activeTiles;

        for (int lineIndex = 0; lineIndex < outputGrid.size(); lineIndex++) {
            for (int cindex = 0; cindex < outputGrid.at(lineIndex)->size(); cindex++) {
                if (outputGrid.at(lineIndex)->at(cindex))
                    localResult++;
            }
            delete outputGrid.at(lineIndex);
        }

        if (localResult > result2) result2 = localResult;
        if (inputTileIndex == 0) result1 = localResult;
    }


    end = clock();

    cout << "result1 : " << result1 << endl;
    cout << "result2 : " << result2 << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;

    return 0;
}