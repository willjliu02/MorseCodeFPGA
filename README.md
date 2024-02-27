# MorseCodeFPGA

Background:
Morse code relies on the metric of a “beat” to identify letters, words, and spaces. Based on this metric, we have the following table:
![Table of the true beat lengths for a symbol](/read_me_refs/beat_table.png)
The letters are organized in the form of a binary tree roughly in the order from the most to least common letters in the alphabet from root to leaf. This can be viewed in the following image.
![Binary tree of the morse code letters](/read_me_refs/morse_code_translator.png)

Decoder Details:
This is the decoder aspect of the morse code translator, so it takes information in the form of morse code and
converts it into human readable text. Currently, the only type of input that this decoder takes is a bit file, where a 1 is an on signal and a 0 is an off signal. 