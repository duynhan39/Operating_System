#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void removeAllComments(ifstream &in, ofstream &out);

int main(int argc, char *argv[]) {
    if(argc <= 3) {
        cout << "Usage: " <<argv[0]<<"<-all> <input file> <outfile>" <<endl;
        exit(1);
    }
    
    // Open files
    ifstream inFile(argv[2]);
    ofstream outFile(argv[3]);
    
    //Check for successful opens
    if(!inFile) {
        cout<<"Error opening input file " << argv[2] <<endl;
        exit(1);
    }
    
    if(!outFile) {
        cout<<"Error opening output file " << argv[3] <<endl;
        exit(1);
    }
    
    string option = argv[1];
    if(option == "-all") {
        removeAllComments(inFile, outFile);
    }
    
    
    
    inFile.close();
    outFile.close();
    
    
    return 0;
}

void removeAllComments(ifstream &in, ofstream &out) {
    string line;
    while(getline(in, line)) {
        istringstream ss(line);
        string word;
        ss>>word;
        if(word.substr(0, 2) != "//") {
            out<<line<<endl;
        }
    }
    
    
}



