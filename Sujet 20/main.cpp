#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <omp.h>
using namespace std;

static const string filename = "input.txt";
//static const string filename = "example.txt";
//static const string filename = "example2.txt";

long long maxLoop = LLONG_MAX;

static long long lowPulseCounter = 0;
static long long highPulseCounter = 0;

enum Pulse { Low, High };
enum FlipFlopState {On, Off};

string pulse2str(Pulse p) {
    if(p == Low) return "low";
    else return "high";
}

struct PileNode {
    string m_moduleName;
    string m_emitterName;
    Pulse m_pulse;
    PileNode(string s, string ss, Pulse p) : m_moduleName(s), m_emitterName(ss), m_pulse(p) {}
};

class Module {
    //When it receives a pulse, it sends the same pulse to all of its destination modules.

    protected:
        string m_name;
        vector<string> m_destinations;
        Pulse m_pulse;

    public:
        Module(string s) : m_name(s), m_pulse(Low) {m_destinations = vector<string>();}
        Module() {}
        string getName() { return m_name;}
        vector<string> & getDestinations() {return m_destinations;}
        virtual void displaySelf() {
            cout << "module " << m_name << " -> ";
            for(int i = 0; i < m_destinations.size(); i++) {
                cout << m_destinations.at(i) << " ";
            }
            cout << endl;
        }
        virtual void processPulse(Pulse pulse, vector<PileNode*> * pile, map<string, Module*> * name2module, string inputModuleStr) {
            for(int i = 0; i < m_destinations.size(); i++) {

                PileNode * node = new PileNode(m_destinations.at(i), m_name, pulse);
                pile->push_back(node);

                if(pulse == High) highPulseCounter++;
                else lowPulseCounter++;

                //cout << m_name << " -" << pulse2str(pulse) << " " << m_destinations.at(i) << endl;
            }
        }
        virtual void notifyAsParent(string parent) {}
};

class FlipFlopModule : public Module {
    //on or off
    FlipFlopState m_state;

    //if receives a high pulse, it is ignored and nothing happens
    //if receives a low pulse, itflips between on and off.
        //If it was off, it turns on and sends a high pulse. 
        //If it was on, it turns off and sends a low pulse.

    public:
        FlipFlopModule(string s) {
            m_state = Off;
            m_name = s;
            m_pulse = Low;
            m_destinations = vector<string>();
            }
        void displaySelf() {
            cout << "flipflop " << m_name << " -> ";
            for(int i = 0; i < m_destinations.size(); i++) {
                cout << m_destinations.at(i) << " ";
            }
            cout << endl;
        }
        void notifyAsParent(string parent) {}
        void processPulse(Pulse pulse, vector<PileNode*> * pile, map<string, Module*> * name2module, string inputModuleStr) {
            if(pulse == Low) {
                Pulse outPulse;
                if(m_state == Off) {
                    m_state = On;
                    outPulse = High;
                }
                else {
                    m_state = Off;
                    outPulse = Low;
                }

                for(int i = 0; i < m_destinations.size(); i++) {
                    PileNode * node = new PileNode(m_destinations.at(i), m_name, outPulse);
                    pile->push_back(node);

                    if(outPulse == High) highPulseCounter++;
                    else lowPulseCounter++;

                    //cout << m_name << " -" << pulse2str(outPulse) << " " << m_destinations.at(i) << endl;
                }
            }
        }
};

class ConjunctionModule : public Module {
    //It remember the type of the most recent pulse received from each of their connected input modules
    //When a pulse is received, the conjunction module first updates its memory for that input. 
    //if it remembers high pulses for all inputs, it sends a low pulse; 
    //otherwise, it sends a high pulse.

    map<string, Pulse> historyPulse;

    public:
        ConjunctionModule(string s) {
            m_name = s;
            m_pulse = Low;
            m_destinations = vector<string>();
        }
        void displaySelf() {
            cout << "conjunction " << m_name << " -> ";
            for(int i = 0; i < m_destinations.size(); i++) {
                cout << m_destinations.at(i) << " ";
            }
            cout << endl;
        }
    void notifyAsParent(string parent) {
        historyPulse[parent] = Low;
    }
    void processPulse(Pulse pulse, vector<PileNode*> * pile, map<string, Module*> * name2module, string inputModuleStr) {

        historyPulse[inputModuleStr] = pulse;

        //cout << inputModuleStr << " " << historyPulse[inputModuleStr] << endl;

        Pulse outPulse = Low;
        for(auto i : historyPulse) {
            //cout << "histo " << i.first << " " << pulse2str(i.second) << endl;
            if(i.second == Low) outPulse = High;
        }

        for(int i = 0; i < m_destinations.size(); i++) {

            PileNode * node = new PileNode(m_destinations.at(i), m_name, outPulse);
            pile->push_back(node);

            if(outPulse == High) highPulseCounter++;
            else lowPulseCounter++;

            //cout << m_name << " -" << pulse2str(outPulse) << " " << m_destinations.at(i) << endl;

        }
    }
};

class DummyModule : public Module {
    int m_cptLow;
    public:
        DummyModule(string s) { m_name = s; m_cptLow = 0;}
        void processPulse(Pulse pulse, vector<PileNode*> * pile, map<string, Module*> * name2module, string inputModuleStr) {
            if(pulse == Low) m_cptLow++;
        }
        int getCptLow() {return m_cptLow;}
        void resetCptLow() {m_cptLow = 0;}

};



int main(char * args[]) {

    std::cout<< "Hello World !" << endl;

    clock_t start, end;

    ifstream infile(filename);
    string line;
    long long result1 = 0;
    long long result2 = 0;
    vector<string> allLines;

    start = clock();

    while(getline(infile, line)) {
        allLines.push_back(line);
    }

    vector<Module*> * modules = new vector<Module*>();
    map<string, Module*> * name2module = new map<string, Module*>();

    for(int i = 0; i < allLines.size(); i++) {
        string line = allLines.at(i);
        istringstream iss(line);
        string s;
        getline(iss, s, ' ');
        string trash; getline(iss, trash, ' ');

        Module * module;
        if(s == "broadcaster") {
            module = new Module(s);
        }
        else if(s.substr(0,1) == "%") {
            module = new FlipFlopModule(s.substr(1, s.size() - 1));
        }
        else {
            module = new ConjunctionModule(s.substr(1, s.size() - 1));
        }

        string destination;
        while(getline(iss, destination, ' ')) {
            int ssize = destination.size();
            if(destination.find(",") != std::string::npos) ssize--;
            string cleanedDestination = destination.substr(0, ssize);
            module->getDestinations().push_back(cleanedDestination);
        }

        modules->push_back(module);
        (*name2module)[module->getName()] = module;
    }

    DummyModule * dummy = new DummyModule("output");
    modules->push_back(dummy);
    (*name2module)[dummy->getName()] = dummy;

    /*
    for(int i = 0; i < modules->size(); i++) {
        Module * m = modules->at(i);
        m->displaySelf();
    }//*/

    /*
    for(auto i : (*name2module)) {
        cout << i.first << " " << i.second->getName() << endl;
    }//*/

    for(int i = 0; i < modules->size(); i++) {
        Module * module = modules->at(i);
        string moduleName = module->getName();
        for(int j = 0; j < module->getDestinations().size(); j++) {
            string targetName = module->getDestinations().at(j);

            if((*name2module)[targetName] == NULL) {
                DummyModule * dumm = new DummyModule(targetName);
                modules->push_back(dumm);
                (*name2module)[dumm->getName()] = dumm;
            }

            (*name2module)[targetName]->notifyAsParent(moduleName);
        }
    }

    vector<PileNode*> * pile = new vector<PileNode*>();
    
    bool part2Found = false;

    for(long long i = 0; i < maxLoop && !part2Found; i++) {

        PileNode * initialNode = new PileNode("broadcaster", "Button", Low);
        pile->push_back(initialNode);
        lowPulseCounter++;

        DummyModule * rxmodule = (DummyModule *) (*name2module)["rx"];
        rxmodule->resetCptLow();

        while(!pile->empty()) {
            PileNode * headNode = pile->at(0);
            Module * headModule = (*name2module)[headNode->m_moduleName];
            headModule->processPulse(headNode->m_pulse, pile, name2module, headNode->m_emitterName);
            pile->erase(pile->begin(), pile->begin()+1);
            delete headNode;
        }

        
        if(rxmodule->getCptLow() == 1 && !part2Found) {
            part2Found = true;
            result2 = i;
        }

        if(i % 1000000 == 0) cout << i / 1000000 << "M loops "<< endl;
        //else cout << "rx low count = " << rxmodule->getCptLow() << endl;
    }

    cout << lowPulseCounter << " " << highPulseCounter << endl;
    result1 = lowPulseCounter  * highPulseCounter;

    end = clock();

    std::cout << "result 1 : " << result1 << endl;
    std::cout << "result 2 : " << result2 << endl;
    std::cout << end - start << " ms to compute." << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}