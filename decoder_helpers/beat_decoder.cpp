/**
 * File: beat_decoder.cpp
 * Created By: William Liu
 * Date: 2/27/24
 * Description: Determines the length of a beat
*/

#include "beat_decoder.h"

void removeNoise(beat *numBits, beat *beatDur) {
    // use bit shifting to the right to do the math
    // x >> 3 = 12.5% of x
    beat numBeats[3] = {1, 3, 7};
    
    for (int i = 0; i < 3; ++i) {
        beat center = numBeats[i] * BEAT_DURATION;
        beat bitError = center >> BEAT_ERROR_RANGE;

        beat lowRange = center - bitError;
        beat highRange;

        if (i < 2) {
            highRange = 2 * center;
        } else {
            highRange = center + bitError;
        }
        

        if (*numBits < highRange && *numBits >= lowRange) {
            *beatDur = numBeats[i];
            adjustFactors(&center, numBits);
            return;
        }
    }

    *beatDur = -1;
}

void adjustFactors(beat *expNumBits, beat *recNumBits) {
    // goal: adjust the beat duration to better include the beats

    // calculates the difference between the received and expected bits
    // divides that by 4, so that we get some shift in the right direction
    // then uses this number to adjust the beat duration
    BEAT_DURATION += (*expNumBits - *recNumBits) >> 2;
}