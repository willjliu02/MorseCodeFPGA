/**
 * File: process_hw.cpp
 * Created By: William Liu
 * Date: 3/27/24
 * Description: Processes a sequence of bits and turns it into a string
*/

// #include <ap_axi_sdata.h>
// #include <hls_stream.h>

#include <iostream>

typedef short bit;
typedef short beat;
typedef short letter;
typedef char* retPtr;
// typedef  AXIVal;
// typedef hls::stream<ap_axis<32,2,5,6>> AXIStream;
typedef char* AXIVal;
typedef char* AXIStream;

enum Meaning{
    // Symbol
    DOT = 0,
    DASH = 1,

    // Pauses
    NEXT_SYMBOL,
    NEXT_LETTER,
    NEXT_WORD,

    // Unknown
    UNKNOWN = -1,

};

bit PREV_BIT = 0;
int NUM_OF_BITS = 0;
const char* LETTERS = "ETIANMSURWDKGOHVF L PJBXCYZQ";
letter MAX_LETTER = 28;
letter CURRENT_LETTER = 0;
int BEAT_ERROR_RANGE = 3; // describes the 2^-BEAT_ERROR_RANGE error
int BEAT_DURATION = 1;

/* ----------------------------- LETTER STATE ----------------------------- */
void initLetters() {
    CURRENT_LETTER = 0;
}

void shiftLetter(bit isDash) {
    CURRENT_LETTER = CURRENT_LETTER * 2 + isDash;
}

void getLetter(char* next_letter) {
    char ret_letter;
    if (CURRENT_LETTER > 0 && CURRENT_LETTER <= MAX_LETTER) {
        ret_letter = LETTERS[CURRENT_LETTER - 1];
    } else {
        ret_letter = *"";
    }

    CURRENT_LETTER = 0;

    next_letter = &ret_letter;
}

void finalize(char* next_letter) {
    getLetter(next_letter);
}

/* ----------------------------- BEAT DECODER ----------------------------- */
void adjustFactors(beat expNumBits, beat recNumBits) {
    // goal: adjust the beat duration to better include the beats

    // calculates the difference between the received and expected bits
    // divides that by 4, so that we get some shift in the right direction
    // then uses this number to adjust the beat duration
    BEAT_DURATION += (expNumBits - recNumBits) >> 2;
}

beat removeNoise(beat numBits) {
    // use bit shifting to the right to do the math
    // x >> 3 = 12.5% of x
    beat numBeats[3] = {1, 3, 7};
    
    for (int i = 0; i < 3; ++i) {
        beat center = numBeats[i] * BEAT_DURATION;
        beat bitError = center >> BEAT_ERROR_RANGE;

        beat lowRange = center - bitError;
        beat highRange;

        if (i < 2) {
            highRange = 2 * center;
        } else {
            highRange = center + bitError;
        }
        

        if (numBits < highRange && numBits >= lowRange) {
            adjustFactors(center, numBits);
            return numBeats[i];
        }
    }

    return -1;
}


/* ------------------------------- PROCESS -------------------------------- */

Meaning parsePrevInputs() {
    // TODO: consider changing this from division to multiple or something else entirely
    // TODO: idea: multiplication in removeNoise (mulitply by beat values)
    beat beat_dur = removeNoise(NUM_OF_BITS);

    Meaning meaning;

    if (PREV_BIT) {
        switch (beat_dur)
        {
            case 1:
                meaning = Meaning::DOT;
                break;
            case 3:
                meaning = Meaning::DASH;
                break;
            default:
                meaning = Meaning::UNKNOWN;
                break;
        }
    } else {
        switch (beat_dur)
        {
            case 1:
                meaning = Meaning::NEXT_SYMBOL;
                break;
            case 3:
                meaning = Meaning::NEXT_LETTER;
                break;
            case 7:
                meaning = Meaning::NEXT_WORD;
                break;
            default:
                meaning = Meaning::UNKNOWN;
                break;
        }
    }

    return meaning;
}

void process(Meaning meaning, char* ret_letter) {
    char ret_let[3] = "  ";
    char *tmp_letter;

    switch(meaning){
        case Meaning::DOT:
            shiftLetter(0);
            ret_let[0] = '\0';
            break;
        case Meaning::DASH:
            shiftLetter(1);
            ret_let[0] = '\0';
            break;
        case Meaning::NEXT_LETTER:
            getLetter(tmp_letter);
            ret_let[0] = *tmp_letter;
            ret_let[1] = '\0';
            break;
        case Meaning::NEXT_WORD:
            getLetter(tmp_letter);
            ret_let[0] = *tmp_letter;
            break;
        default:
            break;
    }

    ret_letter = ret_let;
}

// void processNextBit(hls::stream<ap_axis<32,2,5,6>>& inBit, hls::stream<ap_axis<32,2,5,6>>& outLetter) {
// #pragma HLS INTERFACE axis port=inBit
// #pragma HLS INTERFACE axis port=outLetter
// #pragma HLS INTERFACE s_axilite port=return

//     retPtr tmpRet = nullptr;
//     ap_axis<32,2,5,6> tmp;

//     do {
//         inBit.read(tmp);

//         bit bitVal = tmp.data.to_int();

//         if (bitVal == PREV_BIT) {
//             ++NUM_OF_BITS;
//         } else {
//             Meaning meaning;
//             parsePrevInputs(meaning);

//             process(meaning, tmpRet);

//             PREV_BIT = bitVal;
//             NUM_OF_BITS = 1;

//             tmp.data = tmpRet;

//             outLetter.write(tmp);
//         }
//     } while (!tmp.last);    
// }
int main() {
    bit bytes[100] = {1, 0, 1, 1, 1, 0, 0, 0};
    char letters[100];

    for (int i = 0; i < 8; ++i) {
        bit bitVal = bytes[i];

        if (bitVal == PREV_BIT) {
            ++NUM_OF_BITS;
        } else {
            Meaning meaning = parsePrevInputs();
            
            std::cout << meaning << std::endl;

            process(meaning, letters);

            PREV_BIT = bitVal;
            NUM_OF_BITS = 1;
        }
    }

    finalize(letters);


    std::cout << letters[0] << std::endl;
}