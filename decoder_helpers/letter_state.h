/**
 * File: letter_state.h
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Transforms morse code signals into characters
*/

#ifndef LETTER_STATE_H

#define LETTER_STATE_H

#include "module.h"

const char* LETTERS = "ETIANMSURWDKGOHVF L PJBXCYZQ";
letter MAX_LETTER = 28;
letter CURRENT_LETTER = 0;

/**
 * Initializes the machine to run anew
*/
void initLetters();

/**
 * Shifts the state machine to the next letter
 * 
 * Params:
 * - isDash(short): is really some bit indiciating if the incoming symbol is a dot or dash
*/
void shiftLetter(bit isDash);

/**
 * Gets the letter that the current state machine is on
 * 
 * Returns:
 * The length 1 string that has the character
 * None, if CURRENT_LETTER is not in the range of the list of letters
*/
void getLetter(char* next_letter);

/**
 * Takes care of any leftover letters in the state machine
*/
void finalize(char* next_letter);

#endif