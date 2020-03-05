


struct CharNode
{
    char ch, length;
    struct CharNode * nextNode;
};


void insertCharList(struct CharNode * chNode, char c);

void trasverseCharList(struct CharNode * chNode);


void freeMem(struct CharNode * chNode);

char * getMessage(struct CharNode * chNode);