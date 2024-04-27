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
[Second Project Update](https://docs.google.com/document/d/1_Po3Trma6acqfvD53LwHgvnLtOSarWcZHR8rateOmck/edit?usp=sharing)

## Decoder Details:
This is the decoder aspect of the morse code translator, so it takes information in the form of morse code and
converts it into human readable text. Currently, the only type of input that this decoder takes is a bit file, 
where a 1 is an on signal and a 0 is an off signal. 

### How to Filter:
1) Start off of the board
2) pip install pydub
3) Using the AudioSegment class open the audio file
4) Using the array_of_samples function in the AudioSegment, wrap that in a numpy array of dtype = float
5) Use numpy.save to save the array to a file
6) Move to the board
7) Load the saved numpy file into filter.ipynb
8) Run all the blocks above the for loop that has timing around it
9) Run the for loop to stream in all the data and fill the filtered_signal buffer

### How To Decoder:
1) Create a bit file that contains morse code
2) Run all boxes down to the translate function
3) Choose the decoder type. (For the hardware overlay, you will have to also create the overlay, which can be found in decoder_hw/decoder)
4) Instantiate a beat decoder (the most optimized values were the ones used in the "beat_decoder" function above the tests)
5) Create the actual decoder by passing in the beat decoder.
6) Call the translate function on the name of the bit file and the beat decoder. The output will be printed to the console. If you'd like to change that, then send in the name of an output file.

### PL Implementation:
The function that was moved over from PS to PL was the processNextBit function. The hardware code can be seen in the process_hw.cpp program.The status of this part is that the code is able to be synthesized in HLS, but has run into a memory dependency with one of the AXI classes. 

### Status till Completion:
What is left to complete after this is simply the filter, which will be our last PL implementation, and then the use of a python library to take in audio and to convert that into bits that will be passed to hardware. This leaves the decoder at around 60% complete.

## Encoder Details

### How To:
1) Create a txt file with your given text input (create in same directory as project)
2) Run all boxes and enter in the text file name you want to be encoded
3) Output will be printed and produced in a txt file called output.txt

### PL Implementation:
The function that was moved over to PS from PL was the encode_text function. The hardware code can be found in encoder.cpp. The status of this is that there is a pointer error that is preventing the encoder from being synthesized.

### Status till Completion:
What is left to complete after this is simply to add an IP to interact and make the morse code light up. This puts the encoder at something like 80% complete.
