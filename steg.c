#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// #ifdef _WIN32
// #include <io.h>
// #else
// #include <unistd.h>
// #endif


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
        printf("File \"%s\" not found!\n\n", argv[1]);

        exit(0);
    }
    
    ppm = getPPM(file);

    // showPPM(ppm);
    
    char command = argv[2][0];

    if ( command == 'e' )
    {
        int secret;
        char message[32];
        printf("Enter the secret: ");
        scanf("%d", &secret);
        printf("Enter the message: ");
        scanf("%s", message);
        
        encode(ppm,message,strLength(message),secret);

        decode(ppm,secret);
    } 
    else if ( command == 'd' )
    {
        // printf("Decoding message from file.\nPlease type your message and secret\n");
        int secret;
        printf("Secret: ");
        scanf("%d",&secret);
        printf("Secret: %d\n", secret);
    }
    






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

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret)
{
    srand(secret);
    unsigned int max = im->height*im->width;
    // printf("max value ppm: %u\n",max);

    unsigned char counter = 0, c;
    // printf("size of c: %d\n",sizeof(c));
    
    unsigned int pos, row, collumn;

    while (1)
    {
        // c = message[counter];

        c = 170;

    printf("FIRST character: ");
    intToBin(c);
    printf("\n");

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

        // bits = bits & auxNumber(8);


        if(c == 0) break;
        // printf("char: %c, ascii: %d\n", c, (int) c);

        break;
        
        counter++;
    }

    return im;
}

char * decode(struct PPM * im, unsigned int secret)
{
    srand(secret);
    unsigned int max = im->height*im->width;
    
    unsigned int pos, row, collumn;

    printf("\n\nDECODING!!!!!!!!!!!!!!!!!!\n\n");
    
    unsigned char a = 0;

    printf("INITIAL character: ");
    intToBin(a);
    printf("\n");

    while (1)
    {

        for (size_t i = 0; i < 3; i++)
        {
            pos = rand() % max;
            row = pos % im->height;
            collumn = pos / im->height;

            struct Pixel * pxl = &(im->pixelMatrix[collumn][row]);

            int three_bits = rPixelValue(pxl,1);

            printf("bit %d: ", i);
            intToBin(three_bits);
            printf("\n\n");

            int shift = 3 * i;
            a = a | (three_bits << shift);

            printf("CURRENT CHAR: ");
            intToBin(a);
            printf("\n\n");
        }


        printf("FINAL CHARACTER: %d\n\n",(int) a);

        break;
    }

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