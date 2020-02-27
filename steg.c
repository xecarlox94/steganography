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

void intToBin(unsigned int number){
    if (number > 1){
        intToBin(number/2);
    }

    printf("%u",number % 2);
};

void printPixel(struct Pixel pixel){
    
}

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
    } else if ( number <= auxNumber(6) )
    {
        maxIterations = 2;
    } else if ( number <= auxNumber(9) )
    {
        maxIterations = 3;
    }

    while (i < maxIterations)
    {
        int shift = i * 3;
        int bit3 = auxNumber(3) & (number >> shift);
        // printf("shift: %d, pos: %d, number to write: ", shift, i);
        // intToBin(bit3);
        // printf("\n");
        
        int bit = bit3 & 1;
        // printf("bit 0: ");
        // intToBin(bit);
        // printf("\n");
        pixel->red = wBitPosition(pixel->red, bit, i);
        
        bit = (bit3 >> 1) & 1;
        // printf("bit 1: ");
        // intToBin(bit);
        // printf("\n");
        pixel->green = wBitPosition(pixel->green, bit, i);
        
        bit = (bit3 >> 2) & 1;
        // printf("bit 2: ");
        // intToBin(bit);
        // printf("\n\n");
        pixel->blue = wBitPosition(pixel->blue, bit, i);
        
        i++;
    }

}

int rPixelValue(struct Pixel pixel, int length)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("bit R at pos %d: %d, ", i, rBitPosition(pixel.red,i));
        printf("bit G at pos %d: %d, ", i, rBitPosition(pixel.green,i));
        printf("bit B at pos %d: %d \n\n", i, rBitPosition(pixel.blue,i));
    }
}

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

    // showPPM(ppm);



    // for (size_t i = 32; i < 64; i++)
    // {
    //     printf("%d: ", i);
    //     intToBin(i);
    //     printf("\n");
    // }


    

    struct Pixel pixel;

    pixel.red = 16;
    pixel.green = 16;
    pixel.blue = 16;

    printf("original R: ");
    intToBin(pixel.red);
    printf("\n");
    printf("original G: ");
    intToBin(pixel.green);
    printf("\n");
    printf("original B: ");
    intToBin(pixel.blue);
    printf("\n\n");



    
    int n = 273;

    printf("\n\noriginal number to write: ");
    intToBin(n);
    printf("\n\n");


    wPixelValue(&pixel,n);


    // int temp = auxNumber(3) & (n >> 6);

    // intToBin(t);
    // printf("\n\n");
    // pixel.blue = wBitPosition(pixel.blue, t, pos);



    // printf("modified R: ");
    // int t = (temp >> 0) & 1;
    // printf("bit 0: ");
    // intToBin(t);
    // printf("\n");
    // pixel.red = wBitPosition(pixel.red, t, pos);
    
    // t = (temp >> 1) & 1;
    // printf("bit 1: ");
    // intToBin(t);
    // printf("\n");
    // pixel.green = wBitPosition(pixel.green, t, pos);
    
    // t = (temp >> 2) & 1;
    // printf("bit 2: ");
    // intToBin(t);
    // printf("\n\n");
    // pixel.blue = wBitPosition(pixel.blue, t, pos);



    printf("modified R: ");
    intToBin(pixel.red);
    printf("\n");
    printf("modified G: ");
    intToBin(pixel.green);
    printf("\n");
    printf("modified B: ");
    intToBin(pixel.blue);
    printf("\n\n");

    rPixelValue(pixel,3);

    // int pos = 0;
    // printf("bit R at pos %d : %d \n", pos, rBitPosition(pixel.red,pos));
    // printf("bit G at pos %d : %d \n", pos, rBitPosition(pixel.green,pos));
    // printf("bit B at pos %d : %d \n", pos, rBitPosition(pixel.blue,pos));


    // for (size_t i = 0; i < 3; i++)
    // {
    //     printf("bit %d: %d\n", i, ( temp & 1));
    // }
    





    // int nmb = 273;

    // printf("original number: ");
    // intToBin(nmb);
    // printf("\n");

    // int tmp;

    // for (size_t i = 0; i <=6; i+=3)
    // {
    //     int tmp = auxNumber(3) & (nmb >> i);

    //     printf("number %d: ", (i/3));
    //     intToBin(tmp);
    //     printf("\n");
    // }











    // fclose(file);  RETURNS ERROR !!!!!!!!!!!!!!!

    // printf("\n\nppm\n{\n\twidth: %u;\n\theight: %u;\n\tcolourMax: %u;\n{\n", ppm->width,ppm->height,(int) ppm->colourMax);


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


struct PPM * getPPM(FILE * f)
{
    char ch = 65, prevCh, chCounter = 0, numChar[6];
    // * numChar = (char *) malloc(sizeof(char) * 4);
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
        
        // printf("\n%c,(%u) -> previous: %c,(%u)",ch,(int) ch, prevCh, (int) prevCh);

        if(ch == 32 || prevCh != 32 && ch == 10 )
        {


            values = values + 1;


            numChar[chCounter + 1] = '\0';
            

            // printf("%u", chCounter);

            int i = 0, j;
            double len = chCounter - 1;

            // printf("%f\n",len);
            // printf("%u\n",chCounter);
            // printf("power: %u\n", (int) pow((double) 10, 4));
            while (chCounter > i)
            {
                j = (int) (numChar[i] - 48);
                // printf("j: %u\n", j);

                if (j < 0) j = 0;

                // printf("char: %c\n", numChar[i]);
                // printf("i: %u, len: %f\n", i, len);

                // printf("power: %u\n", (int) pow((double) j, len));

                // printf("power: %u\n", (int) ( j * pow(10.0, len)));
                
                number = number + ( (int) ( j * pow(10.0, len)) );
                // printf("number: %u\n\n", number);
                i = i + 1;
                len = len - 1;
                // chCounter = chCounter - 1;
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

                // printf("values: %u, pixels: %u, number: %u, colour: %u", tempValues, pixels, number, colour);
                // printf(", row: %u, collumn: %u\n\n", row, collumn);
                
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

            // printf("current value: %u", values);
            
                
            // printf("\nstring: %s ,final number: %u, code: %u\n", numChar, number, (int) ch);
            // printf("\nVALUE!!!");
            // printf("\ncurrent value %u\n\n", values);
            

            // printf("\n%s", numChar);

            // 30, 60, 85

            // if (values >= 8) break;



            memset(numChar, 0, sizeof(char) * 6);
            chCounter = 0;
            continue;
        }

        // printf("\nchar: %c, code: %u", ch, (int) ch);
        numChar[chCounter] = ch;

        chCounter = chCounter + 1;

    }

    // values = values - 3;S
    // printf("\n\n%u values!\n", values);
    // printf("is it divisible by 3? %s\n", (values % 3 == 0? "Yes": "No"));
    // printf("height * width = %u\n",(ppm->height * ppm->width));
    // printf("(height * width) / total values = %f\n", (float) (ppm->height * ppm->width) / (float) values);
    // printf("height = %u\n", values / (ppm->width * 3));
    // printf("is this the correct dimension? %s", ( (ppm->height * ppm->width * 3) == values? "Yes": "No"));
    // printf("");
    

    return ppm;
}