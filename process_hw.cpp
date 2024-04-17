/**
 * File: process_hw.cpp
 * Created By: William Liu
 * Date: 3/27/24
 * Description: Processes a sequence of bits and turns it into a string
*/

#include <ap_axi_sdata.h>
#include <hls_stream.h>
 #include <iostream>
/* ------------------------------------------------------------------------ */
/* -------------------------------- MODULE -------------------------------- */
/* ------------------------------------------------------------------------ */
typedef ap_uint<1> bit;
typedef short beat;
typedef ap_uint<5> letter;
// TODO: maybe consider making 2 different axi_val values to have a 1 bit input stream and a smaller than 32 output stream
typedef ap_axis<32,1,1,1> IN_BIT;
typedef ap_axis<32,1,1,1> OUT_LETTER;

enum Meaning{
    // Symbol
    DOT = 0,
    DASH = 1,

    // Pauses
    NEXT_SYMBOL,
    NEXT_LETTER,
    NEXT_WORD,
	FINALIZE,

    // Unknown
    UNKNOWN = -1,

};

bit PREV_BIT = 0;
int NUM_OF_BITS = 0;
// const char* LETTERS = "ETIANMSURWDKGOHVF L PJBXCYZQ";
// const letter LETTERS[MAX_LETTER] = {5, 20, 9, 1, 14, 13, 19, 21, 18, 23, 4, 11, 7, 15, 8, 22, 6, 31, 12, 31, 16, 10, 2, 24, 3, 25, 26, 17} 
letter MAX_LETTER = 28;
// 31 is a space
letter CURRENT_LETTER = 0;
int BEAT_ERROR_RANGE = 3; // describes the 2^-BEAT_ERROR_RANGE error
int BEAT_DURATION = 1;

/* ------------------------------------------------------------------------ */
/* ----------------------------- LETTER STATE ----------------------------- */
/* ------------------------------------------------------------------------ */
void initLetters() {
    CURRENT_LETTER = 0;
}

void shiftLetter(bit isDash) {
    CURRENT_LETTER = CURRENT_LETTER * 2 + isDash + 1;
}

char getLetter(letter *letters) {
    char ret_letter;
    if (CURRENT_LETTER > 0 && CURRENT_LETTER <= MAX_LETTER) {
        ret_letter = letters[CURRENT_LETTER - 1];
    } else {
        ret_letter = *"";
    }

    initLetters();

    return ret_letter;
}

char finalize(letter *letters) {
    return getLetter(letters);
}

/* ------------------------------------------------------------------------ */
/* ----------------------------- BEAT DECODER ----------------------------- */
/* ------------------------------------------------------------------------ */
void adjustFactors(beat expNumBits, beat recNumBits) {
    // goal: adjust the beat duration to better include the beats

    // calculates the difference between the received and expected bits
    // divides that by 4, so that we get some shift in the right direction
    // then uses this number to adjust the beat duration
	short tmp = (BEAT_DURATION + (expNumBits - recNumBits)) >> 2;

    BEAT_DURATION = tmp;
}

beat removeNoise() {
    // use bit shifting to the right to do the math
    // x >> 3 = 12.5% of x
    beat numBeats[3] = {1, 3, 7};
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=numBeats
    
    for (int i = 0; i < 3; ++i) {
#pragma HLS UNROLL
        beat center = numBeats[i] * BEAT_DURATION;
        beat bitError = center >> BEAT_ERROR_RANGE;
        beat highRange;

        if (i < 2) {
            highRange = 2 * center;
        } else {
            highRange = center + bitError;
        }
        
        beat lowRange = center - bitError;
        if (NUM_OF_BITS < highRange) {
        	if (NUM_OF_BITS >= lowRange) {
        		adjustFactors(center, NUM_OF_BITS);
				return numBeats[i];
        	}
        }
    }

    return -1;
}

/* ------------------------------------------------------------------------ */
/* ------------------------------- PROCESS -------------------------------- */
/* ------------------------------------------------------------------------ */
Meaning parsePrevInputs(bit isLast) {
    // TODO: consider changing this from division to multiple or something else entirely
    // TODO: idea: multiplication in removeNoise (multiply by beat values)
    beat beat_dur = removeNoise();

    Meaning meaning;

    if (isLast) {
    	meaning = Meaning::FINALIZE;
    } else if (PREV_BIT) {
    	if (beat_dur == 1) {
    		meaning = Meaning::DOT;
    	} else if (beat_dur == 3){
    		meaning = Meaning::DASH;
    	} else {
    		meaning = Meaning::UNKNOWN;
    	}
    } else {
    	if (beat_dur == 1) {
			meaning = Meaning::NEXT_SYMBOL;
		} else if (beat_dur == 3){
			meaning = Meaning::NEXT_LETTER;
		} else if (beat_dur == 7) {
			meaning = Meaning::NEXT_WORD;
		} else {
			meaning = Meaning::UNKNOWN;
		}
    }

    return meaning;
}

// TODO: when returned the value is now some value offset from chr('a') 
void processNextBit(hls::stream<IN_BIT>& inBit, letter *letters, hls::stream<OUT_LETTER>& outLetter) {
#pragma HLS TOP name=processNextBit
#pragma HLS INTERFACE axis port=inBit
#pragma HLS INTERFACE axis port=outLetter
#pragma HLS INTERFACE m_axi depth=28 port=letters
#pragma HLS INTERFACE ap_ctrl_none port=return

	IN_BIT input;
	OUT_LETTER output;

    // TODO: if LETTERS returns some value (0-25) wihtin the alphabet, then remove all CHARS

    while (true) {
#pragma HLS PIPELINE II=3
// TODO: consider adding some "trailing zeros" to be interpreted as the "end" and to finalize
    	inBit.read(input);
//    	if (inBit.read_nb(input)) {
		bit bitVal = (bit)input.data;

//		output.data = bitVal;
//		output.keep = input.keep;
//		output.strb = input.strb;
//		output.last = input.last;
//		output.dest = input.dest;
//		output.id = input.id;
//		output.user = input.user;
//
//		outLetter.write(output);
		if (input.last == 0 && bitVal == PREV_BIT) {
			++NUM_OF_BITS;
		} else {
			Meaning meaning = parsePrevInputs(input.last);

			char tmp_letter[3] = "  ";
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tmp_letter

			switch(meaning){
				case Meaning::DOT:
					shiftLetter(0);
					tmp_letter[0] = '\0';
					break;
				case Meaning::DASH:
					shiftLetter(1);
					tmp_letter[0] = '\0';
					break;
				case Meaning::NEXT_LETTER:
					tmp_letter[0] = getLetter(letters);
					tmp_letter[1] = '\0';
					break;
				case Meaning::NEXT_WORD:
					tmp_letter[0] = getLetter(letters);
					break;
				case Meaning::FINALIZE:
					shiftLetter(bitVal);
					tmp_letter[0] = getLetter(letters);
					break;
				default:
					tmp_letter[0] = '\0';
			}

			PREV_BIT = bitVal;
			NUM_OF_BITS = 1;

			output.keep = input.keep;
			output.strb = input.strb;
			output.dest = input.dest;
			output.id = input.id;
			output.user = input.user;
			output.last = 0;
			for (int i = 0; i < 2 && tmp_letter[i] != '\0'; ++i) {
				output.data = tmp_letter[i];
				outLetter.write(output);
			}
//		}
		}
    	if (input.last) {
    		break;
    	}
    }
    output.last = 1;
	outLetter.write(output);
}
