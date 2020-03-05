#include <stdio.h>
#include <stdlib.h>

#include "charList.h"


void insertCharList(struct CharNode * chNode, char c)
{
    if ( chNode->length == 0 )
    {

        chNode->ch = c;
        chNode->length= 1;
        chNode->nextNode = NULL;

        return;
    }

    struct CharNode * temp = chNode;

    while (1)
    {
        if (temp->nextNode != NULL)
        {
            temp->length++;
            temp = temp->nextNode;
        }
        else
        {
            temp->length++;
            temp->nextNode = (struct CharNode *) malloc(sizeof(struct CharNode));
            temp = temp->nextNode;

            temp->ch = c;
            temp->length = 1;
            temp->nextNode = NULL;
            return;
        }
    }
}

void trasverseCharList(struct CharNode * chNode)
{
    struct CharNode * temp;
    temp = chNode;

    while (temp != NULL)
    {
        printf("character: %c, length: %d \n",temp->ch,temp->length);

        temp = temp->nextNode;
    }
    
}


void freeMem(struct CharNode * chNode)
{
    if (chNode->nextNode)
    {
        freeMem(chNode->nextNode);
    }
    free(chNode);
}

char * getMessage(struct CharNode * chNode)
{
    char * msg = (char *) malloc(sizeof(char) * chNode->length);

    char counter = 0;
    struct CharNode * temp = chNode;

    while (temp != NULL)
    {
        char ch = temp->ch;
        msg[counter] = ch;
        temp = temp->nextNode;
        counter++;
    }

    freeMem(chNode);
    
    return msg;
}