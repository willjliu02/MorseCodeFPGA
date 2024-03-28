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
    return "000000";
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

    const char* morse_code = MORSE_CODE_DICT[index];

    if (index != -1) {
        size_t morse_code_length = strlen(morse_code);
        size_t encoded_bits_index = 0;
        for (size_t i = 0; i < morse_code_length; ++i) {
            const char* encoded_symbol = encode_symbol(morse_code);
            std::strcpy(encoded_bits + encoded_bits_index, encoded_symbol);
            encoded_bits_index += std::strlen(encoded_symbol);

            if (i < morse_code_length - 1) {
                const char* separator = encode_symbol_separator();
                std::strcpy(encoded_bits + encoded_bits_index, separator);
                encoded_bits_index += std::strlen(separator);
            }
        }
        encoded_bits[encoded_bits_index] = '\0'; // Null-terminate the encoded string
    } else {
        // Handle case when character not found in Morse code dictionary
        strcpy(encoded_bits, "Character not found in Morse code dictionary");
    }
    return encoded_bits;
}
char * encode_text(const char* text) {
    static char encoded_text[200]; // Use a fixed size buffer
    memset(encoded_text, 0, sizeof(encoded_text)); // Initialize with zeros

    int i = 0;
    while (text[i] != '\0') {
        char uppercase_char = toupper(text[i]);
        if (text[i] != ' ') {
            strcat(encoded_text, encode_char(uppercase_char));
            strcat(encoded_text, encode_letter_separator());
            strcat(encoded_text, " ");
        } else {
            strcat(encoded_text, encode_word_separator());
            strcat(encoded_text, " ");
        }
        i++;
    }
    return encoded_text;
}




int main() {
    char text[] = "abcdefghijklmnoqrstuv";
    const char* encoded_text = encode_text(text);
    cout << "Encoded text: " << encoded_text << endl;
    return 0;
}
