/* On my honor, I have neither given nor received unauthorized aid on this assignment */
// Author 180534N - Achintha Rathnayake;

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
char notStr(char a);
vector<int> findIndeces(vector<int> data, int element);
int binToDec(string bin);
vector<string> organizeOutput(vector<string> compressedCode, vector<string> dictionary);
vector<string> decode(vector<string> rawData);
void writeFile(string fileName, vector<string> data);

int main(int argc, char **argv)
{
    if(argc==2){
        if (argv[1][0]=='1')
        {
            //code comprssing part
            //cout << "Code compressor \n";
            vector<string> Instructions = readFile("original.txt");
            vector<string> Dictionary = makeDictionary(Instructions);
            vector<string> CompressedCode = codeCompression(Instructions, Dictionary);
            vector<string> Organizedcode = organizeOutput(CompressedCode, Dictionary);
            writeFile("cout.txt", Organizedcode);

        }
        else if (argv[1][0]=='2')
        {
            //code decompressing part
            //cout << "Code decompressor \n";

            vector<string> DecodedInst = decode(readFile("compressed.txt"));
            writeFile("dout.txt", DecodedInst);
        }
        //else
        //{
        //    cout<<"Invalid Input"<<endl;
        //}
    }
    //else
    //{
    //    cout<<"Invalid Input"<<endl;
    //}        

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
            else if (possibleEncodes.size() > 1)
            {//many forms of compression available
                vector <int> encodeLengths; //contains the length of each string in possible encodes
                for (string encodedCode : possibleEncodes) {
                    encodeLengths.push_back(encodedCode.size());
                }
                int minLength = *min_element(encodeLengths.begin(), encodeLengths.end());
                vector<int> minIndeces = findIndeces(encodeLengths, minLength); //contains the indeces of minimums in encodeLengths vector

                if (minIndeces.size()==1)
                {
                    encodedInstruction = possibleEncodes[minIndeces[0]];
                }
                else
                {
                    //vector<int> prefix;
                    int minVal = 10;
                    string finalEncode;
                    for (int ind : minIndeces)
                    {
                        int prefix = binToDec(possibleEncodes[ind].substr(0, 3));
                        if (prefix<minVal)
                        {
                            minVal = prefix;
                            finalEncode = possibleEncodes[ind];
                        }
                    }
                    encodedInstruction = finalEncode + encodedInstruction;
                }

                
            }

            //encodedInstruction = "    -1";
        }
        compressedInstructions.push_back(encodedInstruction);
        //cout << index << "  original=" << instructionSet[index] << "  encoded=" << encodedInstruction << endl;
        //cout << index << "  original=" << instructionSet[index] << "  encoded=" << compressedInstructions[compressedInstructions.size()-1] << endl;

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
            //cout << "********************************************* RLE found = " << instructionRLE << endl;
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

char notStr(char a) {
    char output;
    if (a=='1')
    {
        output = '0';
    }
    else if(a=='0')
    {
        output = '1';
    }
    return output;
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

int binToDec(string bin) {
    int dec = 0;
    int exponent = 1;
    for (int i = bin.size() - 1; i >= 0; i--) {
        if (bin[i] == '1') {
            dec = dec + (exponent);
        }
        exponent = exponent * 2;
    }
    return dec;
}

vector<string> organizeOutput(vector<string> compressedCode, vector<string> dictionary) {
    vector <string> organizedCode;
    string tempCode = "";

    //organizing compressed code 
    for (string code: compressedCode)
    {
        tempCode = tempCode + code;
        
        if (tempCode.size() > 31) 
        {
            organizedCode.push_back(tempCode.substr(0, 32));
            tempCode = tempCode.substr(32);
        }
        
    }
    string suffix((32 - tempCode.size()), '1');
    tempCode = tempCode + suffix;
    organizedCode.push_back(tempCode);

    //making seperation
    tempCode = "xxxx";
    organizedCode.push_back(tempCode);

    //writing dictionary
    for (string code : dictionary) {
        organizedCode.push_back(code);
    }

    //// to verify results
    //for (string k : organizedCode) {
    //    cout << k << endl;
    //}
    //cout << "\ncompleted compression successfully" << endl;

    return organizedCode;
}

vector<string> decode(vector<string> rawData) {
    vector<string> instructions;
    vector<string> dictionary;
    
    int sepIndex = find(rawData.begin(), rawData.end(), "xxxx") - rawData.begin();
    for (int index=sepIndex+1;index<rawData.size();index++)
    {
        dictionary.push_back(rawData[index]);
        //cout << rawData[index] << endl;
    }

    string temp = rawData[0];
    string identifier;
    for (int index = 1; index < sepIndex; index++)
    {
        temp = temp + rawData[index];
        while (temp.size()>32)
        {
            string inst;
            identifier = temp.substr(0, 3);
            temp = temp.substr(3);
            if (identifier=="000")
            {
                //0 - RLE
                int repetition = binToDec(temp.substr(0, 2)) + 1;
                temp = temp.substr(2);
                for (int i = 0; i<repetition;i++)
                {
                    instructions.push_back(instructions[instructions.size() - 1]);
                }
            }
            else if (identifier == "001")
            {
                //1 - 4bit mismatch
                int location = binToDec(temp.substr(0, 5));
                temp = temp.substr(5);
                
                string bitmask = temp.substr(0, 4);
                temp = temp.substr(4);

                int dictIndex = binToDec(temp.substr(0, 3));
                temp = temp.substr(3);

                inst = dictionary[dictIndex].substr(0, location);

                for (int j = 0; j < 4; j++) {
                    inst = inst + xorStr(bitmask[j], dictionary[dictIndex][location + j]);
                }

                inst = inst + dictionary[dictIndex].substr(location + 4);

                /*if (inst.size() != 32) {
                    cout << identifier << endl;
                }
                cout << dictionary[dictIndex]<<"-----"<<bitmask << endl;*/
            }
            else if (identifier == "010")
            {
                //2 - 1bit mismatch
                int location = binToDec(temp.substr(0, 5));
                temp = temp.substr(5);

                int dictIndex = binToDec(temp.substr(0, 3));
                temp = temp.substr(3);

                inst = dictionary[dictIndex].substr(0, location) + notStr(dictionary[dictIndex][location]) + dictionary[dictIndex].substr(location + 1);

                /*if (inst.size() != 32) {
                    cout << identifier << endl;
                }*/
            }
            else if (identifier == "011")
            {
                //3 - 2bit consecutive mismatch
                int location = binToDec(temp.substr(0, 5));
                temp = temp.substr(5);

                int dictIndex = binToDec(temp.substr(0, 3));
                temp = temp.substr(3);

                inst = dictionary[dictIndex].substr(0, location) + notStr(dictionary[dictIndex][location]) + notStr(dictionary[dictIndex][location + 1]) + dictionary[dictIndex].substr(location + 2);

                /*if (inst.size() != 32) {
                    cout << identifier << endl;
                }*/
            }
            else if (identifier == "100")
            {
                //4 - 2bit non-consecutive mismatch
                int location1 = binToDec(temp.substr(0, 5));
                temp = temp.substr(5);

                int location2 = binToDec(temp.substr(0, 5));
                temp = temp.substr(5);

                int dictIndex = binToDec(temp.substr(0, 3));
                temp = temp.substr(3);

                inst = dictionary[dictIndex].substr(0, location1) + notStr(dictionary[dictIndex][location1]) + dictionary[dictIndex].substr(location1 + 1, location2 - location1 - 1) + notStr(dictionary[dictIndex][location2]) + dictionary[dictIndex].substr(location2 + 1);
            
                /*if (inst.size() != 32) {
                    cout << identifier << endl;
                }*/
            }
            else if (identifier == "101")
            {
                //5 - dictionary hit
                int dictIndex = binToDec(temp.substr(0, 3));
                temp = temp.substr(3);
                inst = dictionary[dictIndex];

                /*if (inst.size() != 32) {
                    cout << identifier << endl;
                }*/
            }
            else if (identifier == "110")
            {
                //6 - Uncompressed
                inst = temp.substr(0, 32);
                temp = temp.substr(32);

                /*if (inst.size() != 32) {
                    cout << identifier << endl;
                }*/
            }

            if (identifier!="000") //To avoid RLE
            {
                instructions.push_back(inst);
            }
            
        }
    }

    while (temp.size() > 2)
    {
        string inst;
        identifier = temp.substr(0, 3);
        temp = temp.substr(3);
        if (identifier == "000")
        {
            //0 - RLE
            int repetition = binToDec(temp.substr(0, 2)) + 1;
            temp = temp.substr(2);
            for (int i = 0; i < repetition; i++)
            {
                instructions.push_back(instructions[instructions.size() - 1]);
            }
        }
        else if (identifier == "001")
        {
            //1 - 4bit mismatch
            int location = binToDec(temp.substr(0, 5));
            temp = temp.substr(5);

            string bitmask = temp.substr(0, 4);
            temp = temp.substr(4);

            int dictIndex = binToDec(temp.substr(0, 3));
            temp = temp.substr(3);

            inst = dictionary[dictIndex].substr(0, location);

            for (int j = 0; j < 4; j++) {
                inst = inst + xorStr(bitmask[0], dictionary[dictIndex][location + j]);
            }

            inst = inst + dictionary[dictIndex].substr(location + 4);
        }
        else if (identifier == "010")
        {
            //2 - 1bit mismatch
            int location = binToDec(temp.substr(0, 5));
            temp = temp.substr(5);

            int dictIndex = binToDec(temp.substr(0, 3));
            temp = temp.substr(3);

            inst = dictionary[dictIndex].substr(0, location) + notStr(dictionary[dictIndex][location]) + dictionary[dictIndex].substr(location + 1);

        }
        else if (identifier == "011")
        {
            //3 - 2bit consecutive mismatch
            int location = binToDec(temp.substr(0, 5));
            temp = temp.substr(5);

            int dictIndex = binToDec(temp.substr(0, 3));
            temp = temp.substr(3);

            inst = dictionary[dictIndex].substr(0, location) + notStr(dictionary[dictIndex][location]) + notStr(dictionary[dictIndex][location + 1]) + dictionary[dictIndex].substr(location + 2);

        }
        else if (identifier == "100")
        {
            //4 - 2bit non-consecutive mismatch
            int location1 = binToDec(temp.substr(0, 5));
            temp = temp.substr(5);

            int location2 = binToDec(temp.substr(0, 5));
            temp = temp.substr(5);

            int dictIndex = binToDec(temp.substr(0, 3));
            temp = temp.substr(3);

            inst = dictionary[dictIndex].substr(0, location1) + notStr(dictionary[dictIndex][location1]) + dictionary[dictIndex].substr(location1 + 1, location2 - location1 - 1) + notStr(dictionary[dictIndex][location2]) + dictionary[dictIndex].substr(location2 + 1);
        }
        else if (identifier == "101")
        {
            //5 - dictionary hit
            int dictIndex = binToDec(temp.substr(0, 3));
            temp = temp.substr(3);
            inst = dictionary[dictIndex];
        }
        else if (identifier == "110")
        {
            //6 - Uncompressed
            inst = temp.substr(0, 32);
            temp = temp.substr(32);
        }
        else if (identifier == "111")
        {
            temp = "1"; //to terminate from while looping condition
        }

        if (identifier != "000") //To avoid RLE
        {
            instructions.push_back(inst);
        }

    }

    //cout << "\ncompleted decompression successfully" << endl;
    return instructions;
}

void writeFile(string fileName, vector<string> data) {
    std::ofstream out_file(fileName);
    if (out_file)
    {
        for (string line : data)
        {
            out_file << line << std::endl;
        }
        out_file.close();
        //cout << "file writing succesfull" << endl;
    }
    //else
    //{
    //    cout << "file writing failed" << endl;
    //}
}
