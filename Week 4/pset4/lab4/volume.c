// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
uint8_t header[HEADER_SIZE];    // 44 bytes of the header
int16_t buffer;                 // each samle is 2 bytes long, so 16 bit is perfect for buffering

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    // where data is stored, size of element to be stored = 44 bytes, number of elements to be read = 1 times 44 bytes, file to read from
    fread(header, HEADER_SIZE, 1, input);
    
    // which data is taken, size of element to be stored = 44 bytes, number of elements to be read = 1 times 44 bytes, file to write to
    fwrite(header, HEADER_SIZE, 1, output);
    
    // TODO: Read samples from input file and write updated data to output file
    // return from fread is size of bytes; while until byte size is zero so file ended and then stores it in buffer
    while (fread(&buffer, sizeof(int16_t), 1, input))   
    {
        buffer = buffer * factor;
        // write as before to adress of buffer, 16 bytes, 1 write of 16 bytes, to the output file
        fwrite(&buffer, sizeof(int16_t), 1, output);     
    }
    
    // Close files
    fclose(input);
    fclose(output);
}
