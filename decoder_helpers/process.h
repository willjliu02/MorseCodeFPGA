/**
 * File: process.h
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Takes the resolved bits and passes it to the translator
*/

#ifndef PROCESS_H

#define PROCESS_H

#include "module.h"

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

/**
 * Determines what the last string of bit B means
 * 
 * Returns:
 * The information as one of the discrete meanings above.
*/
void parsePrevInputs(Meaning *inputMeaning);

/**
 * Processes some parsed information and interacts with the "letter" state machine
 * 
 * Params:
 * - meaning (Meaning): some parsed info from the bitstream input
 * Returns:
 * Either a letter, a letter + a space, or None
*/
void process(Meaning meaning, char* ret_letter);

/**
 * Processes the next bit
 * 
 * Params:
 * - bit(int): either a 0 or 1 that is either an "on" or "off" signal from the sender
 * 
 * Returns:
 * A letter if we have reached the end of a letter/word, else None
*/
void processNextBit(bit bit, char* ret_letter);

#endif