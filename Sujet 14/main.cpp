#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

//static const string filename = "example.txt";
static const string filename = "input.txt";

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
	
	int largeur = 0;
	int longueur = 0;
	
	vector<vector<string>*> grid;
	for(int i = 0; i < allLines.size(); i++) {
		vector<string> * line = new vector<string>();
		grid.push_back(line);
		for(int j = 0; j < allLines.at(i).length(); j++) 
		{
			line->push_back("" + allLines.at(i).substr(j,1));
		}
	}
	
	//4days of computing
	int cyclesToDo = 1000000000;
	
	vector<string> dico;
	
	for(int cyclesCounter = 0; 
			cyclesCounter < cyclesToDo; 
			cyclesCounter++) {
	
		
		if(cyclesCounter % 1000000 == 0)
			cout << cyclesCounter << " sur 1000000000 soit " << (int) ((float) cyclesCounter / (float) cyclesToDo * 100) << "%" <<  endl;
	
		int tampon[102];
		//top
		for(int i = 0; i < grid.size(); i++) {
			for(int j = 0; j < grid.at(i)->size(); j++) {
				if(i == 0) {
					tampon[j] = 0;
				}
				
				if(grid.at(i)->at(j) == ".") {
					tampon[j]++;
				}
				else if(grid.at(i)->at(j) == "#") {
					tampon[j] = 0;
				}
				else if(grid.at(i)->at(j) == "O") {
					grid.at(i)->at(j) = ".";
					grid.at(i - tampon[j])->at(j) = "O";
				}
				else cout << "wtf ? " << endl;
			}
		}
		
		//left
		for(int i = 0; i < grid.size(); i++) {
			int tmp = 0;
			for(int j = 0; j < grid.at(i)->size(); j++) {
				if(grid.at(i)->at(j) == ".") {
					tmp++;
				}
				else if(grid.at(i)->at(j) == "#") {
					tmp = 0;
				}
				else if(grid.at(i)->at(j) == "O") {
					grid.at(i)->at(j) = ".";
					grid.at(i)->at(j - tmp) = "O";
				}
				else cout << "wtf ? " << endl;
			}
		}
			
		//down
		for(int i = grid.size() - 1; i > -1; i--) {
			for(int j = 0; j < grid.at(i)->size(); j++) {
				if(i == grid.size() - 1) {
					tampon[j] = 0;
				}
				
				if(grid.at(i)->at(j) == ".") {
					tampon[j]++;
				}
				else if(grid.at(i)->at(j) == "#") {
					tampon[j] = 0;
				}
				else if(grid.at(i)->at(j) == "O") {
					grid.at(i)->at(j) = ".";
					grid.at(i + tampon[j])->at(j) = "O";
				}
				else cout << "wtf ? " << endl;
			}
		}
		
		//right
		for(int i = 0; i < grid.size(); i++) {
			int tmp = 0;
			for(int j = grid.at(i)->size() - 1; j > -1 ; j--) {
				if(grid.at(i)->at(j) == ".") {
					tmp++;
				}
				else if(grid.at(i)->at(j) == "#") {
					tmp = 0;
				}
				else if(grid.at(i)->at(j) == "O") {
					grid.at(i)->at(j) = ".";
					grid.at(i)->at(j + tmp) = "O";
				}
				else cout << "wtf ? " << endl;
			}
		}
		
		/*string generatedInput = "";
		for(int i = 0; i < grid.size(); i++) {
			for(int j = 0; j < grid.at(i)->size(); j++) {
				generatedInput += grid.at(i)->at(j);
			}
		}
		
		for(int si = 0; si < dico.size(); si++) {
			if(dico.at(si) == generatedInput) cout << si << " est une repetition avec "<< cyclesCounter << endl;
			else dico.push_back(generatedInput);
		}*/
		
	}//end cycle
	
	
	
	for(int i = 0; i < grid.size(); i++) {
		for(int j = 0; j < grid.at(i)->size(); j++) {
			if(grid.at(i)->at(j) == "O") {
				result1 += grid.size() - i;
			}
		}
	}
	
	
	for(int i = 0; i < grid.size(); i++) {
		for(int j = 0; j < grid.at(i)->size(); j++) {
			cout << grid.at(i)->at(j);	
		}
		cout << endl;
	}
	
	
    end = clock();

    cout << "result1 : " << result1 << endl;
    cout << "result2 : " << result2 << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;

    return 0;
}
