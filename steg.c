#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "charList.h"


/*

LINKING

gcc -c -o steg.o steg.c

gcc -c -o charList.o charList.c


COMPILING

Commands to compile on Linux Ubuntu:

gcc -o steg steg.o charList.o -lm

Commands to compile on Linux Centos:

gcc -o steg steg.o charList.o -std=c99

*/



// pixel structure to store the colour values
struct Pixel
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
};

// PPM file structure to store format, width, heigth, max coulour
// and a dinamic 2d pixel matrix
struct PPM
{
    const char * format;
    unsigned int width;
    unsigned int height;
    unsigned char colourMax;
    struct Pixel ** pixelMatrix;
};

// prints a binary number
void intToBin(unsigned int number);

// creates a binary number with n 1's of length
int auxNumber(int n);

// inserts a bit in an integer at a certain position
int wBitPosition(int value, int bit, int pos);

// reads a bit in an integer at a certain position
int rBitPosition(int value, int pos);

// gets the length of a string
int strLength(char * string);

// returns a PPM data structure from a file
struct PPM * getPPM(FILE * f);

// prints a PPM data structure 
void showPPM(struct PPM * im);

// writing a new PPM from a given ppm data structure
void outputPPMFile(struct PPM * ppm, char * outFileName);

// encodes a message in a ppm data structure, using a secret integer
struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret);

// decodes a message from a PPM file, using a secret integer
char * decode(struct PPM * im, unsigned int secret);

// frees memory from a PPM data structure
void freePPM(struct PPM * ppm);

// writes a value on a Pixel structure
void wPixelValue(struct Pixel * pixel, int number);

// reads a value from a pixel structure
int rPixelValue(struct Pixel * pixel, int length);


int main(int argc, char ** argv)
{
    // creates a file and ppm pointer
    FILE * file;
    struct PPM * ppm;

    // checks if arguments are correct
    if (argc < 3 || argc > 5)
    {
        printf("\nwrong number of arguments\n\n");
    }

    // initializes a file reader
    file = fopen(argv[1], "r");

    // if file is not found, terminate program
    if(file == NULL)
    {
        printf("File \"%s\" not found!\n\n", argv[1]);

        exit(0);
    }
    
    // gets ppm data structure from given file
    ppm = getPPM(file);
    
    // gets the user command
    char command = argv[2][0];


    if ( command == 'e' )
    {
        // if user wants to encrypt

        // variables for secret integer and 
        int secret;
        char message[64];

        // getting string message and integer secret
        printf("Please, enter the Message: ");
        fgets(message,64,stdin);
        printf("\nEnter the secret: ");
        scanf("%d", &secret);
        
        // returning encoded ppm file
        struct PPM * encodedPPM = encode(ppm, message, strLength(message), secret);

        // writing encoded ppm file
        outputPPMFile(encodedPPM, argv[3]);

    } 
    else if ( command == 'd' )
    {
        // if user wants to decode message

        // getting integer secret from user
        int secret;
        printf("Secret: ");
        scanf("%d",&secret);

        // returning encoded message from ppm file
        char * msg = decode(ppm,secret);

        // printing encode message
        printf("\nEncoded Message: %s\n", msg);
    } 
    else if ( command == 's')
    {
        // printing ppm file on the terminal
        showPPM(ppm);
    }

    // close file reader
    fclose(file);

    // free file pointer
    free(file);    

    // free PPM data structure
    freePPM(ppm);



    return 0;
}

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret)
{
    // seeds a secret to the pseudo random generator
    srand(secret);

    // gets the max capacity of the ppm data structure
    unsigned int max = im->height*im->width;

    // initialises the message character index
    unsigned char index = 0;

    // runs while index is greater than message size
    while (index <= mSize)
    {
        // stores current message character index in a character variable
        unsigned char c = message[index];
        
        // if character is new line 
        // skip looop and increment index
        if( c == 10) {
            index++;
            continue;
        }

        // loop to write bits on multiple pixels
        for (size_t i = 0; i < 3; i++)
        {
            // initializing position, row and collumn variables
            unsigned int pos, row, collumn;

            // pos is withing the ppm capacity bounds
            pos = rand() % max;

            // row is equal to the position modulus of height
            row = pos % im->height;

            // collumn is equal to the position division of height
            collumn = pos / im->height;

            // getting the shift on the character binary number
            int shift = 3 * i;

            // extracting 3 bits with the correct shift
            int three_bits = (c >> shift ) & auxNumber(3);

            // getting pixel pointer from correct position
            struct Pixel * pxl = &(im->pixelMatrix[collumn][row]);

            // writing the value on the pixel pointer
            wPixelValue(pxl, three_bits);
        }

        // incrementing index
        index++;
    }

    // returning ppm data structure position
    return im;
}

char * decode(struct PPM * im, unsigned int secret)
{
    // seeds a secret to the pseudo random generator
    srand(secret);

    // gets the max capacity of the ppm data structure
    unsigned int max = im->height*im->width;
    
    // initialising charater singly linked list to store an unknown amount of characters
    // setting its original length to 0;
    struct CharNode * charList = (struct CharNode *) malloc(sizeof(struct CharNode));
    charList->length = 0;

    while (1)
    {
        // stores current character being read
        unsigned char c = 0;

        // reads 3 pixels to get the binary number for the character
        for (size_t i = 0; i < 3; i++)
        {
            // initializing position, row and collumn variables
            unsigned int pos, row, collumn;

            // pos is withing the ppm capacity bounds
            pos = rand() % max;

            // row is equal to the position modulus of height
            row = pos % im->height;

            // collumn is equal to the position division of height
            collumn = pos / im->height;

            // getting pixel pointer from correct position
            struct Pixel * pxl = &(im->pixelMatrix[collumn][row]);

            // reading 3 bits from the first pixel collumn of colours
            int three_bits = rPixelValue(pxl,1);

            // getting the correct shift for the character number
            int shift = 3 * i;

            // writing the 3 bits on the correct position
            c = c | (three_bits << shift);
        }

        // finish when it reaches the end of string character
        if( c == 0) break;
        
        // inserts current character in the singly list character
        insertCharList(charList, c);
    }
    
    // returns a string from the character list
    char * message = getMessage(charList);

    return message;
}


struct PPM * getPPM(FILE * f)
{
    // stores the character variables
    // ch for current character, setting a value different from a number character
    // prevCh to store previous character in the loop
    // index to store the character position
    // numChar stores the character of a number
    char ch = 65, prevCh, index = 0, numChar[6];

    // s
    int numValues = 0, valCounter = 0;
    
    struct PPM * ppm = (struct PPM *) malloc(sizeof(struct PPM));
    
    while ( ch != EOF)
    {
        int number = 0;
        
        prevCh = ch;
        
        ch = getc(f);

        if(ch == '#' || ch == 'P')
        {
            while (ch != '\n')
            {
                ch = getc(f);
            }
            ch = 65;
            continue;
        }

        if(ch == 32 || prevCh != 32 && ch == 10 )
        {
            
            numValues = numValues + 1;
            
            numChar[index + 1] = '\0';

            int i = 0, j;
            double len = index - 1;
            
            while (index > i)
            {
                j = (int) (numChar[i] - 48);

                if (j < 0) j = 0;

                number = number + ( (int) ( j * pow(10.0, len)) );

                i = i + 1;
                len = len - 1;
            }
            
            if (numValues <= 3)
            {
                switch (numValues)
                {
                    case 1:
                        ppm->width = number;
                        ppm->format = "P3";
                        break;

                    case 2:
                        ppm->height = number;
                        ppm->pixelMatrix = (struct Pixel **) malloc(sizeof(struct Pixel *) * ppm->width);
                        for(size_t i = 0; i < ppm->width; i++)
                        {
                            ppm->pixelMatrix[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * ppm->height);
                        }
                        break;

                    case 3:
                        ppm->colourMax = (unsigned char) number;
                        break;
                }
            } 
            else
            {
                unsigned int tempValues = numValues - 4;
                unsigned int pixels = tempValues / 3;
                char colour = tempValues % 3;
                unsigned int row = pixels % ppm->height;
                unsigned int collumn = pixels / ppm->height;
                
                switch (colour)
                {
                case 0:
                    ppm->pixelMatrix[collumn][row].red = number;
                    break;
                case 1:
                    ppm->pixelMatrix[collumn][row].green = number;
                    break;
                case 2:
                    ppm->pixelMatrix[collumn][row].blue = number;
                    break;
                }
                
            }
            
            memset(numChar, 0, sizeof(char) * 6);
            index = 0;
            continue;
        }

        numChar[index] = ch;
        index = index + 1;
    }
    

    return ppm;
}

void outputPPMFile(struct PPM * ppm, char * outFileName)
{
    FILE * fout = fopen(outFileName, "w");
    
    fprintf(fout, "%s\n", ppm->format);
    fprintf(fout, "%d %d\n", ppm->width,ppm->height);
    fprintf(fout, "%u \n", ppm->colourMax);

    for (size_t i = 0; i < ppm->width; i++)
    {
        for (size_t j = 0; j < ppm->height; j++)
        {
            struct Pixel pxl = ppm->pixelMatrix[i][j];
            fprintf(fout, "%u %u %u\n", pxl.red, pxl.green, pxl.blue);
        }
    }

    fclose(fout);

    free(fout);
}


void showPPM(struct PPM * im)
{
    printf("printing ppm file\n");
    printf("\n%s\n", im->format);
    printf("%u %u\n", im->width, im->height);
    printf("%u\n",im->colourMax);

    for (size_t i = 0; i < im->width; i++)
    {
        for (size_t j = 0; j < im->height; j++)
        {
            struct Pixel pxl = im->pixelMatrix[i][j];
            printf("%u %u %u\n", pxl.red, pxl.green, pxl.blue);
        }
    }
    
    printf("\n\n");
}


void freePPM(struct PPM * ppm) 
{
    for (size_t i = 0; i < ppm->width; i++)
    {
        struct Pixel * pixel = (struct Pixel *) ppm->pixelMatrix[i];
        free(pixel);
    }
    free((struct Pixel *) ppm->pixelMatrix);
    free(ppm);
}



void wPixelValue(struct Pixel * pixel, int number)
{
    int maxIterations = 1, i = 3;

    while ( number > auxNumber(i)  )
    {
        maxIterations++;
        i += 3;
    }

    i = 0;

    while (i < maxIterations)
    {
        int shift = i * 3;
        int bit3 = auxNumber(3) & (number >> shift);
        
        int bit = bit3 & 1;
        pixel->red = wBitPosition(pixel->red, bit, i);
        
        bit = (bit3 >> 1) & 1;
        pixel->green = wBitPosition(pixel->green, bit, i);
        
        bit = (bit3 >> 2) & 1;
        pixel->blue = wBitPosition(pixel->blue, bit, i);
        
        i++;
    }

}

int rPixelValue(struct Pixel * pixel, int length)
{
    int numberRead = 0;
    for (size_t i = 0; i < length; i++)
    {
        int shift = i * 3;

        int rBit = rBitPosition(pixel->red,i);
        numberRead = numberRead | (rBit << shift);

        shift++;
        int gBit = rBitPosition(pixel->green,i);
        numberRead = numberRead | (gBit << shift);

        shift++;
        int bBit = rBitPosition(pixel->blue,i);
        numberRead = numberRead | (bBit << shift);
    }
    return numberRead;
}




void intToBin(unsigned int number){
    if (number > 1){
        intToBin(number/2);
    }

    printf("%u",number % 2);
};


int auxNumber(int n)
{
    if( n == 0) return 0;

    if( n == 1) return 1;
    
    return (auxNumber(n - 1) << 1) | 1;
}


int wBitPosition(int value, int bit, int pos) 
{
    if ( bit == 0 )
    {
        return value = value & ~(1UL << pos);
    } 
    else
    {
        return value = value | (1UL << pos);
    }
}

int rBitPosition(int value, int pos) 
{
    return (value >> pos) & 1UL;
}

int strLength(char * string)
{
    char c = string[0], counter = 0;
    while ( c != '\0')
    {
        counter++;
        c = string[counter];
    }

    return (int) counter;
}