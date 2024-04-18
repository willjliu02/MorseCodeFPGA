// include files

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>

#include <ap_axi_sdata.h>
#include <hls_stream.h>

using namespace std;

#define ZERO short('0')
#define TO_LOW (short('a') - short('A'))
typedef ap_uint<1> bit;
typedef short beat;
typedef ap_uint<5> letter;
typedef ap_axis<32,1,1,1> IN_BIT;
typedef ap_axis<32,1,1,1> OUT_LETTER;

//letter MAX_LETTER = 28;

letter LETTERS[] = {5, 20, 9, 1, 14, 13, 19, 21, 18, 23, 4, 11, 7, 15, 8, 22, 6, 32, 12, 32, 16, 10, 2, 24, 3, 25, 26, 17};

void processNextBit(hls::stream<IN_BIT>& inBit, letter *letters, hls::stream<OUT_LETTER>& outLetter);

// make a process test method that runs all the info in
int testProcess(char* inputName, char* goldenName){
	hls::stream<IN_BIT> inBit;
	hls::stream<OUT_LETTER> outLetter;
	IN_BIT in_tmp;
	OUT_LETTER out_tmp;
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

            bit inBitVal;
            if (short(inputLine[i]) - ZERO == 0) {
            	inBitVal = 0;
            } else {
            	inBitVal = 1;
            }

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

    in_tmp.data = (bit)1;
    in_tmp.last = 1;
    inBit.write(in_tmp);

    processNextBit(inBit, LETTERS, outLetter);

    bool hasErrors = false;

    while(!(goldenFile >> goldenLine)) {
    	short i = 0;
    	while(i < 100 && goldenLine[i] != '\0') {
    		if (outLetter.read_nb(out_tmp)) {
    			int data = out_tmp.data.to_int();

				char outLetter;
				if (data == 0) {
					outLetter = ' ';
				} else {
					outLetter = (char)(data + TO_LOW - 1);
				}

				cout << out_tmp.data.to_int() + TO_LOW << endl;
				if (outLetter != goldenLine[i]) {
					cout << "ERROR: results mismatch. Expected: " << goldenLine[i] << " Received: " << outLetter << endl;
					hasErrors = true;
				}

				i++;
    		}
    	}
    }

    if (!hasErrors) {
        cout << "Success: results match" << endl;
        return 0;
    }

    return 1;
}

void testHeader(short testNum) {
	cout << "------------------------------------------------------------------------------------------" << endl;
	cout << "----------------------------------------- Test " << testNum << " -----------------------------------------" << endl;
	cout << "------------------------------------------------------------------------------------------" << endl;
}

int test1(){
	testHeader(1);
	return testProcess("test1.txt", "golden_out1.txt");
}

int test2() {
	testHeader(2);
	return testProcess("test2.txt", "golden_out2_3.txt");
}

int test3() {
	testHeader(3);
	return testProcess("test3.txt", "golden_out2_3.txt");
}

int test4() {
	testHeader(4);
	return testProcess("test4.txt", "golden_out4.txt");
}

int main() {
	test1();
	test2();
	test3();
	test4();

    return 0;
}

// have it call the test function each time there's a test file
// if the function is nonblocking then consider having a pointer to be set for when the operation is done
