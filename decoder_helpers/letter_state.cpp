/**
 * File: letter_state.cpp
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Transforms morse code signals into characters
*/

#include "letter_state.h"

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