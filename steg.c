#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "charList.h"


/*

USAGE

Encoding:

./steg [path/fileToEncode] s [path/finalFileEncoded]

Decoding:

./steg [path/file] d

Showing ppm:

./steg [path/file] s

*/


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

    // storing the number of values
    int numValues = 0;
    
    // creates the ppm to store the file PPM data structure
    struct PPM * ppm = (struct PPM *) malloc(sizeof(struct PPM));
    
    // while is no the end of the file
    while ( ch != EOF)
    {
        // initialises the current number
        int number = 0;
        
        // stores the previous character
        prevCh = ch;
        
        // gets the current character
        ch = getc(f);


        // if the line is the format or comment, ignore line
        if(ch == '#' || ch == 'P')
        {
            while (ch != '\n')
            {
                ch = getc(f);
            }
            ch = 65;
            continue;
        }


        // if the current charater is the space on 
        // or the current character is new line and the previous was different from space character
        // calculate the number stored as a string
        if(ch == 32 || prevCh != 32 && ch == 10 )
        {
            // increment number of values 
            numValues = numValues + 1;
            
            // add the end of string character
            numChar[index + 1] = '\0';

            // initialise number character index and the current number in the j integer file
            int i = 0, j;

            // gets the length of the number character
            double len = index - 1;
            
            // loops while index is less than the index
            while (index > i)
            {
                // gets the number correspondent to the number character
                j = (int) (numChar[i] - 48);

                // if number is negative, set it to zero
                if (j < 0) j = 0;

                // construct number from the characters
                number = number + ( (int) ( j * pow(10.0, len)) );

                // increment index
                // decrement length
                i = i + 1;
                len = len - 1;
            }
            
            // if the number of values is less than 3
            // store the header ppm values
            if (numValues <= 3)
            {
                switch (numValues)
                {
                    // if it is the first number
                    case 1:
                        // store width and format
                        ppm->width = number;
                        ppm->format = "P3";
                        break;

                    // if it is the second value
                    case 2:
                        // store height
                        ppm->height = number;

                        // now it is possible to generate a 2d pixel matrix dinamically 
                        // because width and height and width are known

                        // dinamically generate a pixel matrix pointer, with width length
                        ppm->pixelMatrix = (struct Pixel **) malloc(sizeof(struct Pixel *) * ppm->width);

                        // generate an pointer of pointer per each pointer of the main pixel pointer created previously
                        for(size_t i = 0; i < ppm->width; i++)
                        {
                            ppm->pixelMatrix[i] = (struct Pixel *) malloc(sizeof(struct Pixel) * ppm->height);
                        }
                        break;

                    case 3:
                        // storing the max colour value
                        ppm->colourMax = (unsigned char) number;
                        break;
                }
            } 
            else
            {
                // getting the current number of colours
                unsigned int numberColours = numValues - 4;

                // getting the current number of pixels
                unsigned int pixels = numberColours / 3;

                // determining if colour is the current value
                char colour = numberColours % 3;

                // determining the correct row for this pixel
                unsigned int row = pixels % ppm->height;

                // determining the correct collumn for this pixel
                unsigned int collumn = pixels / ppm->height;
                
                switch (colour)
                {
                    case 0:
                        // if colour is equal to zero, assign the number to colour red
                        ppm->pixelMatrix[collumn][row].red = number;
                        break;
                    case 1:
                        // if colour is equal to one, assign the number to colour green
                        ppm->pixelMatrix[collumn][row].green = number;
                        break;
                    case 2:
                        // if colour is equal to two, assign the number to colour blue
                        ppm->pixelMatrix[collumn][row].blue = number;
                        break;
                }
                
            }
            
            // cleaning the character array memory
            memset(numChar, 0, sizeof(char) * 6);

            // setting index to zero
            index = 0;
            continue;
        }

        // inserting charater to current character array
        numChar[index] = ch;

        // incrementing index
        index = index + 1;
    }
    

    return ppm;
}

void outputPPMFile(struct PPM * ppm, char * outFileName)
{
    // initialises the file writer
    FILE * fout = fopen(outFileName, "w");
    
    // writes format, width, height and coulour maximum
    fprintf(fout, "%s\n", ppm->format);
    fprintf(fout, "%d %d\n", ppm->width,ppm->height);
    fprintf(fout, "%u \n", ppm->colourMax);

    // writes the coulours, pixel by pixel, per each line
    for (size_t i = 0; i < ppm->width; i++)
    {
        for (size_t j = 0; j < ppm->height; j++)
        {
            struct Pixel pxl = ppm->pixelMatrix[i][j];
            fprintf(fout, "%u %u %u\n", pxl.red, pxl.green, pxl.blue);
        }
    }

    // closes file writer
    fclose(fout);

    // frees file pointer
    free(fout);
}


void showPPM(struct PPM * im)
{
    /// prints format, width, height and coulour maximum
    printf("printing ppm file\n");
    printf("\n%s\n", im->format);
    printf("%u %u\n", im->width, im->height);
    printf("%u\n",im->colourMax);

    // prints the coulours, pixel by pixel, per each line
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
    // frees each pixel pointer memory
    for (size_t i = 0; i < ppm->width; i++)
    {
        // goes through the main array and frees each pixel pointer
        struct Pixel * pixel = (struct Pixel *) ppm->pixelMatrix[i];
        free(pixel);
    }
    
    // frees the pixel pointer of pointer
    free((struct Pixel *) ppm->pixelMatrix);

    // frees ppm structure
    free(ppm);
}



void wPixelValue(struct Pixel * pixel, int number)
{
    // stores the number of iterations, for longer number than 3 bits ones
    // store the variable i that stores the number of bits
    int maxIterations = 1, i = 3;

    // determines how long in bits the number is
    while ( number > auxNumber(i) )
    {
        maxIterations++;
        i += 3;
    }
    
    // i variable will now stores which colour collumn the number is being writen on
    i = 0;

    while (i < maxIterations)
    {
        // determines the current shift
        int shift = i * 3;

        // gets 3 bits from the number to be written
        int bit3 = auxNumber(3) & (number >> shift);
        
        // gets the first bit of the 3 bits
        int bit = bit3 & 1;
        // writes the bit in the correct position on the red colour
        pixel->red = wBitPosition(pixel->red, bit, i);
        
        // gets the second bit of the 3 bits
        bit = (bit3 >> 1) & 1;
        // writes the bit in the correct position on the red green
        pixel->green = wBitPosition(pixel->green, bit, i);
        
        // gets the third bit of the 3 bits
        bit = (bit3 >> 2) & 1;
        // writes the bit in the correct position on the red blue
        pixel->blue = wBitPosition(pixel->blue, bit, i);
        
        i++;
    }

}

int rPixelValue(struct Pixel * pixel, int length)
{
    // store the number being read from the pixel
    int numberRead = 0;

    // iterates through each coulour collumn
    for (size_t i = 0; i < length; i++)
    {
        // determines the shift for the colour
        int shift = i * 3;

        // gets the bit from the colour red in the correct position
        int rBit = rBitPosition(pixel->red,i);
        // writes the bit in the number to be read from the pixel
        numberRead = numberRead | (rBit << shift);

        // increment the shift
        shift++;
        // gets the bit from the colour green in the correct position
        int gBit = rBitPosition(pixel->green,i);
        // writes the bit in the number to be read from the pixel
        numberRead = numberRead | (gBit << shift);

        // increment the shift
        shift++;
        // gets the bit from the colour blue in the correct position
        int bBit = rBitPosition(pixel->blue,i);
        // writes the bit in the number to be read from the pixel
        numberRead = numberRead | (bBit << shift);
    }

    // returns the number to be read
    return numberRead;
}




void intToBin(unsigned int number){

    // while the number is bigger than 1
    if (number > 1){
        // call the function recursively with half of the current number
        intToBin(number/2);
    }

    // gets the remainder of 2, which is either 1 or 0
    printf("%u",number % 2);
};


int auxNumber(int n)
{
    // if zero returns 0
    if( n == 0) return 0;

    // if one returns 1
    if( n == 1) return 1;
    
    // else returns the result of bitwise 1 OR with result ot the number minus 1 with left shift of one
    // creates a binary number composed of 1's with length n
    return (auxNumber(n - 1) << 1) | 1;
}


int wBitPosition(int value, int bit, int pos) 
{
    if ( bit == 0 )
    {
        // if the bit to be written is 0

        // then do a AND bit operation with the colour value 
        // and the negation of 1 bits with left shift equal to pos
        return value = value & ~(1UL << pos);
    } 
    else
    {
        // if the bit to be written is 1

        // then do a OR bit operation with the colour value 
        // and 1 bits with left shift equal to pos
        return value = value | (1UL << pos);
    }
}

int rBitPosition(int value, int pos) 
{
    // reads the bit value from a specific position

    // AND bitswise operation with value with right shift of position value
    // with 1 bit 
    return (value >> pos) & 1UL;
}

int strLength(char * string)
{
    // stores current character
    char c;

    // initialises the counter to 0 
    int counter = 0;

    // loops until the end of string
    while ( c != '\0')
    {
        // increments the counter
        counter++;

        // store the character at position equal to counter
        c = string[counter];
    }

    return counter;
}