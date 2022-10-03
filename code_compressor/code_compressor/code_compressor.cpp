// On my honor, I have neither given nor received unauthorized aid on this assignment 
// Author 180534N;
// code_compressor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>

using namespace std;

vector<string> readFile(string fileName);
string* makeDictionary(vector <string> instructionSet);

int main()
{
    cout << "Code compressor \n";
    
    vector <string> Instructions = readFile("original.txt");
    vector <int> Repetition_count;
    vector <string> Dict ;


    
    /*for (string i : Instructions){
        cout << i << endl;
    }
    cout << Instructions.size();*/
     


    return 0;
}


vector<string> readFile(string fileName) {
    string line;
    vector <string> instructions;

    std::ifstream in_file;
    in_file.open(fileName);

    if (in_file.is_open()) {
        cout << "File open successful" << endl;
        while (!in_file.eof())
        {
            in_file >> line;
            instructions.push_back(line);
            //cout << line << endl;
        }
        in_file.close();
        
        
        /*for (string i : dict)
        {
            cout << i << endl;
        }
        cout << dict.size();*/
        
    }
    else {
        cout << "File open failed" << endl;
    }

    return instructions;
}

string* makeDictionary(vector <string> instructionSet) {
    
        vector<string> duplicate;

        // STL function to sort the array of string
        sort(instructionSet.begin(), instructionSet.end());

        for (int i = 1; i < instructionSet.size(); i++) {
            if (instructionSet[i - 1] == instructionSet[i]) {

                // STL function to push the duplicate
                // words in a new vector string
                if (duplicate.empty())
                    duplicate.push_back(instructionSet[i]);
                else if (instructionSet[i] != duplicate.back())
                    duplicate.push_back(instructionSet[i]);
            }
        }

        if (duplicate.size() == 0)
            cout << "No Duplicate words" << endl;
        else
            for (int i = 0; i < duplicate.size(); i++)
                cout << duplicate[i] << endl;
    

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
