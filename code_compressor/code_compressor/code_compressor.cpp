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
vector<string> codeCompression(vector <string> instructionSet, vector <string> dictionary);
string convertToBinary(int num);
string convertToBinary(int num, int precision);

int main()
{
    cout << "Code compressor \n";
    
    vector <string> Instructions = readFile("original.txt");
    vector <string> Dictionary = makeDictionary(Instructions);
    vector <string> CompressedCode = codeCompression(Instructions, Dictionary);
    
    /*for (string i : CompressedCode){
        cout << i << endl;
    }
    cout << "\ncode length=" << CompressedCode.size() << endl;*/
     
    /*cout << " 9=" << convertToBinary(9, 5) << endl;
    cout << "10=" << convertToBinary(10, 6) << endl;
    cout << "16=" << convertToBinary(16, 9) << endl;
    cout << " 0=" << convertToBinary(0, 4) << endl;*/

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

vector<string> codeCompression(vector <string> instructionSet, vector <string> dictionary) {
    vector <string> compressedInstructions;
    string encodedInstruction;

    /*for (string code : instructionSet)
    {
        encodedInstruction = "";
        auto it = find(dictionary.begin(), dictionary.end(), code);
        if (it != dictionary.end())
        {
            int index = it - dictionary.begin();
            encodedInstruction = "101" + convertToBinary(index,3);
        }
        else {
            encodedInstruction = "    -1";
        }
        compressedInstructions.push_back(encodedInstruction);
        cout << i << "  original=" << code << "  encoded=" << encodedInstruction << endl;
        i++;
    }*/

    for (int index = 0; index < instructionSet.size(); index++)
    {
        encodedInstruction = "";
        auto it = find(dictionary.begin(), dictionary.end(), instructionSet[index]);
        if (it != dictionary.end())
        {
            int index = it - dictionary.begin();
            encodedInstruction = "101" + convertToBinary(index, 3);
        }
        else {
            encodedInstruction = "    -1";
        }
        compressedInstructions.push_back(encodedInstruction);
        cout << index << "  original=" << instructionSet[index] << "  encoded=" << encodedInstruction << endl;

        
    }

    return compressedInstructions;
}

string convertToBinary(int num, int precision) {
    //cout << num << " - ";
    string binaryNum = "";
    if (num==0)
    {
        binaryNum = "0";
    }
    else
    {
        while (num != 1)
        {
            if (num % 2 == 1) {
                binaryNum = "1" + binaryNum;
            }
            else
            {
                binaryNum = "0" + binaryNum;
            }
            num = num / 2;
        }
        binaryNum = "1" + binaryNum;
    }
    

    if (binaryNum.size()<precision)
    {
        //binaryNum = "0" * (precision - binaryNum.size()) + binaryNum;
        string prefix((precision - binaryNum.size()), '0');
        binaryNum = prefix + binaryNum;
    }
    //cout << binaryNum << endl;
    return binaryNum;
}

string convertToBinary(int num) {
    string output;
    if (num == 0) {
        output = "000";
    }
    else if (num == 1) {
        output = "001";
    }
    else if (num == 2) {
        output = "010";
    }
    else if (num == 3) {
        output = "011";
    }
    else if (num == 4) {
        output = "100";
    }
    else if (num == 5) {
        output = "101";
    }
    else if (num == 6) {
        output = "110";
    }
    else if (num == 7) {
        output = "111";
    }
    else
    {
        output = num;
    }

    return output;
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
