/**
 * File: process.cpp
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Takes the resolved bits and passes it to the translator
*/

#include "process.h"
#include "letter_state.h"
#include "beat_decoder.h"
#include <string.h>

void parsePrevInputs(Meaning inputMeaning) {
    // TODO: consider changing this from division to multiple or something else entirely
    // TODO: idea: multiplication in removeNoise (mulitply by beat values)
    beat beat_dur;

    // TODO: fix to match beat_decoder
    removeNoise(NUM_OF_BITS, beat_dur);

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

    inputMeaning = meaning;
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
    }

    ret_letter = ret_let;
}

void processNextBit(AXIStream& inBit, AXIStream& outLetter) {
#pragma HLS INTERFACE axis port=inBit
#pragma HLS INTERFACE axis port=outLetter
#pragma HLS INTERFACE s_axilite port=return

    retPtr tmpRet = NULL;
    AXIVal tmp;

    do {
        inBit.read(tmp);

        bit bitVal = tmp.data.to_int();

        if (bitVal == PREV_BIT) {
            ++NUM_OF_BITS;
        } else {
            Meaning meaning;
            parsePrevInputs(meaning);

            process(meaning, tmpRet);

            PREV_BIT = bitVal;
            NUM_OF_BITS = 1;

            tmp.data = tmpRet;

            outLetter.write(tmp);
        }
    } while (!tmp.last);    
}