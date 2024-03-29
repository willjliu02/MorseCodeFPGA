#include <iostream>
#include <cstring>

using namespace std;

const char* MORSE_CODE_DICT[] = {
        ".-",   "-...", "-.-.", "-..",  ".","..-.", "--.",  "....", "..",   ".---", "-.-",  ".-..", "--",
        "-.",   "---",  ".--.", "--.-", ".-.",  "...",  "-",    "..-",  "...-", ".--",  "-..-", "-.--", "--..",
        ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", "/"
};

const char LETTER_AS_AN_INDEX[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
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
    int index = findIndex(LETTER_AS_AN_INDEX,37,c);

    if (index != -1) {
        size_t morse_code_length = 0;
        while (MORSE_CODE_DICT[index][morse_code_length] != '\0') {
            ++morse_code_length;
        }

        size_t encoded_bits_index = 0;
        for (size_t i = 0; i < morse_code_length && encoded_bits_index < sizeof(encoded_bits) - 1; ++i) {
            const char* encoded_symbol = encode_symbol(MORSE_CODE_DICT[index]);
            while (*encoded_symbol != '\0' && encoded_bits_index < sizeof(encoded_bits) - 1) {
                encoded_bits[encoded_bits_index++] = *encoded_symbol++;
            }

            if (i < morse_code_length - 1 && encoded_bits_index < sizeof(encoded_bits) - 1) {
                const char* separator = encode_symbol_separator();
                while (*separator != '\0' && encoded_bits_index < sizeof(encoded_bits) - 1) {
                    encoded_bits[encoded_bits_index++] = *separator++;
                }
            }
        }
        encoded_bits[encoded_bits_index] = '\0'; // Null-terminate the encoded string
    } else {
        // Handle case when character not found in Morse code dictionary
        size_t msg_length = sizeof("Character not found in Morse code dictionary") - 1;
        for (size_t i = 0; i < msg_length && i < sizeof(encoded_bits) - 1; ++i) {
            encoded_bits[i] = "Character not found in Morse code dictionary"[i];
        }
        encoded_bits[msg_length] = '\0'; // Null-terminate the encoded string
    }
    return encoded_bits;
}

const char* encode_text(const char* text) {
    static char encoded_text[200]; // Use a fixed size buffer
    memset(encoded_text, 0, sizeof(encoded_text)); // Initialize with zeros

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

