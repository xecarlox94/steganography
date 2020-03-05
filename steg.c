#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "charList.h"


struct Pixel
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
};


struct PPM
{
    const char * format;
    unsigned int width;
    unsigned int height;
    unsigned char colourMax;
    struct Pixel ** pixelMatrix;
};


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


struct PPM * getPPM(FILE * f);

void showPPM(struct PPM * im);

void outputPPMFile(struct PPM * ppm, char * outFileName);

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret);

char * decode(struct PPM * im, unsigned int secret);

void freePPM(struct PPM * ppm);



void wPixelValue(struct Pixel * pixel, int number);

int rPixelValue(struct Pixel * pixel, int length);


int main(int argc, char ** argv)
{
    FILE * file;
    struct PPM * ppm;

    if (argc < 3 || argc > 5)
    {
        printf("\nwrong number of arguments\n\n");
    }

    file = fopen(argv[1], "r");

    if(file == NULL)
    {
        printf("File \"%s\" not found!\n\n", argv[1]);

        exit(0);
    }
    
    ppm = getPPM(file);
    
    char command = argv[2][0];

    if ( command == 'e' )
    {
        int secret;
        char message[32];

        printf("Message: ");
        fgets(message,32,stdin);

        printf("\n\nEnter the secret: ");
        scanf("%d", &secret);
        
        struct PPM * encodedPPM = encode(ppm, message, strLength(message), secret);

        outputPPMFile(encodedPPM, argv[3]);

    } 
    else if ( command == 'd' )
    {
        int secret;
        printf("Secret: ");
        scanf("%d",&secret);

        char * msg = decode(ppm,secret);

        printf("Encoded message: %s\n\n", msg);

    } 
    else if ( command == 's')
    {
        showPPM(ppm);
    }


    fclose(file);

    free(file);    

    freePPM(ppm);



    return 0;
}

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret)
{
    srand(secret);
    unsigned int max = im->height*im->width;

    unsigned char counter = 0, c;
    
    unsigned int pos, row, collumn;

    while (counter <= mSize)
    {
        c = message[counter];
        
        if( c == 10) {
            counter++;
            continue;
        }

        for (size_t i = 0; i < 3; i++)
        {
            pos = rand() % max;
            row = pos % im->height;
            collumn = pos / im->height;

            int shift = 3 * i;
            int three_bits = (c >> shift ) & auxNumber(3);

            struct Pixel * pxl = &(im->pixelMatrix[collumn][row]);

            wPixelValue(pxl, three_bits);


        }

        counter++;
    }

    return im;
}

char * decode(struct PPM * im, unsigned int secret)
{
    srand(secret);

    unsigned int max = im->height*im->width;
    
    unsigned int pos, row, collumn;

    struct CharNode * charList = (struct CharNode *) malloc(sizeof(struct CharNode));
    charList->length = 0;
    
    unsigned char c,counter=0;

    while (1)
    {
        c = 0;

        for (size_t i = 0; i < 3; i++)
        {

            pos = rand() % max;
            row = pos % im->height;
            collumn = pos / im->height;

            struct Pixel * pxl = &(im->pixelMatrix[collumn][row]);

            int three_bits = rPixelValue(pxl,1);

            int shift = 3 * i;
            c = c | (three_bits << shift);
        }

        
        if( c == 0) break;



        counter++;


        insertCharList(charList, c);

    }

    

    char * message = getMessage(charList);

    return message;
}


struct PPM * getPPM(FILE * f)
{
    char ch = 65, prevCh, chCounter = 0, numChar[6];
    int values = 0, valCounter = 0;
    
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
            
            values = values + 1;
            
            numChar[chCounter + 1] = '\0';

            int i = 0, j;
            double len = chCounter - 1;
            
            while (chCounter > i)
            {
                j = (int) (numChar[i] - 48);

                if (j < 0) j = 0;

                number = number + ( (int) ( j * pow(10.0, len)) );

                i = i + 1;
                len = len - 1;
            }
            
            if (values <= 3)
            {
                switch (values)
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
                unsigned int tempValues = values - 4;
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
            chCounter = 0;
            continue;
        }

        numChar[chCounter] = ch;
        chCounter = chCounter + 1;
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