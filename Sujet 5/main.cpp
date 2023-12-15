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

class Almanach {

    public:
        Almanach(vector<string> lines) {

            vector<Triplet*> * vectorToFeed = NULL;
            for(int lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
                istringstream iss(lines.at(lineIndex));
                string s;
                Triplet * t;
                int cptWords = 0;
                while(getline(iss, s, ' ')) {
                    if(lineIndex == 0) {
                        if(s != "seeds:") m_seeds.push_back(stoll(s));
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
            for(int seedIndex = 0; seedIndex < m_seeds.size(); seedIndex++) {
                long long currentSeed = m_seeds.at(seedIndex);
                long long location = computeResult(currentSeed);
                m_seedToLocation.insert(pair<long long, long long> (currentSeed, location));
            }
        };

        long long getBestLocation() {
            long long bestLocation = LLONG_MAX;
            map<long long, long long>::iterator it;
            for (it=m_seedToLocation.begin(); it!=m_seedToLocation.end(); ++it) {
                if(it->second < bestLocation) bestLocation = it-> second;
            }
            return bestLocation;
        };

    private:
        vector<long long> m_seeds;
        vector<Triplet*> m_seedToSoil;
        vector<Triplet*> m_soilToFertilizer;
        vector<Triplet*> m_fertilizeToWater;
        vector<Triplet*> m_waterToLight;
        vector<Triplet*> m_lightToTemperature;
        vector<Triplet*> m_temperatureToHumidity;
        vector<Triplet*> m_humidityToLocation;
        map<long long, long long> m_seedToLocation;

        long long computeResult(long long seed) {
            long long soil = computeResultFromVector(seed, &m_seedToSoil);
            long long fertilizer = computeResultFromVector(soil, &m_soilToFertilizer);
            long long water = computeResultFromVector(fertilizer, &m_fertilizeToWater);
            long long light = computeResultFromVector(water, &m_waterToLight);
            long long temperature = computeResultFromVector(light, &m_lightToTemperature);
            long long humidity = computeResultFromVector(temperature, &m_temperatureToHumidity);
            long long location = computeResultFromVector(humidity, &m_humidityToLocation);

            return location;
        };

        long long computeResultFromVector(long long source, vector<Triplet*> * orderedTripletVector) {
            for(int index = 0; index < orderedTripletVector->size(); index++) {
                Triplet * currentTriplet = orderedTripletVector->at(index);
                long long idPlusRange = currentTriplet->sourceId + currentTriplet->sourceRange - 1;
                if(currentTriplet->sourceId <= source && idPlusRange >= source) 
                {
                    long long gap = source - currentTriplet->sourceId;
                    return currentTriplet->destinationId + gap;
                }
                else if(currentTriplet->sourceId > source){
                    return source;
                }
            }
            return source;
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