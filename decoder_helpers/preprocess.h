/**
 * File: preprocess.h
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Preps the series of bits to be turned into letters
*/

#ifndef PREPROCESS_H

#define PREPROCESS_H

#include "process.h"

short PREV_BIT = 0;
int NUM_OF_BITS = 0;

/**
 * Processes the next bit
 * 
 * Params:
 * - bit(int): either a 0 or 1 that is either an "on" or "off" signal from the sender
 * 
 * Returns:
 * A letter if we have reached the end of a letter/word, else None
*/
char processNextBit(int bit);

/**
 * Determines what the last string of bit B means
 * 
 * Returns:
 * The information as one of the discrete meanings above.
*/
Meaning parse_PrevInputs();

#endif