/**
 * File: beat_decoder.h
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Determines the length of a beat
*/

#ifndef BEAT_DECODER_H

#define BEAT_DECODER_H

#include "module.h"

// probably change since hardware is bad at floats
int BEAT_ERROR_RANGE = 3; // describes the 2^-BEAT_ERROR_RANGE error
int BEAT_DURATION = 1;
/**
 * Removes noise from the beat_duration
 * 
 * Params:
 * - beat_nums(float): the rough number of beat
*/
void removeNoise(beat *numOfbeat, beat *beat_dur);

/**
 * Adjusts the beat duration
 *  
 * Params:
 * - expNumBits (beat): the expecred number of bits
 * - recNumBits (beat): the actual received number of bits
*/
void adjustFactors(beat *expNumBits, beat *recNumBits);

#endif
