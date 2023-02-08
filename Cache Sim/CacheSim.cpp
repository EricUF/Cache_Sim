// CacheSim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;

void conversion(vector<vector<int>>& array, string address, int design, int replace, int set_amt, int lines_cache, int& count, int& missCount, int& hitCount);

void exampleReader(string file, vector<vector<int>>& array, int design, int replace, int set_amt, int lines_cache, int& missCount, int& hitCount) {
    ifstream infile;
    infile.open(file);
    string line;
    int count = 1;
    while (getline(infile, line))
    {
        auto start = line.find_first_of(" ,\n", 0);
        auto end = line.find_first_of(" ,\n", start + 1);

        line = line.substr(start + 1, end - start - 1);
        conversion(array, line, design, replace, set_amt, lines_cache, count, missCount, hitCount);
        count++;
    }

}

void readTraceFile(vector<vector<int>> & array, int design, int replace, int set_amt, int lines_cache, int& missCount, int& hitCount) {
    //Call in file to import
    //make this one function

    //while in the import loop

    //go through line by line
    int choice = 0;
    cout << "Choose a trace file to analyze: " << endl;
    cout << "1. gcc.trace" << endl;
    cout << "2. read01.trace" << endl;
    cout << "3. read02.trace" << endl;
    cout << "4. read03.trace" << endl;
    cout << "5. swim.trace" << endl;
    cout << "6. write01.trace" << endl;
    cout << "7. write02.trace" << endl;
    cin >> choice;

    ifstream infile;

    if (choice == 1) {
        infile.open("gcc.trace");
    }
    else if (choice == 2) {
        infile.open("read01.trace");
    }
    else if (choice == 3) {
        infile.open("read02.trace");
    }
    else if (choice == 4) {
        infile.open("read03.trace");
    }
    else if (choice == 5) {
        infile.open("swim.trace");
    }
    else if (choice == 6) {
        infile.open("write01.trace");
    }
    else if (choice == 7) {
        infile.open("write02.trace");
    }

    string line;
    int count = 1;
    while (getline(infile, line))
    {
        auto start = line.find_first_of(" ,\n", 0);
        auto end = line.find_first_of(" ,\n", start + 1);

        line = line.substr(start + 1, end - start - 1);
        conversion(array, line, design, replace, set_amt, lines_cache, count, missCount, hitCount);
        count++;
    }

    cout << "END OF FILE" << endl;
}

int binToDec(string number) {

    if (number.find('1') == std::string::npos) {
        return 0;
    }

    int i = 0;
    int length = 0;
    while (number.at(i) == '0') {
        i++;
        length++;
    }

    string line = number.substr(length, number.size() - length);
    int x = 1;
    int result = 0;

    for (int i = line.size() - 1; i >= 0; i--) {
        if (line[i] == '1') {
            result = result + x;
        }
        x = x * 2;
    }

    return result;
}

void DM(vector<vector<int>> & array, int tag, int line_number, int& missCount, int& hitCount) {
    //DM FUNCTION
    // 
    // HIT
    // ADD UP HIT COUNTER, MOVE NEXT LINE
    // 
    // MISS, CHECK TO SEE IF 0, 
    // IF 0, PLACE IT THERE
    // IF NOT 0, REPLACE IT

    //HIT CHECK
    if (array[line_number][2] == tag) {
        //cout << "HIT" << endl;
        hitCount++;
        return;
    }
    //MISS CHECK
    else {
        array[line_number][2] = tag;
        missCount++;
        return;
    }

}

void NW(vector<vector<int>>& array, int tag, int set, int replace, int set_amt, int lines_cache, int & count, int& missCount, int& hitCount) {
    //NW FUNCTION
    //COUNTER KEEPS TRACK OF WHEN ADDED
    //
    //HIT
    //ADD UP HIT COUNTER, MOVE NEXT LINE
    //
    // MISS, CHECK TO SEE IF SET HAS SPACE, 
    // IF SPACE, PLACE IT THERE
    // IF NO SPACE, CHECK LRU OR FIFO
    //REPLACE ACCORDINGLY

    //HIT CHECK
    //LRU
    if (replace == 1) {
        for (int i = 0; i < array.size(); i++) {
            if (array[i][1] == set) {
                if (array[i][2] == tag) {
                    //cout << "HIT" << endl;
                    for (int j = 0; j < array.size(); j++) {
                        if (array[j][3] != 0) {
                            array[j][3]++;
                        }
                    }
                    array[i][3] = 1;
                    hitCount++;
                    return;
                }
            }
        }
    }
    //FIFO
    else {
        for (int i = 0; i < array.size(); i++) {
            if (array[i][1] == set) {
                if (array[i][2] == tag) {
                    //cout << "HIT" << endl;                   
                    hitCount++;
                    return;
                }
            }
        }
    }
    

    //MISS CHECK
    int linesPerSet = lines_cache / set_amt;

    //if there is an empty line in the set
    //LRU
    if (replace == 1) {
        for (int i = 0; i < array.size(); i++) {
            if (array[i][1] == set) {
                if (array[i][2] == -1) {
                    //cout << "Count:  " << count << endl;
                    for (int j = 0; j < array.size(); j++) {
                        if (array[j][3] != 0) {
                            array[j][3]++;
                        }
                    }
                    array[i][2] = tag;
                    array[i][3] = 1;
                    missCount++;
                    //counter++;
                    return;
                }
            }
        }
    }
    //FIFO
    else {
        for (int i = 0; i < array.size(); i++) {
            if (array[i][1] == set) {
                if (array[i][2] == -1) {
                    //cout << "Count:  " << count << endl;
                    array[i][2] = tag;
                    array[i][3] = count;
                    missCount++;
                    //counter++;
                    return;
                }
            }
        }
    }
    

    //if the set is full
    //LRU
    if (replace == 1) {
        //everything is added with a count of 1
        //all counts increase by one
        //if address is hit, reset the count to 1
        //replace the biggest number then when called upon LRU miss
        
        int biggest = 0;
        int pos = 0;
        for (int i = 0; i < array.size(); i++) {
            if (array[i][1] == set) {
                if (array[i][3] > biggest) {
                    biggest = array[i][3];
                    pos = i;
                }
            }
        }
        //cout << "Count:  " << count << endl;
        for (int j = 0; j < array.size(); j++) {
            if (array[j][3] != 0) {
                array[j][3]++;
            }
        }
        array[pos][2] = tag;
        array[pos][3] = 1;
        missCount++;
        //counter++;
        return;
    }
    //FIFO
    else {
        int smallest = INT_MAX;
        int pos = 0;
        for (int i = 0; i < array.size(); i++) {
            if (array[i][1] == set) {
                if (array[i][3] < smallest) {
                    smallest = array[i][3];
                    pos = i;
                }
            }
        }
        //cout << "Count:  " << count << endl;
        array[pos][2] = tag;
        array[pos][3] = count;
        missCount++;
        return;
    }
    
}

void FA(vector<vector<int>>& array, int tag, int replace, int & count, int& missCount, int & hitCount) {
    //FA FUNCTION
    //COUNTER KEEPS TRACK OF WHEN ADDED
    //
    //HIT
    //ADD UP HIT COUNTER, MOVE NEXT LINE
    //
    // MISS, CHECK TO SEE IF THE CACHE IS FULL OR NOT
    // IF EMPTY, PLACE IT THERE
    // IF FULL, CHECK LRU OR FIFO
    //REPLACE ACCORDINGLY

    //CHECK ALL

    //HIT CHECK
    //LRU
    if (replace == 1) {
        for (int i = 0; i < array.size(); i++) {
            if (array[i][2] == tag) {
                //cout << "HIT" << endl;
                for (int j = 0; j < array.size(); j++) {
                    if (array[j][3] != 0) {
                        array[j][3]++;
                    }                       
                }
                array[i][3] = 1;
                hitCount++;
                return;
            }
        }
    }
    //FIFO
    else {
        for (int i = 0; i < array.size(); i++) {
            if (array[i][2] == tag) {
                //cout << "HIT" << endl;                   
                hitCount++;
                return;
            }
        }
    }
    
    //MISS CHECK
    //CHECK FOR EMPTY
    //not full
    //LRU
    if (replace == 1) {
        for (int i = 0; i < array.size(); i++) {
            //empty spot
            if (array[i][2] == -1) {
                for (int j = 0; j < array.size(); j++) {
                    if (array[j][3] != 0) {
                        array[j][3]++;
                    }
                }
                array[i][2] = tag;
                array[i][3] = 1;
                missCount++;
                //counter++;
                return;
            }
        }
    }
    //FIFO
    else {
        for (int i = 0; i < array.size(); i++) {
            //empty spot
            if (array[i][2] == -1) {
                array[i][2] = tag;
                array[i][3] = count;
                missCount++;
                //counter++;
                return;
            }
        }
    }
    
    //full
    //check replace method
    //LRU
    if (replace == 1) {
        int biggest = 0;
        int pos = 0;
        for (int i = 0; i < array.size(); i++) {
            if (array[i][3] > biggest) {
                biggest = array[i][3];
                pos = i;
            }
        }
        for (int j = 0; j < array.size(); j++) {
            if (array[j][3] != 0) {
                array[j][3]++;
            }
        }
        array[pos][2] = tag;
        array[pos][3] = 1;
        missCount++;
        //cout << "Miss for " << pos << " value: " << tag << " Miss Count: " << missCount << endl;
        return;
    }
    //FIFO
    else {
        int smallest = INT_MAX;
        int pos = 0;
        for (int i = 0; i < array.size(); i++) {
            if (array[i][3] < smallest) {
                smallest = array[i][3];
                pos = i;
            }
        }
        
        array[pos][2] = tag;
        array[pos][3] = count;
        missCount++;
        //cout << "Miss for " << pos << " value: " << tag << " Miss Count: " << missCount << endl;
        return;
    }
}

string hexToBin(string line) {
    string result;
    for (int i = 0; i < line.size(); i++) {
        if (line.at(i) == '0') {
            result.append("0000");
        }
        if (line.at(i) == '1') {
            result.append("0001");
        }
        if (line.at(i) == '2') {
            result.append("0010");
        }
        if (line.at(i) == '3') {
            result.append("0011");
        }
        if (line.at(i) == '4') {
            result.append("0100");
        }
        if (line.at(i) == '5') {
            result.append("0101");
        }
        if (line.at(i) == '6') {
            result.append("0110");
        }
        if (line.at(i) == '7') {
            result.append("0111");
        }
        if (line.at(i) == '8') {
            result.append("1000");
        }
        if (line.at(i) == '9') {
            result.append("1001");
        }
        if (line.at(i) == 'A' || line.at(i) == 'a') {
            result.append("1010");
        }
if (line.at(i) == 'B' || line.at(i) == 'b') {
    result.append("1011");
}
if (line.at(i) == 'C' || line.at(i) == 'c') {
    result.append("1100");
}
if (line.at(i) == 'D' || line.at(i) == 'd') {
    result.append("1101");
}
if (line.at(i) == 'E' || line.at(i) == 'e') {
    result.append("1110");
}
if (line.at(i) == 'F' || line.at(i) == 'f') {
    result.append("1111");
}
    }

    return result;
}

string decToBin(int dec) {
    int bin[32];
    int x = 0;
    while (dec > 0) {
        bin[x] = dec % 2;
        dec = dec / 2;
        x++;
    }
    string result;
    for (int i = x - 1; i >= 0; i--) {
        result.append(to_string(bin[i]));
    }
    return result;
}

void conversion(vector<vector<int>>& array, string address, int design, int replace, int set_amt, int lines_cache, int& count, int& missCount, int& hitCount) {
    //SET THIS AS A CONVERSION FUNCTION THAT TAKES IN THE DESIGN
    //convert hex to bin
    //set up parameters based on design number
    string line = address;
    long int num = 0;

    //if first two letters are 0x, substring
    if (line.substr(0, 2) == "0x") {
        line = line.substr(2, line.size() - 1);
    }

    string binary_address = hexToBin(line);
    int tag = 0;
    int line_number = 0;

    //cout << binary_address << endl;
    string binTotal = decToBin(lines_cache - 1);
    int binSize = binTotal.size();
    //cout << binSize << endl;

    //Direct Mapped
    if (design == 1) {
        string temp = binary_address.substr(0, 26 - binSize);
        tag = binToDec(temp);
        //cout << "Tag: " << tag << endl;

        string temp1 = binary_address.substr(26 - binSize, binSize);
        line_number = binToDec(temp1);
        //cout << "Line Number: " << line_number << endl;

        DM(array, tag, line_number, missCount, hitCount);
    }
    //N Way Associative
    else if (design == 2) {
        int lineBits = log2(set_amt);
        int offBits = 6;
        int tagBits = 32 - lineBits - offBits;

        //cout << lineBits << " " << offBits << " " << tagBits << endl;

        string temp = binary_address.substr(0, tagBits);
        tag = binToDec(temp);
        //cout << "Tag: " << tag << endl;

        string temp1 = binary_address.substr(tagBits, lineBits);
        line_number = binToDec(temp1);
        //cout << "Set: " << line_number << endl;

        NW(array, tag, line_number, replace, set_amt, lines_cache, count, missCount, hitCount);
    }
    //Fully Associative
    else if (design == 3) {
        int tagBits = 32 - 6;

        string temp = binary_address.substr(0, tagBits);
        tag = binToDec(temp);
        //cout << "Tag: " << tag << endl;

        FA(array, tag, replace, count, missCount, hitCount);
    }
}

void dataExamples(){

    int choice = 0;
    cout << "Choose a trace file to analyze: " << endl;
    cout << "1. gcc.trace" << endl;
    cout << "2. read01.trace" << endl;
    cout << "3. read02.trace" << endl;
    cout << "4. read03.trace" << endl;
    cout << "5. swim.trace" << endl;
    cout << "6. write01.trace" << endl;
    cout << "7. write02.trace" << endl;
    cin >> choice;

    ifstream infile;
    string file;
    if (choice == 1) {
        file = ("gcc.trace");
    }
    else if (choice == 2) {
        file = ("read01.trace");
    }
    else if (choice == 3) {
        file = ("read02.trace");
    }
    else if (choice == 4) {
        file = ("read03.trace");
    }
    else if (choice == 5) {
        file = ("swim.trace");
    }
    else if (choice == 6) {
        file = ("write01.trace");
    }
    else if (choice == 7) {
        file = ("write02.trace");
    }

    cout << "Cache Examples (64 bit)" << endl;
    cout << "Cache Size: [512, 1024, 2048, 4096, 8192, 16384]" << endl;
    int counter = -1;

    int design = 0;
    int num_bytes = 0;
    
    int num_blocks = 0;
    int replace = 0;
    for (int i = 0; i < 54; i++) {
        counter++;
        int amt_bytes = 64;


            if (i == 0) {
                cout << "Direct Mapped Cache: [";
                design = 1;
            }
            else if (i == 6) {
                cout << "LRU Fully Associated: [";
                design = 3;
                replace = 1;
            }
            else if (i == 12) {
                cout << "FIFO Fully Associated: [";
                design = 3;
                replace = 2;
            }
            else if (i == 18) {
                cout << "LRU 2 Way Associated: [";
                design = 2;
                num_blocks = 2;
                replace = 1;
            }
            else if (i == 24) {
                cout << "FIFO 2 Way Associated: [";
                design = 2;
                num_blocks = 2;
                replace = 2;
            }
            else if (i == 30) {
                cout << "LRU 4 Way Associated: [";
                design = 2;
                num_blocks = 4;
                replace = 1;
            }
            else if (i == 36) {
                cout << "FIFO 4 Way Associated: [";
                design = 2;
                num_blocks = 4;
                replace = 2;
            }
            else if (i == 42) {
                cout << "LRU 8 Way Associated: [";
                design = 2;
                num_blocks = 8;
                replace = 1;
            }
            else if (i == 48) {
                cout << "FIFO 8 Way Associated: [";
                design = 2;
                num_blocks = 8;
                replace = 2;
            }
        

        if (counter == 0) {
            num_bytes = 512;
        }
        else if (counter == 1) {
            num_bytes = 1024;
        }
        else if (counter == 2) {
            num_bytes = 2048;
        }
        else if (counter == 3) {
            num_bytes = 4096;
        }
        else if (counter == 4) {
            num_bytes = 8192;
        }
        else if (counter == 5) {
            counter = -1;
            num_bytes = 16384;
        }

        int lines_cache = num_bytes / amt_bytes;
        int set_amt = 1;
        vector<vector<int>> array;

        //DM
        if (design == 1) {
            for (int i = 0; i < lines_cache; i++) {
                array.push_back({ i, i, -1, 0 });
            }
        }
        //N WAY
        else if (design == 2) {
            set_amt = lines_cache / num_blocks;

            int a = -1;
            int b = 0;
            for (int i = 0; i < lines_cache; i++) {
                a++;
                if (a % num_blocks == 0 && a != 0) {
                    b++;
                    array.push_back({ i, b, -1, 0 });
                }
                else {
                    array.push_back({ i, b, -1, 0 });
                }
            }
        }
        //FA
        else if (design == 3) {
            for (int i = 0; i < lines_cache; i++) {
                array.push_back({ i, 0, -1, 0 });
            }
        }
        //NOW MAIN PROGRAM BEGINS, READ TRACE FILES
        int missCount = 0;
        int hitCount = 0;
        exampleReader(file, array, design, replace, set_amt, lines_cache, missCount, hitCount);

        //EXIT THE MAIN PROGRAM FUNCTION

        //PRINT OUT THE HIT AND MISS RATE WITH DATA
        float hits = (float)hitCount;
        float miss = (float)missCount;

        //cout << "Hit: " << hits << " Miss: " << miss << endl;

        float hitRate = hits / (hits + miss);
        //cout << "Hit rate for cache is: " << hitRate << endl;

        if (i == 5 || i == 11 || i == 17 || i == 23 || i == 29 || i == 35 || i == 41 || i == 47 || i == 53) {
            cout << hitRate << "]" << endl;
        }
        else {
            cout << hitRate << ", ";
        }

        array.clear();
    }

}

int main()
{
    cout << "Welcome to the Cache Sim." << endl;

    //SET PARAMETERS
    cout << "Select the Cache Design: " << endl;
    cout << "1. Direct Mapped" << endl;
    cout << "2. N-Way Associative" << endl;
    cout << "3. Fully Associative" << endl;
    cout << "4. Data Examples" << endl;
    int design;
    cin >> design;

    if (design == 4) {
        dataExamples();
    }
    else {

        int replace = 0;
        if (design == 2 || design == 3) {
            cout << "Choose the replacement method: " << endl;
            cout << "1. LRU (Least Recently Used)" << endl;
            cout << "2. FIFO (First In First Out)" << endl;
            cin >> replace;
        }

        cout << "Input the amount of bytes in the cache: " << endl;
        int num_bytes;
        cin >> num_bytes;

        cout << "Input the amount of bytes in a block: " << endl;
        int amt_bytes;
        cin >> amt_bytes;

        int num_blocks = 0;
        if (design == 2) {
            cout << "Input the amount of blocks in a set: " << endl;
            cin >> num_blocks;
        }

        int lines_cache = num_bytes / amt_bytes;
        int set_amt = 1;
        vector<vector<int>> array;

        //DM
        if (design == 1) {
            for (int i = 0; i < lines_cache; i++) {
                array.push_back({ i, i, -1, 0 });
            }
        }
        //N WAY
        else if (design == 2) {
            set_amt = lines_cache / num_blocks;

            int a = -1;
            int b = 0;
            for (int i = 0; i < lines_cache; i++) {
                a++;
                if (a % num_blocks == 0 && a != 0) {
                    b++;
                    array.push_back({ i, b, -1, 0 });
                }
                else {
                    array.push_back({ i, b, -1, 0 });
                }
            }
        }
        //FA
        else if (design == 3) {
            for (int i = 0; i < lines_cache; i++) {
                array.push_back({ i, 0, -1, 0 });
            }
        }
        //NOW MAIN PROGRAM BEGINS, READ TRACE FILES
        int missCount = 0;
        int hitCount = 0;
        readTraceFile(array, design, replace, set_amt, lines_cache, missCount, hitCount);

        for (int i = 0; i < array.size(); i++) {
            for (int j = 0; j < array[i].size(); j++)
                cout << array[i][j] << " ";
            cout << endl;
        }
        //EXIT THE MAIN PROGRAM FUNCTION

        //PRINT OUT THE HIT AND MISS RATE WITH DATA
        float hits = (float)hitCount;
        float miss = (float)missCount;

        cout << "Hit: " << hits << " Miss: " << miss << endl;

        float hitRate = hits / (hits + miss);
        cout << "Hit rate for cache is: " << hitRate << endl;
    }
}