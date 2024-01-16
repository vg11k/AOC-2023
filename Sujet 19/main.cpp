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

static long long globalResultat = 0;
static long long globalVerification = 0;

enum PartCat { X, M, A, S};
enum Instruction { MORE, LESS};
enum ResultState { NEXT, GOTO, ACCEPTED, REJECTED};

static vector<PartCat> cats;

string stateToString(ResultState rs) {
    if(rs == NEXT) return "next";
    if(rs == GOTO) return "goto";
    if(rs == ACCEPTED) return "accepted";
    return "rejected";
}

string catAsString(PartCat cat) {
    if(cat == X) return "X";
    else if(cat == M) return "M";
    else if(cat == A) return "A";
    else return "S";
}

struct Part {
    map<PartCat,long> parts;
    long sumResult;
    Part(string input) {
        string cleanedInput = input.substr(1, input.length() - 2);
        sumResult = 0;
        istringstream iss(cleanedInput);
        string s;
        while(getline(iss, s, ',')) {
            PartCat p;
            string sp = s.substr(0,1);
            long val = stol(s.substr(2));
            if(sp == "x") p = X;
            else if (sp == "m") p = M;
            else if (sp == "a") p = A;
            else if (sp == "s") p = S;
            else cout << "wtf cat" << sp << " for input " << input << endl;
            parts[p] = val;
            sumResult += val;
        }
    }

    Part(long x, long m, long a, long s) {
        parts[X] = x;
        parts[M] = m;
        parts[A] = a;
        parts[S] = s;
        sumResult = x + m + a + s;
    }

    void displaySelf() {
        for (auto i = parts.begin(); i != parts.end(); i++)
		    cout << i->first << " " << i->second << " ";
		cout << endl;
    }
};

//crappy job to keep all classes in a single file
class VWorkflow {
    public:
        virtual void displaySelf() {}
        virtual string getName() {return "";}
        virtual void processSpaces(map<string, VWorkflow*> workflows,  
                            map<PartCat,vector<vector<int>*>*> feedMap){cout << "virtuaaal" << endl;}
};


void cleanMap(map<PartCat, ResultState*> ruleMap) {
    for(int i = 0; i < 4; i++) delete ruleMap[cats.at(i)];
}

  map<PartCat,vector<vector<int>*>*> buildCopy( map<PartCat,vector<vector<int>*>*> original) {

    //lets have some memory leak
    map<PartCat,vector<vector<int>*>*> feedMap;
    for(int i = 0; i < 4; i++) {
        PartCat cat = cats.at(i);
        feedMap[cat] = new vector<vector<int>*>();
        for(int j = 0; j < original[cat]->size(); j++) {
            vector<int> * space = new vector<int>();
            space->push_back(original[cat]->at(j)->at(0));
            space->push_back(original[cat]->at(j)->at(1));
            feedMap[cat]->push_back(space);
        }
    }

    return feedMap;
  }


struct RuleResult {
    string workFlowName;
    ResultState state;
    map<PartCat, ResultState*> validCombinations;

    RuleResult() {}
    RuleResult(string input) {
        workFlowName == "no name";
        if(input == "A") state = ACCEPTED;
        else if(input == "R") state = REJECTED;
        else {
            workFlowName = input;
            state = GOTO;
        }
    }
    
    void displaySelf() {
        if(state == ACCEPTED) cout << "accepted";
        else if(state == REJECTED) cout << "rejected";
        else if(state == GOTO) cout << "goto " << workFlowName;
        else cout << "next";
        cout << endl;
    }
};

class SimpleRule {
    protected:
        RuleResult result;
    public:
        SimpleRule(){}
        SimpleRule(string input) {
            result = RuleResult(input);
        }

        virtual RuleResult process(Part * p) {
            return result;
        }

        virtual RuleResult processSpaces(map<PartCat,vector<vector<int>*>*> * spaces) {
            return result;
        }

        virtual void splitMap(  map<PartCat,vector<vector<int>*>*> & inputMap, 
                        map<PartCat,vector<vector<int>*>*> * usedOutputMap, 
                        map<PartCat,vector<vector<int>*>*> * unusedOutputMap) {

            for(int cati = 0; cati < cats.size(); cati++) {
                PartCat c = cats.at(cati);
                (*usedOutputMap)[c] = new vector<vector<int>*>();
                for(int i = 0; i < inputMap[c]->size(); i++) {

                    vector<int> * spaceToCopy = inputMap[c]->at(i);
                    vector<int> * space = new vector<int>();

                    (*usedOutputMap)[c]->push_back(space); 
                    space->push_back(spaceToCopy->at(0));
                    space->push_back(spaceToCopy->at(1));
                }
                (*unusedOutputMap)[c] = new vector<vector<int>*>();
            }
        }

        virtual void displaySelf() {
            result.displaySelf();
        }

        RuleResult getResult() {return result;}
        

};

class ComplexRule: public SimpleRule {
    private:
        PartCat cat;
        Instruction instruction;
        long value;
        map<PartCat, ResultState*> cover;
    public:
        ComplexRule(string s) {
            int index = s.find(":");
            string conditionAsString = s.substr(0, index);
            string resultAsString = s.substr(index + 1);

            string catAsString = conditionAsString.substr(0,1);
            if(catAsString == "x") cat = X;
            else if(catAsString == "m") cat = M;
            else if(catAsString == "a") cat = A;
            else if(catAsString == "s") cat = S;
            else cout << "wtf cat = " << catAsString << " for string [" << s << "]" << endl;

            value = stol(conditionAsString.substr(2)) - 1;

            if(conditionAsString.substr(1,1) == "<") {
                instruction = LESS;
            }
            else {
                instruction = MORE;
            }
            result = RuleResult(resultAsString);
        }
        
        RuleResult process(Part * p) {
            long partCat = p->parts[cat];
            if(instruction == MORE && partCat > value) return result;
            else if(instruction == LESS && partCat < value) return result;
            RuleResult next;
            next.state = NEXT;
            return next;
        }

        RuleResult processSpaces(map<PartCat,vector<vector<int>*>*> spaces) {
            if(spaces[cat]->size() > 0) {
                long partCat = spaces[cat]->at(0)->at(0);
                if(instruction == MORE && partCat > value) return result;
                partCat = spaces[cat]->at(spaces[cat]->size() - 1)->at(1);
                if(instruction == LESS && partCat < value) return result;
            }
            cout << "err" << endl;
        }

        void splitMap(  map<PartCat,vector<vector<int>*>*> & inputMap, 
                        map<PartCat,vector<vector<int>*>*> * usedOutputMap, 
                        map<PartCat,vector<vector<int>*>*> * unusedOutputMap) {

            for(int cati = 0; cati < 4; cati++) {
                if(cats.at(cati) != cat) {
                    vector<vector<int>*> * usedSpaces = new vector<vector<int>*>();
                    (*usedOutputMap)[cats.at(cati)] = usedSpaces;

                    vector<vector<int>*> * unusedSpaces = new vector<vector<int>*>();
                    (*unusedOutputMap)[cats.at(cati)] = unusedSpaces;
                    for(int i = 0; i < inputMap[cats.at(cati)]->size(); i++) {
                        vector<int> * spaceToCopy = inputMap[cats.at(cati)]->at(i);

                        vector<int> * space = new vector<int>();
                        usedSpaces->push_back(space);
                        space->push_back(spaceToCopy->at(0));
                        space->push_back(spaceToCopy->at(1));

                        space = new vector<int>();
                        unusedSpaces->push_back(space);
                        space->push_back(spaceToCopy->at(0));
                        space->push_back(spaceToCopy->at(1));
                    }
                }
                else {
                    vector<vector<int>*> * inputVectors = inputMap[cat];
                    vector<vector<int>*> * usedVectors = new vector<vector<int>*>();
                    (*usedOutputMap)[cat] = usedVectors;
                    vector<vector<int>*> * unusedVectors  = new vector<vector<int>*>();
                    (*unusedOutputMap)[cat] = unusedVectors;

                    bool hasCut = false;
                    if(instruction == LESS) {
                        for(int i = 0; i < inputVectors->size(); i++) {
                            vector<int> * space = new vector<int>();
                            int val = inputVectors->at(i)->at(0);
                            space->push_back(val);
                            if(val > value && !hasCut) {
                                hasCut = true;
                            }

                            val = inputVectors->at(i)->at(1);
                            if(val > value && !hasCut) {
                                hasCut = true;
                                space->push_back(value - 1);
                                usedVectors->push_back(space);
                                space = new vector<int>();
                                space->push_back(value);
                            }
                            space->push_back(val);
                            
                            if(!hasCut) usedVectors->push_back(space);
                            else unusedVectors->push_back(space);
                        }
                    }
                    else {
                        for(int i = 0; i < inputVectors->size(); i++) {
                            vector<int> * space = new vector<int>();
                            int val = inputVectors->at(i)->at(0);
                            space->push_back(val);
                            if(val > value && !hasCut) {
                                hasCut = true;
                            }

                            val = inputVectors->at(i)->at(1);
                            if(val > value && !hasCut) {
                                hasCut = true;
                                space->push_back(value);
                                unusedVectors->push_back(space);
                                space = new vector<int>();
                                space->push_back(value + 1);  
                            }
                            space->push_back(val);

                            if(!hasCut) unusedVectors->push_back(space);
                            else usedVectors->push_back(space);
                        }
                    }

                }
            }
        }


        void displaySelf() {
            if(cat == X) cout << "X";
            else if(cat == M) cout << "M";
            else if(cat == A) cout << "A";
            else if(cat == S) cout << "S";

            if(instruction == LESS) cout << "<";
            else cout << ">";

            cout << value << ":";
            SimpleRule::displaySelf();
        }
};

class Workflow : public VWorkflow {
    string name;
    vector<SimpleRule*> rules;

    public:
        Workflow(string input) {
            int delim = input.find("{");
            name = input.substr(0, delim);
            string rulesAsString = input.substr(delim + 1, input.size() - name.length() - 2);
            istringstream iss(rulesAsString);
            string s;
            while(getline(iss, s, ',')) {
                if(s.find(":") ==std::string::npos) {
                    rules.push_back(new SimpleRule(s));
                }
                else {
                    rules.push_back(new ComplexRule(s));
                }
            }
        }

        RuleResult process(Part * p) {
            for(unsigned int ruleId = 0; ruleId < rules.size(); ruleId++) {
                SimpleRule * r = rules.at(ruleId);
                RuleResult result = r->process(p);
                if(result.state != NEXT) return result;
            }
        }

        void processSpaces(map<string, VWorkflow*> workflows,  
                            map<PartCat,vector<vector<int>*>*> feedMap) {
                                

            map<PartCat,vector<vector<int>*>*> localSpaces;
            localSpaces = buildCopy(feedMap);
            
            for(unsigned int ruleId = 0; ruleId < rules.size(); ruleId++) {

                SimpleRule * r = rules.at(ruleId);
                map<PartCat,vector<vector<int>*>*> * splitUsedMap = new map<PartCat,vector<vector<int>*>*>();
                map<PartCat,vector<vector<int>*>*> * splitUnusedMap = new map<PartCat,vector<vector<int>*>*>();

                r->splitMap(localSpaces, splitUsedMap, splitUnusedMap);

                RuleResult rr = r->processSpaces(splitUsedMap);

                if(rr.state != NEXT) {
                    if(rr.state == ACCEPTED) {
                        vector<long long> vsubres;
                        for(int icat = 0; icat < cats.size(); icat++) {
                            int subres = 1;
                            vector<vector<int>*> * spaces = (*splitUsedMap)[cats.at(icat)];
                            for(int i = 0; i < spaces->size(); i++) {
                                int a = spaces->at(i)->at(0);
                                int b = spaces->at(i)->at(1);
                                subres += b - a;
                            }
                            vsubres.push_back(subres);
                        }

                        long long res = vsubres.at(0) * vsubres.at(1) * vsubres.at(2) * vsubres.at(3);
                        globalResultat += res;
                        globalVerification += res;
                    }
                    if(rr.state == REJECTED) {
                        vector<long long> vsubres;
                        for(int icat = 0; icat < cats.size(); icat++) {
                            int subres = 1;
                            vector<vector<int>*> * spaces = (*splitUsedMap)[cats.at(icat)];
                            for(int i = 0; i < spaces->size(); i++) {
                                int a = spaces->at(i)->at(0);
                                int b = spaces->at(i)->at(1);
                                subres += b - a;
                            }
                            vsubres.push_back(subres);
                        }

                        long long res = vsubres.at(0) * vsubres.at(1) * vsubres.at(2) * vsubres.at(3);
                        globalVerification+= res;
                    }
                    else if(rr.state == GOTO) {
                        string childWorkflow = rr.workFlowName;
                        Workflow * w = (Workflow*) workflows[childWorkflow]; 
                        w->processSpaces(workflows, *splitUsedMap);
                    }
                }
                else cout << "err" << endl;

                localSpaces = buildCopy(*splitUnusedMap);//memory leeeeak


                for(int cati = 0; cati < 4; cati++) {
                    vector<vector<int>*> * vec = (*splitUsedMap)[cats.at(cati)];
                    for(int si = 0; si < vec->size(); si++)
                        delete vec->at(si);
                    delete vec;
                    vec = (*splitUnusedMap)[cats.at(cati)];
                    for(int si = 0; si < vec->size(); si++)
                        delete vec->at(si);
                    delete vec;
                }

                delete splitUsedMap;
                delete splitUnusedMap;
            }

        }

        void displaySelf() {
            cout << "Workflow " << name << " with " << rules.size() << " rules : " << endl;
            for(int i = 0; i < rules.size(); i++) rules.at(i)->displaySelf();
            cout << endl;
        }

        string getName() { return name;}


};


int main(char * args[]) {

    std::cout<< "Hello World !" << endl;

    clock_t start, end;

    ifstream infile(filename);
    string line;
    long long result1 = 0;
    vector<string> allLines;

    start = clock();

    while(getline(infile, line)) {
        allLines.push_back(line);
    }

    cats.push_back(X);
    cats.push_back(M);
    cats.push_back(A);
    cats.push_back(S);

    map<string, VWorkflow*> workflows;
    vector<Part*> parts;

    bool workflowCompleted = false;
    for(int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        string line = allLines.at(lineIndex);
        if(line == "") {
            workflowCompleted = true;
        }
        else if(!workflowCompleted) {
            Workflow * w = new Workflow(line);
            workflows[w->getName()] = w;
        }
        else {
            parts.push_back(new Part(line));
        }
    }

    VWorkflow * w;
    string workflowname = "in";

    for(int partIdx = 0; partIdx < parts.size(); partIdx++) {
        Workflow * wf = (Workflow*) workflows[workflowname];
        Part * p = parts.at(partIdx);
        RuleResult rr = wf->process(p);
        while(rr.state == GOTO) {
            wf = (Workflow*) workflows[rr.workFlowName];
            rr = wf->process(p);
        }

        if(rr.state == ACCEPTED) {
            result1 += p->sumResult;
        }
        else if(rr.state == REJECTED) {
        } 
        else cout << "unfinished business ! " << endl;
    }

    cout << "part 1 complete" << endl;

    //cl main.cpp /O2 /openmp
    //don't do that, it's brute-force and take 6 months at least.
    /*
    #pragma omp parallel for
    for(int xi = 0; xi < 4000; xi++) {
        for(unsigned int mi = 0; mi < 4000; mi++) {
            for(unsigned int ai = 0; ai < 4000; ai++) {
                for(unsigned int si = 0; si < 4000; si++) {
                    Part * p = new Part(xi, mi, ai, si);
                    Workflow * wf = (Workflow*) workflows["in"];
                    RuleResult rr = wf->process(p);
                    while(rr.state == GOTO) {
                        wf = (Workflow*) workflows[rr.workFlowName];
                        rr = wf->process(p);
                    }
                    if(rr.state == ACCEPTED) result2++;
                    delete p;
                }
            }
            cout << "xi = " << xi << " mi = " << mi << " proc = " << omp_get_thread_num() << endl;
        }
    }
    //*/

    map<PartCat,vector<vector<int>*>*> feedMap;
    for(int i = 0; i < 4; i++) {
        PartCat cat = cats.at(i);
        feedMap[cat] = new vector<vector<int>*>();
        vector<int> * vec = new vector<int>(); 
        vec->push_back(0); vec->push_back(3999); 
        feedMap[cat]->push_back(vec);
    }
    
    w = workflows[workflowname]; 
    w->processSpaces(workflows, feedMap);

    end = clock();

    std::cout << "result 1 : " << result1 << endl;
    std::cout << "result 2 : " << globalResultat << endl;
    std::cout << "verification (256MM) : " << globalVerification << endl;
    std::cout << end - start << " ms to compute." << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}