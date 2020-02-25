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

int main(int argc, char ** argv)
{
    FILE * file;

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
    
    
    struct PPM * ppm = getPPM(file);

    showPPM(ppm);






    // fclose(file);  RETURNS ERROR !!!!!!!!!!!!!!!

    // printf("\n\nppm\n{\n\twidth: %d;\n\theight: %d;\n\tcolourMax: %d;\n{\n", ppm->width,ppm->height,(int) ppm->colourMax);


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
    printf("%d %d\n", im->width, im->height);
    printf("%d\n",im->colourMax);

    for (size_t i = 0; i < im->width; i++)
    {
        for (size_t j = 0; j < im->height; j++)
        {
            struct Pixel pxl = im->pixelMatrix[i][j];
            printf("%d %d %d\n", pxl.red, pxl.green, pxl.blue);
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
        
        // printf("\n%c,(%d) -> previous: %c,(%d)",ch,(int) ch, prevCh, (int) prevCh);

        if(ch == 32 || prevCh != 32 && ch == 10 )
        {


            values = values + 1;


            numChar[chCounter + 1] = '\0';
            

            // printf("%d", chCounter);

            int i = 0, j;
            double len = chCounter - 1;

            // printf("%f\n",len);
            // printf("%d\n",chCounter);
            // printf("power: %d\n", (int) pow((double) 10, 4));
            while (chCounter > i)
            {
                j = (int) (numChar[i] - 48);
                // printf("j: %d\n", j);

                if (j < 0) j = 0;

                // printf("char: %c\n", numChar[i]);
                // printf("i: %d, len: %f\n", i, len);

                // printf("power: %d\n", (int) pow((double) j, len));

                // printf("power: %d\n", (int) ( j * pow(10.0, len)));
                
                number = number + ( (int) ( j * pow(10.0, len)) );
                // printf("number: %d\n\n", number);
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

                // printf("values: %d, pixels: %d, number: %d, colour: %d", tempValues, pixels, number, colour);
                // printf(", row: %d, collumn: %d\n\n", row, collumn);
                
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

            // printf("current value: %d", values);
            
                
            // printf("\nstring: %s ,final number: %d, code: %d\n", numChar, number, (int) ch);
            // printf("\nVALUE!!!");
            // printf("\ncurrent value %d\n\n", values);
            

            // printf("\n%s", numChar);

            // 30, 60, 85

            // if (values >= 8) break;



            memset(numChar, 0, sizeof(char) * 6);
            chCounter = 0;
            continue;
        }

        // printf("\nchar: %c, code: %d", ch, (int) ch);
        numChar[chCounter] = ch;

        chCounter = chCounter + 1;

    }

    // values = values - 3;S
    // printf("\n\n%d values!\n", values);
    // printf("is it divisible by 3? %s\n", (values % 3 == 0? "Yes": "No"));
    // printf("height * width = %d\n",(ppm->height * ppm->width));
    // printf("(height * width) / total values = %f\n", (float) (ppm->height * ppm->width) / (float) values);
    // printf("height = %d\n", values / (ppm->width * 3));
    // printf("is this the correct dimension? %s", ( (ppm->height * ppm->width * 3) == values? "Yes": "No"));
    // printf("");
    

    return ppm;
}