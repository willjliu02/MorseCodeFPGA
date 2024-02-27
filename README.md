# MorseCodeFPGA

## Abstract
In this project, we will be designing a morse code translator that uses a combination of hardware and software to take in data from a variety of inputs types using software, process the inputs using hardware, and output the translation in their respective forms on an OLED for the decoding and as LED blinks for the encoding.

## Background:
Morse code relies on the metric of a “beat” to identify letters, words, and spaces. Based on this metric, we have the following table:
![Table of the true beat lengths for a symbol](/read_me_refs/beat_table.png)
The letters are organized in the form of a binary tree roughly in the order from the most to least common letters in the alphabet from root to leaf. This can be viewed in the following image.
![Binary tree of the morse code letters](/read_me_refs/morse_code_translator.png)

## More Detailed Information
[Formal Proposal Document](https://docs.google.com/document/d/1Nj2nfRw68O-NiOnoTTRfmW-j80fDnC_wfzZ5VHfgZU8/edit?usp=sharing)

## Decoder Details:
This is the decoder aspect of the morse code translator, so it takes information in the form of morse code and
converts it into human readable text. Currently, the only type of input that this decoder takes is a bit file, 
where a 1 is an on signal and a 0 is an off signal. 

### How To:
1) Create a bit file that contains morse code
2) Run all boxes down to the translate function
3) Call the translate function on the name of the bit file. The output will be printed to the console. If you'd like to change that, then send in the name of an output file.

## Encoder Details

### How To: