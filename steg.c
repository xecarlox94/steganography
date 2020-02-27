#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


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


struct PPM * getPPM(FILE * f);

void showPPM(struct PPM * im);

// void intToBin(unsigned int number){
//     if (number > 1){
//         intToBin(number/2);
//     }

//     printf("%u",number % 2);
// };


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



void wPixelValue(struct Pixel * pixel, int number)
{
    int maxIterations = 0, i = 0;

    if ( number <= auxNumber(3) )
    {
        maxIterations = 1;
    } 
    else if ( number <= auxNumber(6) )
    {
        maxIterations = 2;
    } 
    else if ( number <= auxNumber(9) )
    {
        maxIterations = 3;
    }

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

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret);

char * decode(struct PPM * im, unsigned int secret);

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
        printf("File not found!");

        exit(0);
    }
    
    
    ppm = getPPM(file);

    showPPM(ppm);
    






    // fclose(file);  RETURNS ERROR !!!!!!!!!!!!!!!
    free(file);
    // FREEING MEMORY
    // for (size_t i = 0; i < ppm->width; i++)
    // {
    //     struct Pixel * pixel = (struct Pixel *) ppm->width + i;
    //     free(pixel);
    // }
    // free((struct Pixel *) ppm->width);
    free(ppm);


    return 0;
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

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret)
{
    return im;
}

char * decode(struct PPM * im, unsigned int secret)
{
    return "asadasd";
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