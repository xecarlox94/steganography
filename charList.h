

// singly linked Character List data structure
struct CharNode
{
    // stores the character
    char ch;
    
    // stores the length of the list ahead of this node
    char length;

    // stores the pointer for the next CharNode  
    struct CharNode * nextNode;
};


// inserts a character node on the end of a list
void insertCharList(struct CharNode * chNode, char c);


// trasverse per each list node
void trasverseCharList(struct CharNode * chNode);

// recursively frees memory from the list
void freeMem(struct CharNode * chNode);

// gets message stored in the list
char * getMessage(struct CharNode * chNode);