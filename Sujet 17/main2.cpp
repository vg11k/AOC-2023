#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

//1296 trop haut, 20/420/1500 en 33mn
//1296 ,20/350/1350 en 29mn
//1292, 25/340/1340 en 3h
//1288, 30/360/1340 en 21h

static const int maxErrorFactor = 35;
static const int maxProfondeur = 380;
static const long long cap = 1320;
static const int scoreDistanceFactor = 25;

static const string filename = "input.txt";
//static const string filename = "example.txt";

static const int minimumBlocks = 3;
static const int maximumBlocks = 10;

static long long bestDistanceGlobal = LONG_MAX;

struct Tile {
    int x;
    int y;
    int id;
    long long loss;
    Tile(int a, int b, long c) : x(a), y(b), loss(c) {id = x + 10000 * y;}
};

enum Direction { Top, Bot, Left, Right, None};

class Path {
    private :
        Tile * from;
        Tile * to;
        vector<Tile *> path;
        long long heatLoss;
        bool dynamic;

    public:
    Path(Tile * a, Tile * b): from(a), to(b) {heatLoss = LONG_MAX; path = vector<Tile*>(); dynamic = false;}
    long long getHeatLoss(){return heatLoss;}
    void setDynamic(bool b){dynamic = b;}
    void displaySumForDeep(int deep) {
        long long heatsum = 0;
        for(int i = 0; i < deep && i < path.size(); i++) {
            heatsum += path.at(i)->loss;
        }
        cout << "heat at deep " << deep << " equals " << heatsum << endl;
    }

    Tile * getTileAt(int i) {return path.at(i);}

    void recopyPathFrom(Path * otherPath, map<int, vector<Path*>*> * tileIdToPaths) {
        heatLoss = 0;

        path.clear();
        for(int i = 0; i < otherPath->path.size(); i++) {
            Tile * t = otherPath->path.at(i);
            addTile(t);
        }
    }

    void addTile(Tile * t) {
        path.push_back(t);
        if(heatLoss == LONG_MAX) heatLoss = 0;
        heatLoss += t->loss;
    }

    void stepBack() {
        Tile * t = path.at(path.size() - 1);
        heatLoss -= t->loss;
        path.pop_back();
    }

    bool containsTile(Tile * t) {
        for(int ti = 0; ti < path.size(); ti++) {
            if(t->id == path.at(ti)->id) return true;
        }
        return false;
    }

    void displaySelf(vector<vector<Tile*>*> * inputGrid) {
        for(int i = 0; i < inputGrid->size(); i++) {
            for(int j = 0; j < inputGrid->at(i)->size(); j++) {
                Tile * t = inputGrid->at(i)->at(j);
                bool contains = false;
                for(int ti = 0; ti < path.size() && !contains; ti++) {
                    if(path.at(ti)->id == t->id) contains = true;
                }

                if(contains) std::cout << "#";
                else std::cout << t->loss;
            }
            std::cout << endl;
        }
    }
};

void recurs(map<int, Path*> * tileIdToBestPath, vector<vector<Tile*>*> * inputGrid, int finalId, Tile * currentTile, Direction lastdirection, int lastdirectionCount, Path * dynamicPath, map<int, vector<Path*>*> * tileIdToPaths, int profondeurDegradation, long long profondeur, vector<vector<bool>*> * trackerGrid) {

    if(trackerGrid->at(currentTile->y)->at(currentTile->x)) return;
    
    dynamicPath->addTile(currentTile);
    trackerGrid->at(currentTile->y)->at(currentTile->x) = true;


    bool bestThanBefore = false;
    Path* bestPathToHere = (*tileIdToBestPath)[currentTile->id];

    int localDegradation = profondeurDegradation;

    bool prune = false;

    long long bestDistanceFinale = bestDistanceGlobal;//(*tileIdToBestPath)[finalId]->getHeatLoss();
    //if(LONG_MAX != bestDistanceFinale) {
        long long min = cap;
        if(bestDistanceFinale < min) min = bestDistanceFinale;
        if(dynamicPath->getHeatLoss() > min
            /*(profondeur < 100 && dynamicPath->getHeatLoss() > 400) ||
            (profondeur < 150 && dynamicPath->getHeatLoss() > 650) ||
            (profondeur < 200 && dynamicPath->getHeatLoss() > 900)/*profondeur > maxProfondeur / 2 && dynamicPath->getHeatLoss() > bestDistanceFinale * 0.75)*/) {
           //  (profondeur > maxProfondeur / 4 && dynamicPath->getHeatLoss() > bestDistanceFinale * 0.8)) {
            prune = true;
        }
    //}

    if(profondeur > scoreDistanceFactor) {
        int currentDistanceScore = currentTile->x + currentTile->y;
        int distanceScoreLess20 = dynamicPath->getTileAt(profondeur - scoreDistanceFactor)->x + dynamicPath->getTileAt(profondeur - scoreDistanceFactor)->y;
        if(distanceScoreLess20 > currentDistanceScore) prune = true;
    }

    int localMaxErrorFactor = maxErrorFactor;

    if(!prune) {
        if(dynamicPath->getHeatLoss() < bestPathToHere->getHeatLoss()) {

            bestPathToHere->recopyPathFrom(dynamicPath, tileIdToPaths);
            bestThanBefore = true;
            localDegradation = localDegradation--;
            if(localDegradation < 0) localDegradation = 0;



            vector<Path*> * pathsToUpdate = (*tileIdToPaths)[currentTile->id];

            pathsToUpdate->push_back(bestPathToHere);

            if(finalId == currentTile->id) {
                std::cout << "new best score : " << bestPathToHere->getHeatLoss() << " at deepness " << profondeur << endl;
                /*dynamicPath->displaySumForDeep(250);
                dynamicPath->displaySumForDeep(200);
                dynamicPath->displaySumForDeep(150);
                dynamicPath->displaySumForDeep(100);*/
                bestDistanceGlobal = bestPathToHere->getHeatLoss();
            } 
        }
        else {
            if(profondeur < maxProfondeur / 4) localMaxErrorFactor *= 1.2;
            if(currentTile->x < 70 && currentTile->y < 70) localMaxErrorFactor *= 1.2;
            if(currentTile->x < 105 && currentTile->y < 105 && currentTile->x > 35 && currentTile->y > 35) localMaxErrorFactor *= 1.2;
            if(currentTile->x > 70 && currentTile->y > 70) localMaxErrorFactor *= 1.2;
            localDegradation++;
        }
    }

    



    if(currentTile->id != finalId && 
        (bestThanBefore || localDegradation < localMaxErrorFactor) && 
        profondeur < maxProfondeur &&
        !prune) {

        Tile * nextTile;

        //right
        if(currentTile->x < inputGrid->at(0)->size() -1) {
            if(None == lastdirection || 
                (Left != lastdirection && 
                    ((Right != lastdirection && lastdirectionCount > minimumBlocks) || 
                    (Right == lastdirection && lastdirectionCount < maximumBlocks)))) {

                nextTile = inputGrid->at(currentTile->y)->at(currentTile->x+1);

                int newDirectionCount = 1;
                if(Right == lastdirection) newDirectionCount+= lastdirectionCount;

                recurs(tileIdToBestPath, inputGrid, finalId, nextTile, Right, newDirectionCount, dynamicPath, tileIdToPaths, localDegradation, profondeur + 1, trackerGrid);
            }
        }


        //bot
        if(currentTile->y < inputGrid->size() - 1) {
            if(None == lastdirection || 
                (Top != lastdirection && 
                    ((Bot != lastdirection && lastdirectionCount > minimumBlocks) || 
                    (Bot == lastdirection && lastdirectionCount < maximumBlocks)))) {

                nextTile = inputGrid->at(currentTile->y+1)->at(currentTile->x);
                int newDirectionCount = 1;
                if(Bot == lastdirection) newDirectionCount+= lastdirectionCount;

                recurs(tileIdToBestPath, inputGrid, finalId, nextTile, Bot, newDirectionCount, dynamicPath, tileIdToPaths, localDegradation, profondeur + 1, trackerGrid);
            }
        }

        //left
        if(currentTile->x > 0) {
            if(None == lastdirection || 
                (Right != lastdirection && 
                    ((Left != lastdirection && lastdirectionCount > minimumBlocks) || 
                    (Left == lastdirection && lastdirectionCount < maximumBlocks)))) {

                nextTile = inputGrid->at(currentTile->y)->at(currentTile->x-1);
                int newDirectionCount = 1;
                if(Left == lastdirection) newDirectionCount+= lastdirectionCount;

                recurs(tileIdToBestPath, inputGrid, finalId, nextTile, Left, newDirectionCount, dynamicPath, tileIdToPaths, localDegradation, profondeur + 1, trackerGrid);
            }
        }

                
        //top
        if(currentTile->y > 0) {
            if(None == lastdirection || 
                (Bot != lastdirection && 
                    ((Top != lastdirection && lastdirectionCount > minimumBlocks) || 
                    (Top == lastdirection && lastdirectionCount < maximumBlocks)))) {

                nextTile = inputGrid->at(currentTile->y-1)->at(currentTile->x);
                if(!dynamicPath->containsTile(nextTile)) {
                    int newDirectionCount = 1;
                    if(Top == lastdirection) newDirectionCount+= lastdirectionCount;

                    recurs(tileIdToBestPath, inputGrid, finalId, nextTile, Top, newDirectionCount, dynamicPath, tileIdToPaths, localDegradation, profondeur + 1, trackerGrid);
                }
            }
        }
    }
    dynamicPath->stepBack();
    trackerGrid->at(currentTile->y)->at(currentTile->x) = false;
}

int main(char * args[]) {

    std::cout<< "Hello World !" << endl;
    std::cout << "max error factor : " << maxErrorFactor << " & max deepness : " << maxProfondeur << " & cap = " << cap <<  endl;

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

    vector<vector<bool>*> * trackerGrid = new vector<vector<bool>*>();

    vector<vector<Tile*>*> * inputGrid = new vector<vector<Tile*>*>();
    vector<Tile*> allTiles;
    for(int i = 0; i < allLines.size(); i++) {
        vector<Tile*> * line = new vector<Tile*>();
        vector<bool> * trackerLine = new vector<bool>();
        for(int j = 0; j < allLines.at(i).length(); j++) {
            Tile * t = new Tile(j, i, stoll(allLines.at(i).substr(j,1)));
            line->push_back(t);
            allTiles.push_back(t);
            trackerLine->push_back(false);
        }
        inputGrid->push_back(line);
        trackerGrid->push_back(trackerLine);
    }

    map<int, vector<Path*>*> * tileIdToPaths = new map<int, vector<Path*>*>();
    for(int tileIndex = 0; tileIndex < allTiles.size(); tileIndex++) {
        (*tileIdToPaths)[allTiles.at(tileIndex)->id] = new vector<Path*>();
    }

    map<int, Path*> * targetTileIdToPath = new map<int, Path*>();
    for(int tileIndex = 0; tileIndex < allTiles.size(); tileIndex++) {
        (*targetTileIdToPath)[allTiles.at(tileIndex)->id] = new Path(allTiles.at(0), allTiles.at(tileIndex));
    }



    Tile * startingTile = inputGrid->at(0)->at(0);
    Tile * endingTile = inputGrid->back()->back();
    Path * dynamicPath = new Path(startingTile, endingTile);
    dynamicPath->setDynamic(true);

    std::cout << "start recursive" << endl;

    recurs(targetTileIdToPath, inputGrid, endingTile->id, startingTile, None, 1, dynamicPath, tileIdToPaths, 0, 0, trackerGrid);

    std::cout << "end recursive" << endl;

    Path * resultPath = (*targetTileIdToPath)[endingTile->id];

    resultPath->displaySelf(inputGrid);

    for(int i = 0; i < inputGrid->size(); i++) {
        for(int j = 0; j < inputGrid->at(i)->size(); j++) {
            Tile * t = inputGrid->at(i)->at(j);
            std::cout << (*targetTileIdToPath)[t->id]->getHeatLoss() << " ";
        }
        std::cout << endl;
    }

    result = resultPath->getHeatLoss();
    result-= startingTile->loss;

    end = clock();

    std::cout << "result : " << result << endl;
    std::cout << end - start << " ms to compute." << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}