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
char xorStr(char a, char b);
vector <int> findIndeces(vector<int> data, int element);

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
            //found in dictionary
            int index = it - dictionary.begin();
            encodedInstruction = "101" + convertToBinary(index, 3);
        }
        else 
        {
            //not found in dictionary
            vector <string> possibleEncodes;
            for (int dictIndex = 0; dictIndex < dictionary.size();dictIndex++) {
                //int mismatchCount = 0;
                vector <int> mismatchLocations;

                //checking number of mismatches
                for (int addr=0; addr < 32; addr++) {
                    if (dictionary[dictIndex][addr]!=instructionSet[index][addr])//mismatch condition
                    {
                        //mismatchCount++;
                        mismatchLocations.push_back(addr);
                        if (addr<29)//cheking for 4 bitmask case
                        {
                            string prefixInst = instructionSet[index].substr(0, addr);
                            string misInst = instructionSet[index].substr(addr, 4);
                            string suffixInst = instructionSet[index].substr(addr + 4);

                            string prefixDict = dictionary[dictIndex].substr(0, addr);
                            string misDict = dictionary[dictIndex].substr(addr, 4);
                            string suffixDict = dictionary[dictIndex].substr(addr + 4);

                            string tempEncode;
                            if ((prefixInst==prefixDict) && (suffixInst==suffixDict))
                            {
                                string bitmask;
                                for (int i = 0; i < 4; i++) {
                                    bitmask = bitmask + xorStr(misInst[i], misDict[i]);
                                }
                                tempEncode = "001" + convertToBinary(addr, 5) + bitmask + convertToBinary(dictIndex, 3);
                                possibleEncodes.push_back(tempEncode);
                            }
                        }

                        if (addr!=31)//consecutive two bit mismatch
                        {
                            string prefixInst = instructionSet[index].substr(0, addr);
                            //string misInst = instructionSet[index].substr(addr, 2);
                            string suffixInst = instructionSet[index].substr(addr + 2);

                            string prefixDict = dictionary[dictIndex].substr(0, addr);
                            //string misDict = dictionary[dictIndex].substr(addr, 2);
                            string suffixDict = dictionary[dictIndex].substr(addr + 2);

                            string tempEncode;
                            if ((prefixInst == prefixDict) && (suffixInst == suffixDict))
                            {
                                tempEncode = "011" + convertToBinary(addr, 5) + convertToBinary(dictIndex, 3);
                                possibleEncodes.push_back(tempEncode);
                            }
                        }

                        //only one bit mismatch
                        string prefixInst = instructionSet[index].substr(0, addr);
                        //string misInst = instructionSet[index].substr(addr, 1);
                        string suffixInst = instructionSet[index].substr(addr + 1);

                        string prefixDict = dictionary[dictIndex].substr(0, addr);
                        //string misDict = dictionary[dictIndex].substr(addr, 1);
                        string suffixDict = dictionary[dictIndex].substr(addr + 1);

                        string tempEncode;
                        if ((prefixInst == prefixDict) && (suffixInst == suffixDict))
                        {
                            tempEncode = "010" + convertToBinary(addr, 5) + convertToBinary(dictIndex, 3);
                            possibleEncodes.push_back(tempEncode);
                        }


                    }//end of mismatch condition
                    
                }//end of loop iterating over 32 bits

                if (mismatchLocations.size() == 2) {
                    string tempEncode = "100" + convertToBinary(mismatchLocations[0], 5) + convertToBinary(mismatchLocations[1], 5) + convertToBinary(dictIndex, 3);
                    possibleEncodes.push_back(tempEncode);
                }

            }//end of loop iterating over dictionary

            if (possibleEncodes.size()==0)
            {//cannot compress in any form
                encodedInstruction = "110" + instructionSet[index];
            }
            else if (possibleEncodes.size() == 1) 
            {//only one compression form available
                encodedInstruction = possibleEncodes[0] + encodedInstruction;
            }
            else
            {//many forms of compression available
                vector <int> encodeLengths;
                for (string encodedCode : possibleEncodes) {
                    encodeLengths.push_back(encodedCode.size());
                }
                int minLength = *min_element(encodeLengths.begin(), encodeLengths.end());
                vector<int> minIndeces = findIndeces(encodeLengths, minLength);

                if (minIndeces.size()==1)
                {
                    encodedInstruction = possibleEncodes[minIndeces[0]];
                }
                else
                {

                }

                
            }

            //encodedInstruction = "    -1";
        }
        compressedInstructions.push_back(encodedInstruction);
        cout << index << "  original=" << instructionSet[index] << "  encoded=" << encodedInstruction << endl;

        //cheking for RLE
        int repetionRLE = 0;
        for (int temp = 1; temp < 5; temp++) {
            if (index + temp <= instructionSet.size() - 1) {
                if (instructionSet[index] == instructionSet[index + temp]) {
                    repetionRLE++;
                }
                else
                {
                    temp = 6;
                }
            }
            else
            {
                temp = 6;
            }
        }
        if (repetionRLE!=0)
        {
            string instructionRLE = "000" + convertToBinary(repetionRLE-1, 2);
            index = index + repetionRLE;
            compressedInstructions.push_back(instructionRLE);
            cout << "********************************************* RLE found = " << instructionRLE << endl;
        }
        
    }//end of instruction set iterating loop

    return compressedInstructions;
}//end of compressing function

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

char xorStr (char a, char b) {
    char result;
    if (a == '1')
    {
        if (b == '1')
        {
            result = '0';
        }
        else if (b == '0')
        {
            result = '1';
        }
    }
    else if (a == '0')
    {
        if (b == '1')
        {
            result = '1';
        }
        else if (b == '0')
        {
            result = '0';
        }
    }

    return result;
}

vector <int> findIndeces(vector<int> data, int element) {
    vector<int> indeces;
    for (int i = 0; i < data.size(); i++) {
        if (data[i]==element)
        {
            indeces.push_back(i);
        }
    }
    return indeces;
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
