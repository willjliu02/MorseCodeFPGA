#include <hls_stream.h>
#include <ap_axi_sdata.h>
using namespace hls;

const char MORSE_CODE_DICT[37][6] = {
        ".-",   "-...", "-.-.", "-..",  ".","..-.", "--.",  "....", "..",   ".---", "-.-",  ".-..", "--",
        "-.",   "---",  ".--.", "--.-", ".-.",  "...",  "-",    "..-",  "...-", ".--",  "-..-", "-.--", "--..",
        ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", "/"
};

const char LETTER_AS_AN_INDEX[37] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                                     'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                     '0', ' '};

char toupper(char c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

const char* encode_symbol(const char symbol) {
    return (symbol == '-') ? "111" : "1";
}

const char* symbol_separator(){
    return "0";
}

const char* letter_separator(){
    return "000";
}

const char* word_separator(){
    return "0000000";
}

int findIndex(const char arr[], int size, const char target) {

    for (int i = 0; i < size; ++i) {
        if (arr[i]==target) {
            return i; // Return the index if the target value is found
        }
    }
    return -1; // Return -1 if the target value is not found in the array
}

int morseCodeLength(const char morse[]) {
    int length = 0;
    while (morse[length] != '\0') {
        length++;
    }
    return length;
}

const char* encode_char(const char c) {
	static char result[50];
    int index = findIndex(LETTER_AS_AN_INDEX, 37, toupper(c));
    const char* trans = MORSE_CODE_DICT[index];
    int symbol_index = 0;
    for (int i = 0; trans[i] != '\0'; i++) {
        const char* encoded_symbol = encode_symbol(trans[i]);
        while (*encoded_symbol != '\0') {
            result[symbol_index++] = *encoded_symbol++;
        }
        if (trans[i + 1] != '\0') {
            const char* separator = symbol_separator();
            while (*separator != '\0') {
                result[symbol_index++] = *separator++;
            }
        } else {
            const char* separator = letter_separator();
            while (*separator != '\0') {
                result[symbol_index++] = *separator++;
            }
        }
    }
    //delete[] result;
    return result;
}
void initialize_array(char array[], int size) {
    for (int i = 0; i < size; ++i) {
        array[i] = 0;
    }
}

const char* encode_text(const char text[]) {
    static char encoded_text[200]; // Static array to store encoded message
    initialize_array(encoded_text, 200); // Clear the array before use

    int index = 0;
    for (int i = 0; text[i] != '\0'; ++i) {
        const char* encoded_char = encode_char(text[i]);
        while (*encoded_char != '\0') {
            encoded_text[index++] = *encoded_char++;

        }
        encoded_text[index++] = ' ';
    }


    return encoded_text;
}


typedef ap_axiu<8,32,1,1>AXI_STREAM;


void morse_encoder(stream<AXI_STREAM>& in_stream, stream<AXI_STREAM>& out_stream)
 {
#pragma HLS INTERFACE axis register both port= out_stream
#pragma HLS INTERFACE axis register both port= in_stream
#pragma HLS INTERFACE ap_ctrl_none port=return



    while (1) {
        AXI_STREAM input = in_stream.read();
        char data[2]; // Modify to accept an array of characters
        data[0] = input.data; // Extracting data from the input stream
        data[1] = '\0'; // Null-terminate the string

        const char* encoded_data = encode_text(data);

        AXI_STREAM output;
        output.data = encoded_data;
        output.keep = input.keep;
        output.last = input.last;
        out_stream.write(output);





    }
}
