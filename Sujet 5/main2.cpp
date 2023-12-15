#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Triplet{
    long long destinationId;
    long long sourceId;
    long long sourceRange;
};

struct Doublet {
    long long seedStart;
    long long seedRange;

    Doublet(){}
    Doublet(long long a, long long b) : seedStart(a), seedRange(b) {}
};

class Almanach {

    public:
        Almanach(vector<string> lines) {

            vector<Triplet*> * vectorToFeed = NULL;
            for(int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
                istringstream iss(lines.at(lineIndex));
                string s;
                Triplet * t;
                Doublet * d;
                int cptWords = 0;
                while(getline(iss, s, ' ')) {
                    if(lineIndex == 0) {
                        if(s != "seeds:"){
                            if(cptWords % 2 == 1) {
                                d = new Doublet();
                                d-> seedStart = stoll(s);
                            }
                            else {
                                d->seedRange = stoll(s);
                                m_seedRange.push_back(d);
                            }
                        } 
                    }
                    else if(s != "" && s != "map:") {
                        if(s == "seed-to-soil") vectorToFeed = & m_seedToSoil;
                        else if(s == "soil-to-fertilizer")  vectorToFeed = & m_soilToFertilizer;
                        else if(s == "fertilizer-to-water")  vectorToFeed = & m_fertilizeToWater;
                        else if(s == "water-to-light")  vectorToFeed = & m_waterToLight;
                        else if(s == "light-to-temperature") vectorToFeed = & m_lightToTemperature;
                        else if(s == "temperature-to-humidity")  vectorToFeed = & m_temperatureToHumidity;
                        else if(s == "humidity-to-location") vectorToFeed = & m_humidityToLocation;
                        else if(cptWords == 0) {
                            t = new Triplet();
                            t->destinationId = stoll(s);
                        }
                        else if(cptWords == 1) t->sourceId = stoll(s);
                        else if(cptWords == 2) {
                            t->sourceRange = stoll(s);

                            int pos = 0;
                            while(pos < vectorToFeed->size() && t->sourceId > vectorToFeed->at(pos)->sourceId) {
                                pos++;
                            }
                            vectorToFeed->insert(vectorToFeed->begin() + pos, t);
                        }
                        else cout << "weird state" << endl;
                    }
                    cptWords++;
                }
            }
        };

        void computeSeedToLocations() {

            clock_t start, end;

            for(int seedIndex = 0; seedIndex < m_seedRange.size(); seedIndex++) {

                start = clock();

                long long currentSeed = m_seedRange.at(seedIndex)->seedStart;
                Doublet * currentRange = m_seedRange.at(seedIndex);
                long long location = computeRangeResult(currentRange);
                m_bestLocations.push_back(location);

                end = clock();
                cout << "seed range " << seedIndex << " took " << end - start << "ms" << endl;

            }
        };

        long long getBestLocation() {
            long long bestLocation = LLONG_MAX;
            for(int i = 0; i < m_bestLocations.size(); i++) {
                if(m_bestLocations.at(i) < bestLocation) {
                    bestLocation = m_bestLocations.at(i);
                }
            }
            return bestLocation;
        };

    private:
        vector<Doublet*> m_seedRange;
        vector<Triplet*> m_seedToSoil;
        vector<Triplet*> m_soilToFertilizer;
        vector<Triplet*> m_fertilizeToWater;
        vector<Triplet*> m_waterToLight;
        vector<Triplet*> m_lightToTemperature;
        vector<Triplet*> m_temperatureToHumidity;
        vector<Triplet*> m_humidityToLocation;
        vector<long long> m_bestLocations;

        long long computeRangeResult(Doublet * currentSeedRange) {
            
            vector<Doublet *> singleValueVector;
            singleValueVector.push_back(currentSeedRange);

            vector<Doublet *> soils = computeResultsFromVector(&singleValueVector, &m_seedToSoil);
            vector<Doublet *> fertilizers = computeResultsFromVector(&soils, &m_soilToFertilizer);
            clearVectorData(&soils);
            vector<Doublet *> waters = computeResultsFromVector(&fertilizers, &m_fertilizeToWater);
            clearVectorData(&fertilizers);
            vector<Doublet *> lights = computeResultsFromVector(&waters, &m_waterToLight);
            clearVectorData(&waters);
            vector<Doublet *> temperatures = computeResultsFromVector(&lights, &m_lightToTemperature);
            clearVectorData(&lights);
            vector<Doublet *> humiditys = computeResultsFromVector(&temperatures, &m_temperatureToHumidity);
            clearVectorData(&temperatures);
            vector<Doublet *> locations = computeResultsFromVector(&humiditys, &m_humidityToLocation);
            clearVectorData(&humiditys);

            int bestIndex = 0;
            for(int locationIndex = 0; locationIndex < locations.size(); locationIndex++){
                if(locations.at(locationIndex)->seedStart < locations.at(bestIndex)->seedStart) bestIndex = locationIndex;
            }

            long long result = locations.at(bestIndex)->seedStart;

            clearVectorData(&locations);
            
            return result;
        };

        void clearVectorData(vector<Doublet *> * v) {
            while(!v->empty()) {
                delete v->back();
                v->pop_back();
            }
        }

        vector<Doublet*> computeResultsFromVector(
            vector<Doublet*> * sourceRanges, 
            vector<Triplet*> * orderedTripletVector)
        {

            vector<Doublet*> results;
            for(int sourceIndex = 0; sourceIndex < sourceRanges->size(); sourceIndex++) {

                long long sourceStart = sourceRanges->at(sourceIndex)->seedStart;
                long long sourceEnd = sourceStart + sourceRanges->at(sourceIndex)->seedRange - 1;

                for(int index = 0; index < orderedTripletVector->size(); index++) {

                    Triplet * currentTriplet = orderedTripletVector->at(index);
                    long long tripletStart = currentTriplet->sourceId;
                    long long tripletEnd = tripletStart + currentTriplet->sourceRange - 1;

                    //current triplet contains source start !
                    if(tripletStart <= sourceStart && tripletEnd >= sourceStart) 
                    {
                        long long gapBetweenSourceAndTripletStarts = sourceStart - tripletStart;

                        Doublet * doublet = new Doublet();

                        doublet->seedStart = currentTriplet->destinationId + gapBetweenSourceAndTripletStarts;

                        //source end is over current triplet end
                        if(tripletEnd <= sourceEnd) {
                            doublet->seedRange = sourceEnd - tripletEnd;
                            sourceStart += doublet->seedRange;
                        }
                        //current triplet contains source end full
                        else {
                            doublet->seedRange = sourceEnd - sourceStart;
                            sourceStart = sourceEnd;                            
                        }
                        results.push_back(doublet);
                    }
                    //our source is between two triplets !
                    else if(tripletStart > sourceStart && sourceStart != sourceEnd)
                    {
                        long long range = sourceEnd - sourceStart;

                        if(index + 1 < orderedTripletVector->size()) {
                            long long nextSourceId = orderedTripletVector->at(index + 1)->sourceId;
                            if(sourceEnd > nextSourceId) {
                                range = nextSourceId - sourceStart;
                                sourceStart = nextSourceId;
                            }
                        }

                        Doublet * doubletIntermediaire = new Doublet(sourceStart, range);
                        results.push_back(doubletIntermediaire);
                    }
                }// end ordered destinations

                //source is over the biggest triplet !
                if(sourceStart != sourceEnd) {
                    Doublet * reliquat = new Doublet(sourceStart, sourceEnd - sourceStart);
                    results.push_back(reliquat);
                }

            }// end sources loop
            return results;
        };
};

int main(char * args[]) {

    cout<< "Hello World !" << endl;

    clock_t start, end;

    ifstream infile("input.txt");
    string line;
    long long result = 0;
    vector<string> allLines;

    start = clock();

    while (getline(infile, line))
    {
        allLines.push_back(line);
    }

    Almanach a(allLines);
    a.computeSeedToLocations();
    result = a.getBestLocation();
    
    end = clock();

    cout << "result : " << result << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}