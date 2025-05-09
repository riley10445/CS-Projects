#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char encode(char* str, char command, int x);
char decode(char* str, char command, int x);
void binary_dec(char* str, int x);
void binary_enc(char* str, int x);
void shift_left(char* str, int x);
void shift_right(char* str, int x);
void decrease(char* str, int x);
void increase(char* str, int x);

void binary_dec(char* str, int x){

    int num_elements = 0;
    int x_len = x;
    while (x_len > 0) {
        x_len /= 2;
        num_elements++;
    }

    int binary[num_elements];
    int i = 0;
    while (x > 0) {
        binary[i] = x % 2;
        x /= 2;
        i++;
    }

    int binI = num_elements-1;
    for(int i = 0; i<strlen(str); i++){
        if(binary[binI] == 1){
            if (str[i] == 'A'){
                str[i] = 'Z';
            }
            else if (str[i] == 'a'){
                str[i] = 'z';
            }
            else if (str[i] == '0'){
                str[i] = '9';
            }
            else if(isalnum(str[i])){
                str[i] = str[i] - 1;
            }
        }
        else{
            if (str[i] == 'Z'){
                str[i] = 'A';
            }
            else if (str[i] == 'z'){
                str[i] = 'a';
            }
            else if (str[i] == '9'){
                str[i] = '0';
            }
            else if(isalnum(str[i])){
                str[i] = str[i] + 1;
            }
        }
        binI--;
        if (binI < 0){
            binI = num_elements - 1;
        }
    }
}

void binary_enc(char* str, int x){

    int num_elements = 0;
    int x_len = x;
    while (x_len > 0) {
        x_len /= 2;
        num_elements++;
    }

    int binary[num_elements];
    int i = 0;
    while (x > 0) {
        binary[i] = x % 2;
        x /= 2;
        i++;
    }

    int binI = num_elements-1;
    for(int i = 0; i<strlen(str); i++){
        if(binary[binI] == 1){
            if (str[i] == 'Z'){
                str[i] = 'A';
            }
            else if (str[i] == 'z'){
                str[i] = 'a';
            }
            else if (str[i] == '9'){
                str[i] = '0';
            }
            else if(isalnum(str[i])){
                str[i] = str[i] + 1;
            }
        }
        else{
            if (str[i] == 'A'){
                str[i] = 'Z';
            }
            else if (str[i] == 'a'){
                str[i] = 'z';
            }
            else if (str[i] == '0'){
                str[i] = '9';
            }
            else if(isalnum(str[i])){
                str[i] = str[i] - 1;
            }
        }
        binI--;
        if (binI < 0){
            binI = num_elements - 1;
        }
    }
}

void shift_right(char* str, int x) {
    int len = strlen(str);
    char temp;
    for (int i = 0; i < x; i++) {
        temp = str[len-1];
        for (int j = len-1; j > 0; j--) {
            str[j] = str[j-1];
        }
        str[0] = temp;
    }
}

void shift_left(char* str, int x) {
    int len = strlen(str);
    char temp;
    for (int i = 0; i < x; i++) {
        temp = str[0];
        for (int j = 0; j < len-1; j++) {
            str[j] = str[j+1];
        }
        str[len-1] = temp;
    }
}

void decrease(char *str, int x) { 
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] - x < 33) {
            str[i] = 127 - (33 - (str[i] - x));
            }
        else{
            str[i] -= x;
        }
    }
}

void increase(char *str, int x) { 
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] + x > 126) {
            str[i] = 33 + ((str[i] + x) - 127);
            }
        else{
            str[i] += x;
        }
    }
}

char encode(char* str, char command, int x) {
    if (x == 0) {
        return *str;
    }
    switch (command) {
        case 'L':
            shift_right(str, x);
            break;
        case 'R':
            shift_left(str, x);
            break;
        case 'C':
            decrease(str, x);
            break;
        case 'B':
            binary_enc(str, x);
            break;
        default:
            printf("Invalid command\n");
            break;
    }
    return *str;
}

char decode(char* str, char command, int x) {
    if (x == 0) {
        return *str;
    }
    switch (command) {
        case 'L':
            shift_left(str, x);
            break;
        case 'R':
            shift_right(str, x);
            break;
        case 'C':
            increase(str, x);
            break;
        case 'B':
            binary_dec(str, x);
            break;
        default:
            printf("Invalid command\n");
            break;
    }
    return *str;
}


int main(int argc, char* argv[]) {

    char line[1000];
    char* word = malloc(100 * sizeof(char));

    if (argc != 4) { 
        printf("Invalid number of arguments, must be exactly 4\n");
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r"); //Opens file
    if (input_file == NULL) {
        printf("Could not open file '%s'\n", argv[1]);
        return 1;
    }

    char* operation = argv[2];
    char* encoding_sequence = argv[3];

    if (strcmp(operation, "encode") == 0) {
        while (fgets(line, 1000, input_file) != NULL) {

            int shift;
            int seq = 0;
            int seq_len = strlen(encoding_sequence);
            int seq_test = 0;

            int len = strlen(line);
            if (line[len - 1] == '\n') {
                line[len - 1] = '\0'; 
            }

            while (seq_test < seq_len){
                char c = encoding_sequence[seq_test];
                char n = encoding_sequence[seq_test+1];
                if (c != 'L' && c != 'R' && c != 'C' && c != 'B') {
                    printf("I cannot encrypt the message with task '%c'!\n", c);
                    exit(1);
                }
                if (!isdigit(n) || n < '0' || n > '9'){
                    printf("Please enter a valid number for the encryption task '%c'!\n", c);
                    exit(1);
                }
                seq_test += 2;
            }

            char* ptr = line;
            while (sscanf(ptr, "%s", word) == 1) {
                while (seq<seq_len) { 
                    char c = encoding_sequence[seq];
                    shift = atoi(&encoding_sequence[seq+1]);
                    char new_word = encode(word, c, shift);
                    *word = new_word;
                    seq += 2;
                }
                ptr += strlen(word) + 1; 
                seq=0;
                printf("%s\n", word);
            }
        }
    } 
    else if (strcmp(operation, "decode") == 0) {
        while (fgets(line, 1000, input_file) != NULL) {
            int shift;
            int seq = 0;
            int seq_len = strlen(encoding_sequence);
            int seq_test = 0;

            int len = strlen(line); 
            if (line[len - 1] == '\n') {
                line[len - 1] = '\0'; 
            }

            while (seq_test < seq_len){
                char c = encoding_sequence[seq_test];
                char n = encoding_sequence[seq_test+1];
                if (c != 'L' && c != 'R' && c != 'C' && c != 'B') {
                    printf("I cannot decrypt the message with task '%c'!\n", c);
                    exit(1);
                }
                if (!isdigit(n) || n < '0' || n > '9'){
                    printf("Please enter a valid number for the encryption task '%c'!\n", c);
                    exit(1);
                }
                seq_test += 2;
            }

            char* ptr = line;
            while (sscanf(ptr, "%s", word) == 1) {
                while (seq<seq_len) {
                    char c = encoding_sequence[seq];
                    shift = atoi(&encoding_sequence[seq+1]);
                    char new_word = decode(word, c, shift);
                    *word = new_word;
                    seq += 2;
                }
                ptr += strlen(word) + 1; 
                seq=0;
                printf("%s\n", word);
            }
        }
    } 
    else {
        printf("Invalid option, must be 'encode' or 'decode'!\n");
        return 1;
    }

    fclose(input_file);
    free(word);

    return 0;
}
