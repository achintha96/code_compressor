// On my honor, I have neither given nor received unauthorized aid on this assignment 
// Author 180534N;
// code_compressor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <algorithm>

using namespace std;

vector<string> readFile(string fileName);
vector<string> makeDictionary(vector <string> instructionSet);

int main()
{
    cout << "Code compressor \n";
    
    vector <string> Instructions = readFile("original.txt");
    vector <string> Dictionary = makeDictionary(Instructions);
    
    /*for (string i : Dictionary){
        cout << i << endl;
    }*/
     


    return 0;
}


vector<string> readFile(string fileName) {
    string line;
    vector <string> instructions;

    std::ifstream in_file;
    in_file.open(fileName);

    if (in_file.is_open()) {
        //cout << "File open successful" << endl;
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
        //cout << "File open failed" << endl;
    }

    return instructions;
}

vector<string> makeDictionary(vector <string> instructionSet) {
    vector <string> duplicates;
    vector <int> duplicateCount;
    int rep;
    vector <string> dictionary;
    int dictSize = 8;
    int index;
    int currentMax;

    for (string code : instructionSet)
    {
        rep = count(instructionSet.begin(), instructionSet.end(), code);
        if ( rep > 1)
        {
            if (count(duplicates.begin(),duplicates.end(),code)==0)
            {
                duplicates.push_back(code);
                duplicateCount.push_back(rep);
                //cout << code << " --> " << rep<<endl;
            }   
        }
    }

    if (duplicates.size() < 8) {
        dictSize = duplicates.size();
    }

    for (int i = 0; i < dictSize; i++)
    {
        currentMax = *max_element(duplicateCount.begin(), duplicateCount.end());
        index = find(duplicateCount.begin(), duplicateCount.end(), currentMax)-duplicateCount.begin();
        //cout << "max-" << currentMax << " index-" << index << endl;
        dictionary.push_back(duplicates[index]);
        duplicateCount[index] = -1;
    }

    /*for (string code : dictionary) {
        cout << code << "-->" << count(instructionSet.begin(), instructionSet.end(), code) << endl;
    }*/
    
    return dictionary;

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
