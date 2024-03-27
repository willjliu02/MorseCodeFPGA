// include files

#include "../decoder_helpers/module.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../decoder_helpers/process.h"
using namespace std;

#define ZERO short('0')



// make a process test method that runs all the info in
void testProcess(char* inputName, char* goldenName){
    AXIStream inBit, outLetter;
    AXIVal in_tmp, out_tmp;
    char inputLine[100], outputLine[100], goldenLine[100];

    ifstream inputFile, outputFile, goldenFile;
    inputFile.open(inputName, ifstream::in);
    goldenFile.open(goldenName, ifstream::in);

    // gets all the values along the whitespace
    while (inputFile >> inputLine) {
        for (short i = 0; i < 100; i++) {
            if (inputLine[i] == '\0') {
                break;
            } 

            short inBitVal = short(inputLine[i]) - ZERO;

            in_tmp.data = inBitVal;
            in_tmp.keep = 1;
            in_tmp.strb = 1;
            in_tmp.user = 1;
            in_tmp.last = 0;

            in_tmp.id = 0;
            in_tmp.dest = 1;

            inBit.write(in_tmp);
        }
    } 

    processNextBit(inBit, outLetter);

    bool hasErrors = false;
    while (goldenFile >> goldenLine) {
        outLetter.read(out_tmp);

        for(short i = 0; i < 100; i++) {
            if (goldenLine[i] == '\0') {
                break;
            }

            char outLetter = out_tmp.data.to_char()
            if (outLetter != goldenLine[i]) {
                cout << "ERROR: results mismatch. Expected: " << goldenLine[i] << " Received: " outLetter <<  << endl;
                hasErrors = true;
            }
        }
    }

    if (!hasErrors) {
        cout << "Success: results match" << endl;
        return 0;
    }

    return 1;
}

int main() {
    char inputName[100] = "test1.txt";
    char goldenName[100] = "golden_out1.txt";
    testProcess(inputName, goldenName);

    return 0;
}

// have it call the test function each time there's a test file
// if the function is nonblocking then consider having a pointer to be set for when the operation is done