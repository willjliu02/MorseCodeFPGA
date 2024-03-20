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
float BEAT_ERROR_RANGE = 0.15;
int BEAT_DURATION = 1;
/**
 * Removes noise from the beat_duration
 * 
 * Params:
 * - beat_nums(float): the rough number of beats
*/
void removeNoise(beats beatNums, beats *beat_dur);

/**
 * Adjusts the error range and the beat duration
*/
void adjustFactors();

#endif
