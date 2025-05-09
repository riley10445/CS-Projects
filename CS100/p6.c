# include <stdio.h>
# include <stdlib.h>
# include <string.h>

void copy_ppm(char *input_filename, char *output_filename) {
    FILE *input_file, *output_file;
    char magic_number[3];
    int width, height, maxval, i, j;
    int r, g, b;

    // Open input file for reading
    input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        printf("Error: Unable to open input file '%s'\n", input_filename);
        return;
    }

    // Read header information from input file
    fscanf(input_file, "%s\n%d %d\n%d\n", magic_number, &width, &height, &maxval);

    // Open output file for writing
    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Error: Unable to open output file '%s'\n", output_filename);
        fclose(input_file);
        return;
    }

    // Write header information to output file
    fprintf(output_file, "%s\n%d %d\n%d\n", magic_number, width, height, maxval);

    // Copy pixel data from input file to output file
    fprintf(output_file, " ");
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(input_file, "%3d %3d %3d", &r, &g, &b);
            fprintf(output_file, "%3d %3d %3d   ", r, g, b);
        }
        fprintf(output_file, "\n ");
    }

    // Close input and output files
    fclose(input_file);
    fclose(output_file);
}


// Function to copy a PGM file
void copy_pgm(char *input_filename, char *output_filename) {
    FILE *input_file, *output_file;
    char magic_number[3];
    int width, height, maxval, i, j;
    int gray;

    // Open input file for reading
    input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        printf("Error: Unable to open input file '%s'\n", input_filename);
        return;
    }

    // Read header information from input file
    fscanf(input_file, "%s\n%d %d\n%d\n", magic_number, &width, &height, &maxval);

    // Open output file for writing
    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Error: Unable to open output file '%s'\n", output_filename);
        fclose(input_file);
        return;
    }

    // Write header information to output file
    fprintf(output_file, "%s\n%d %d\n%d\n", magic_number, width, height, maxval);

    // Copy pixel data from input file to output file
    fprintf(output_file, " ");
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(input_file, "%3d", &gray);
            fprintf(output_file, "%3d ", gray);
        }
        fprintf(output_file, "\n ");
    }

    // Close input and output files
    fclose(input_file);
    fclose(output_file);
}


void grayscale(char *input_filename, char *output_filename) {
    FILE *input_file, *output_file;
    char magic_number[3];
    int width, height, maxval, i, j;
    unsigned char r, g, b, gray;

    // Open input file for reading
    input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        printf("Error: Unable to open input file '%s'\n", input_filename);
        return;
    }

    // Read header information from input file
    fscanf(input_file, "%s\n%d %d\n%d\n", magic_number, &width, &height, &maxval);

    // Check that input is a PPM file
    if (strcmp(magic_number, "P3") != 0) {
        printf("Error: Input file is not a PPM file\n");
        fclose(input_file);
        return;
    }

    // Open output file for writing
    output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        printf("Error: Unable to open output file '%s'\n", output_filename);
        fclose(input_file);
        return;
    }

    // Write header information to output file
    fprintf(output_file, "P2\n%d %d\n%d\n", width, height, maxval);

    // Convert RGB to grayscale and write pixel data to output file
    fprintf(output_file, " ");
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(input_file, "%hhu %hhu %hhu", &r, &g, &b);
            gray = (r + g + b) / 3;
            fprintf(output_file, "%3hhu ", gray);
        }
        fprintf(output_file, "\n ");
    }

    // Close input and output files
    fclose(input_file);
    fclose(output_file);
}

void shrink(char *input_filename, char *output_filename) {
    FILE *input_file, *output_file;
    char magic_number[3];
    int width, height, maxval, new_width, new_height, i, j;
    unsigned char pixel1, pixel2, pixel3, pixel4, new_pixel;

    // Open input file for reading
    input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        printf("Error: Unable to open input file '%s'\n", input_filename);
        return;
    }

    // Read header information from input file
    fscanf(input_file, "%s\n%d %d\n%d\n", magic_number, &width, &height, &maxval);

    // Check that input is a PGM file
    if (strcmp(magic_number, "P2") != 0) {
        printf("Error: Input file is not a PGM file\n");
        fclose(input_file);
        return;
    }

    // Calculate new width and height
    new_width = width / 2;
    new_height = height / 2;

    // Open output file for writing
    output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        printf("Error: Unable to open output file '%s'\n", output_filename);
        fclose(input_file);
        return;
    }

    // Write header information to output file
    fprintf(output_file, "P2\n%d %d\n%d\n", new_width, new_height, maxval);

    // Shrink
    fprintf(output_file, " ");
    for (i = 0; i < new_height; i++) {
        for (j = 0; j < new_width; j++) {
            // Read four pixels from input file
            fscanf(input_file, "%hhu", &pixel1);
            fscanf(input_file, "%hhu", &pixel2);
            fscanf(input_file, "%hhu", &pixel3);
            fscanf(input_file, "%hhu", &pixel4);

            // Calculate new pixel value as the average of the four pixels
            new_pixel = (pixel1 + pixel2 + pixel3 + pixel4) / 4;

            // Write new pixel to output file
            fprintf(output_file, "%3hhu ", new_pixel);
        }

        // Ignore the last pixel of odd-numbered rows
        if (width % 2 == 1) {
            fscanf(input_file, "%hhu", &pixel1);
        }

        // Move to next row
        fscanf(input_file, "\n");
        fprintf(output_file, "\n ");
    }

    // Close input and output files
    fclose(input_file);
    fclose(output_file);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Error: Insufficient arguments\n");
        printf("Usage: %s <function> <input_filename> <output_filename>\n", argv[0]);
        return 1;
    }

    char *function = argv[1];
    char *input_filename = argv[2];
    char *output_filename = argv[3];

    if (strcmp(function, "copy-ppm") == 0) {
        copy_ppm(input_filename, output_filename);
    } else if (strcmp(function, "copy-pgm") == 0) {
        copy_pgm(input_filename, output_filename);
    } else if (strcmp(function, "grayscale") == 0) {
        grayscale(input_filename, output_filename);
    } else if (strcmp(function, "shrink") == 0) {
        shrink(input_filename, output_filename);
    } else {
        printf("Error: Unknown function '%s'\n", function);
        return 1;
    }

    return 0;
}