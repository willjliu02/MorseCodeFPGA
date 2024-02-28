/**
 * File: letter_state.h
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Transforms morse code signals into characters
*/

#ifndef LETTER_STATE_H

#define LETTER_STATE_H

char* LETTERS = "ETIANMSURWDKGOHVF L PJBXCYZQ";

/**
 * Initializes the machine to run anew
*/
void initLetters();

/**
 * Shifts the state machine to the next letter
 * 
 * Params:
 * - isDash(bool): is really some bit indiciating if the incoming symbol is a dot or dash
*/
void shiftLetter(bool isDash);

/**
 * Gets the letter that the current state machine is on
 * 
 * Returns:
 * The length 1 string that has the character
 * None, if CURRENT_LETTER is not in the range of the list of letters
*/
char getLetter();

/**
 * Takes care of any leftover letters in the state machine
*/
char finalize();

#endif