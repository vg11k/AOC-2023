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

struct Lens {
	string label;
	int id;
	Lens() { label = ""; id = 0;}
};

int main(char * args[]) {

    cout<< "Hello World !" << endl;

    bool debug = false;

    clock_t start, end;

    ifstream infile(filename.c_str());
    string line;
    long long result1 = 0;
    long long result2 = 0;
    vector<string> allLines;

    start = clock();

    while(getline(infile, line)) {
        allLines.push_back(line);
    }
	
	map<int, vector<Lens>*> hashmap;
	
	istringstream iss(allLines.at(0));
	string s;
	while(getline(iss, s, ',')) {
		long currentValue = 0;
		int hashbox = 0;
		
		int mode = 0;
		Lens lens;
		
		for(int cindex = 0; cindex < s.length(); cindex++) {
			char c = s[cindex];
			int asciicode = (int) c;
			long currentValuePreUpdate = currentValue;

			currentValue += asciicode;
			currentValue *= 17;
			currentValue %= 256;
			
			if(c == '-' || c == '=') {
				hashbox = currentValuePreUpdate;
				if(c == '-') mode = 1;
				else if(c== '=') mode = 2;
			}
			else if(mode == 0) lens.label += s.substr(cindex, 1);
			else lens.id = ((int) c) - 48;
		}
		
		if(mode == 2 && hashmap.find(hashbox) == hashmap.end()) {
			vector<Lens> * tab = new vector<Lens>();
			hashmap[hashbox] = tab;
		}
		
		if(mode == 2) {
			vector<Lens> * hashlist = hashmap.find(hashbox)->second;
			bool found = false;
			for(int i = 0; i < hashlist->size(); i++) {
				if(hashlist->at(i).label == lens.label) {
					hashlist->at(i).id = lens.id;
					found = true;
				}
			}
			if(!found) hashlist->push_back(lens);
		}
		else if (mode == 1) {
			if(hashmap.find(hashbox) != hashmap.end()) {
				vector<Lens> * hashlist = hashmap.find(hashbox)->second;
				for(int i = 0; i < hashlist->size(); i++) {
					if(hashlist->at(i).label == lens.label) {
						hashlist->erase (hashlist->begin()+i,hashlist->begin()+i+1);
					}
				}
			}			
		}
		else cout << "wtf ?? "<<endl;
		
		result1 += currentValue;
    }
	
	for (std::map<int,vector<Lens>*>::iterator it=hashmap.begin(); it!=hashmap.end(); ++it) {
		int hashbox = it->first;
		vector<Lens>* hashlist = it->second;
		
		for(int boxid = 0; boxid < hashlist->size(); boxid++) {
			result2 += (hashbox + 1) * (boxid + 1) * hashlist->at(boxid).id;
		}
	}
	
	
	end = clock();

    cout << "result1 : " << result1 << endl;
    cout << "result2 : " << result2 << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;

    return 0;
}