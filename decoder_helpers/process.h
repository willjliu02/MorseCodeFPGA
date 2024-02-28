/**
 * File: process.h
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Takes the resolved bits and passes it to the translator
*/

#ifndef PROCESS_H

#define PROCESS_H

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

/**
 * Processes some parsed information and interacts with the "letter" state machine
 * 
 * Params:
 * - meaning (Meaning): some parsed info from the bitstream input
 * Returns:
 * Either a letter, a letter + a space, or None
*/
char process(Meaning meaning);

#endif