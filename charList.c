#include <stdio.h>
#include <stdlib.h>

#include "charList.h"

// definitions of these functions on "charList.h"  file

void insertCharList(struct CharNode * chNode, char c)
{
    // if it is the first list node
    if ( chNode->length == 0 )
    {
        // set initial values for character, length and next node
        chNode->ch = c;
        chNode->length= 1;
        chNode->nextNode = NULL;

        return;
    }

    // store original pointer if is necessary to iterate through the list
    struct CharNode * temp = chNode;

    while (1)
    {
        if (temp->nextNode != NULL)
        {
            // if there is another node 
            // increase length
            temp->length++;

            // change temp pointer to next node
            temp = temp->nextNode;
        }
        else
        {
            // if there is no next node
            // increment current node lentgh
            temp->length++;

            // create new node
            temp->nextNode = (struct CharNode *) malloc(sizeof(struct CharNode));

            // change pointer for next node
            temp = temp->nextNode;
            // set its values for character, length and next node
            temp->ch = c;
            temp->length = 1;
            temp->nextNode = NULL;
            return;
        }
    }
}

void trasverseCharList(struct CharNode * chNode)
{
    // stores first pointer address in a iterative temporary node
    struct CharNode * temp = chNode;

    // iterate while there is a next nodes
    while (temp != NULL)
    {

        // print node
        printf("character: %c, length: %d \n",temp->ch,temp->length);

        // go to following node
        temp = temp->nextNode;
    }
    
}


void freeMem(struct CharNode * chNode)
{
    // while there is a next node
    if (chNode->nextNode)
    {
        // call this function for following node
        freeMem(chNode->nextNode);
    }
    
    // free pointer for current node
    free(chNode);
}

char * getMessage(struct CharNode * chNode)
{
    // allocates memory dinamically depending on the length of the list
    char * msg = (char *) malloc(sizeof(char) * chNode->length);

    // sets the initial index and stores initial pointer address on a temporary node
    char index = 0;
    struct CharNode * temp = chNode;

    // while there is a next node
    while (temp != NULL)
    {
        // store node's character
        char ch = temp->ch;

        // insert it correctly in the message array
        msg[index] = ch;

        // trasverse for following node
        temp = temp->nextNode;

        // increase index
        index++;
    }

    freeMem(chNode);
    
    return msg;
}