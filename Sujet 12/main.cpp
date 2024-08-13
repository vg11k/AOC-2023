#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <omp.h>

using namespace std;

// compile with: /openmp

static string fileInputName = "input.txt";
//static string fileInputName = "example.txt";

class Spring {
private:
    vector<string> conditions;
    vector<int> records;
    string conditionAsString;
    int maxRecordValue;
    long occurenceMaxOutputValue;
    int secondMaxRecordValue;
    int nbUnknown;
public:
    Spring(string line) {
        nbUnknown = 0;
        istringstream iss(line);
        string s;
        getline(iss, s, ' ');
        conditionAsString = s;
        for (int i = 0; i < s.length(); i++) {
            string sub = s.substr(i, 1);
            conditions.push_back(sub);
            if (sub == "?") nbUnknown++;
        }
        maxRecordValue = 0;
        secondMaxRecordValue = 0;
        occurenceMaxOutputValue = 0;
        getline(iss, s, ' ');
        istringstream iss2(s);
        string ss;
        while (getline(iss2, ss, ',')) {
            int v = stoi(ss);
            records.push_back(v);
            if (v > maxRecordValue) {
                maxRecordValue = v;
            }
        }
        for (int i = 0; i < records.size(); i++) {
            if (records.at(i) > secondMaxRecordValue && records.at(i) < maxRecordValue) {
                secondMaxRecordValue = records.at(i);
            }
            if (records.at(i) == maxRecordValue) {
                occurenceMaxOutputValue++;
            }
        }
    }

    Spring(Spring * originalSpring, int replicateCount) {
        for(int i = 0; i < replicateCount; i++) {
            if(i > 0) conditions.push_back("?");
            for(int j = 0; j < originalSpring->conditions.size(); j++) {
                conditions.push_back(originalSpring->conditions.at(j));
            }
            for(int j= 0; j < originalSpring->records.size(); j++) {
                records.push_back(originalSpring->records.at(j));
            }
        }
    }

    vector<int> getRecord() { return records; }

    vector<string> getConditions() {return conditions;}

    vector<int>* stringToRecord(string val) {
        vector<int>* record = new vector<int>();
        record->push_back(0);
        for (int cindex = 0; cindex < val.length(); cindex++) {
            string character = val.substr(cindex, 1);
            if (character == "#") record->at(record->size() - 1)++;
            else if (record->at(record->size() - 1) != 0) record->push_back(0);
        }
        if (record->at(record->size() - 1) == 0)
            record->pop_back();
        return record;
    }
};

class CacheWithHash {
    private :
        int m_cursor;
        vector<int> m_computedOutput;
        int m_damageCounter;
        string m_hash;
    public : 
        CacheWithHash(int cursor, vector<int> * computedOutput, int damageCounter) {
            m_cursor = cursor;
            for(int cpt = 0; cpt < computedOutput->size(); cpt++)
                m_computedOutput.push_back(computedOutput->at(cpt));
            m_damageCounter = damageCounter;
            m_hash = "";
        }

         string getHash() {
            if(m_hash != "") return m_hash;
            for(int i = 0; i < m_computedOutput.size(); i++) {
                m_hash += std::to_string(m_computedOutput.at(i)) + "-";
            }
            m_hash += "a" + std::to_string(m_damageCounter);
            m_hash += "b" + std::to_string(m_cursor);
            return m_hash;
        }
};

 unsigned long long int recursiveResult(int cursor, 
                            Spring * spring, 
                            vector<int> * computedOutput, 
                            int * damageCounter,
                            map< string,  unsigned long long int> & hashToCache) {

    CacheWithHash cacheWHash(cursor, computedOutput, damageCounter[0]);
    if(hashToCache.find(cacheWHash.getHash()) != hashToCache.end()) {
        //cout << "raccourci " << cursor << endl;
        return hashToCache.find(cacheWHash.getHash())->second;
    }
    //cout << "recur " << cursor << endl;
    
    

    if(damageCounter[0] > 0) {
        if(computedOutput->size() >= spring->getRecord().size()) {
            return 0;
        }

        if(damageCounter[0] > spring->getRecord().at(computedOutput->size())) {
            return 0;
        }
    }

    if(cursor >= spring->getConditions().size()) {
        if(damageCounter[0] > 0) {
            computedOutput->push_back(damageCounter[0]);
        } 

        bool valid = computedOutput->size() == spring->getRecord().size() &&
                computedOutput->at(computedOutput->size() - 1) == 
                    spring->getRecord().at(spring->getRecord().size() - 1);

        if(damageCounter[0] > 0) {
            computedOutput->pop_back();
        }

        return valid ? 1 : 0;
    }

    unsigned long long int result = 0;
    string state = spring->getConditions().at(cursor);

    if(state == "#") {
        damageCounter[0]++;
        result = recursiveResult(cursor + 1, spring, computedOutput, damageCounter, hashToCache);
        damageCounter[0]--;
        hashToCache.insert(
                    pair< string,  unsigned long long int>(cacheWHash.getHash(), result));
    }
    else if(state == ".") {
        int damageValue = damageCounter[0];
        bool valid = true;
        if(damageValue > 0) {
            computedOutput->push_back(damageValue);
            valid = computedOutput->size() <= spring->getRecord().size() &&
                damageValue == spring->getRecord().at(computedOutput->size() - 1);
            damageCounter[0] = 0;
        }
        result = valid ? recursiveResult(cursor + 1, spring, computedOutput, damageCounter, hashToCache) : 0;
        if(damageValue > 0) {
            computedOutput->pop_back();
        }
        damageCounter[0] = damageValue;
        hashToCache.insert(pair< string,  unsigned long long int>(cacheWHash.getHash(), result));
    }
    else {
        // simulate '#'
        damageCounter[0]++;
        result = recursiveResult(cursor + 1, spring, computedOutput, damageCounter, hashToCache);
        damageCounter[0]--;

        // simulate '.'
        int damageValue = damageCounter[0];
        bool valid = true;
        if(damageValue > 0) {
            computedOutput->push_back(damageValue);
            valid = computedOutput->size() <= spring->getRecord().size() &&
                damageValue == spring->getRecord().at(computedOutput->size() - 1);
            damageCounter[0] = 0;
        }
        result = valid ? result + recursiveResult(cursor + 1, spring, computedOutput, damageCounter, hashToCache) : result;
        if(damageValue > 0) {
            computedOutput->pop_back();
        }
        damageCounter[0] = damageValue;
        hashToCache.insert(pair< string,  unsigned long long int>(cacheWHash.getHash(), result));
    }

    return result;
}



 unsigned long long int countValidConfiguration(Spring * spring) {

    map< string,  unsigned long long int> hashToCache;
    vector<int> * computedOutput = new vector<int>();
    int * damageCounter = new int[1]; 
    damageCounter[0] = 0;

    unsigned long long int result = recursiveResult(0, spring, computedOutput, damageCounter, hashToCache);

    delete computedOutput;
    delete damageCounter;

    return result;
}

 unsigned long long int initateComputation(const int nbLinesToDo, const vector<Spring*> & springs, const int nbReplication) {

    unsigned long long int * results = new unsigned long long int[nbLinesToDo];

    #pragma omp parallel for
    for(int lineIndex = 0; lineIndex < nbLinesToDo; lineIndex++) {
        
        Spring * extendedSpring = new Spring(springs.at(lineIndex), nbReplication);
        unsigned long long int val = countValidConfiguration(extendedSpring);
        results[lineIndex] = val;

        delete extendedSpring;
    }

    unsigned long long int result = 0;
    for(int lineIndex = 0; lineIndex < nbLinesToDo; lineIndex++) {
        result += results[lineIndex];
    }

    delete results;
    return result;
}



int main(char* args[]) {

    std::cout << "Hello World !" << endl;
    bool debug = false;
    clock_t start, end;

    ifstream infile(fileInputName);
    string line;
    unsigned long long int result = 0;
    vector<string> allLines;

    start = clock();

    while (getline(infile, line)) {
        allLines.push_back(line);
    }

    vector<Spring*> springs;
    for (int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        springs.push_back(new Spring(allLines.at(lineIndex)));
    }

    //first part
    result = initateComputation(allLines.size(), springs, 1);
    cout << "part 1 : " << result << endl;

    result = 0;
    //second part
    result = initateComputation(allLines.size(), springs, 5);
    cout << "part 2 : " << result << endl;

    end = clock();
    std::cout << end - start << " ms to compute." << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}