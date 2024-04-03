#include <iostream>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

using namespace hls;

typedef ap_axiu<8,1,1,1>AXI_STREAM;

const char* encode_text(const char* text);

void morse_encoder(stream<AXI_STREAM>& in_stream, stream<AXI_STREAM>& out_stream) {
#pragma HLS INTERFACE axis register both port= out_stream
#pragma HLS INTERFACE axis register both port= in_stream
#pragma HLS INTERFACE ap_ctrl_none port=return

    while (!in_stream.empty()) {
        AXI_STREAM input = in_stream.read();
        char data[2]; // Modify to accept an array of characters
        data[0] = input.data; // Extracting data from the input stream
        data[1] = '\0'; // Null-terminate the string

        // Perform Morse encoding on the data
        const char* encoded_data = encode_text(data);

        // Output the encoded data in AXI-Stream format
        AXI_STREAM output;
        output.data = encoded_data;
        output.keep = input.keep;
        output.last = input.last;
        out_stream.write(output);
    }
}



const char* MORSE_CODE_DICT[37] = {
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

const char* encode_symbol(const char* symbol) {
    return (symbol[0] == '-') ? "111" : "1";
}

const char* encode_symbol_separator() {
    return "0";
}

const char* encode_word_separator() {
    return "0000000";
}

const char* encode_letter_separator() {
    return "000";
}

int findIndex(const char arr[], int size, const char target) {
    for (int i = 0; i < size; ++i) {
        if (arr[i]==target) {
            return i; // Return the index if the target value is found
        }
    }
    return -1; // Return -1 if the target value is not found in the array
}

const char* encode_char(const char c) {
    static char encoded_bits[50];
    int index = findIndex(LETTER_AS_AN_INDEX, 37, c);

    if (index != -1) {
           size_t morse_code_length = 0;
           while (MORSE_CODE_DICT[index][morse_code_length] != '\0') {
               ++morse_code_length;
           }

        size_t encoded_bits_index = 0;
        const char* encoded_symbol = encode_symbol(MORSE_CODE_DICT[index]);
        const char* separator = encode_symbol_separator();

        while (*encoded_symbol != '\0' && encoded_bits_index < sizeof(encoded_bits) - 1) {
            encoded_bits[encoded_bits_index++] = *encoded_symbol++;
        }

        for (size_t i = 1; i < morse_code_length && encoded_bits_index < sizeof(encoded_bits) - 1; ++i) {
            while (*separator != '\0' && encoded_bits_index < sizeof(encoded_bits) - 1) {
                encoded_bits[encoded_bits_index++] = *separator++;
            }
            encoded_symbol = encode_symbol(MORSE_CODE_DICT[index]);
            while (*encoded_symbol != '\0' && encoded_bits_index < sizeof(encoded_bits) - 1) {
                encoded_bits[encoded_bits_index++] = *encoded_symbol++;
            }
        }
        encoded_bits[encoded_bits_index] = '\0'; // Null-terminate the encoded string
    } else {
        // Handle case when character not found in Morse code dictionary
        const char* error_msg = "Character not found in Morse code dictionary";
        size_t i = 0;
        size_t encoded_bits_index = 0;
        while (error_msg[i] != '\0' && encoded_bits_index < sizeof(encoded_bits) - 1) {
            encoded_bits[encoded_bits_index++] = error_msg[i++];
        }
        encoded_bits[encoded_bits_index] = '\0'; // Null-terminate the encoded string
    }
    return encoded_bits;
}
void initialize_array(char* array, int size) {
    for (int i = 0; i < size; ++i) {
        array[i] = 0;
    }
}

const char* encode_text(const char* text) {
    static char encoded_text[200]; // Use a fixed size buffer

    initialize_array(encoded_text, sizeof(encoded_text)); // Initialize with zeros

    int i = 0;
    int encoded_text_index = 0; // New index variable for updating encoded_text

    while (text[i] != '\0') {
        char uppercase_char = toupper(text[i]);
        if (text[i] != ' ') {
            const char* encoded_char = encode_char(uppercase_char);
            while (*encoded_char != '\0' && encoded_text_index < sizeof(encoded_text) - 1) {
                encoded_text[encoded_text_index++] = *encoded_char++;
            }
            const char* letter_separator = encode_letter_separator();
            while (*letter_separator != '\0' && encoded_text_index < sizeof(encoded_text) - 1) {
                encoded_text[encoded_text_index++] = *letter_separator++;
            }
            encoded_text[encoded_text_index++] = ' '; // Add space after each encoded character
        } else {
            const char* word_separator = encode_word_separator();
            while (*word_separator != '\0' && encoded_text_index < sizeof(encoded_text) - 1) {
                encoded_text[encoded_text_index++] = *word_separator++;
            }
            encoded_text[encoded_text_index++] = ' '; // Add space after word separator
        }
        ++i;
    }
    return encoded_text;
}



