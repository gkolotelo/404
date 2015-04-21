#include <string>
#include <list>
#include  <iomanip>

using namespace std;

#ifndef types_cpp
    #include "types.cpp"
#endif
#define assembler_cpp

#define LABEL_NOT_DEFINED -1
#define MAX_MEMORY_LINES 2048


typedef enum{
    Left,
    Right
} Side;

typedef string LabelType;


typedef struct{
    string name;
    int addr;
}AddressElement;

typedef struct {
    Element *elem;
    int addr;
    Side side;
    AddressElement* addrLink;
} MemoryElement;

class AddressMap {
 public:
    std::list<AddressElement> addrList;
    std::list<AddressElement>::iterator addrIterator;
    
 public:
    AddressElement* setAddress(string _name, int _addr);
    //void set2();
    int getAddress(string _name);
    AddressMap() {
        addrIterator = addrList.begin();
    }
};

class MemoryMap {
 public:
    list<MemoryElement> memoryList;
    list<MemoryElement>::iterator memoryIterator;

    AddressMap *addrMap;

    int cursor;  // left se par, right se impar

    MemoryMap();
    string generateLine(int addr, MemoryElement el1, MemoryElement el2);
    bool getNewCursor(DirectiveContentContainer DCC, int *cursor);
    void align(DirectiveContentContainer DCC, int *cursor);
    string getWordHexStr(string addr);
    string getInstContentHexStr(string addr);
    bool isLast();
    bool isDigit(string in);
    void splitWord(string word, Element *half_1, Element *half_2);
    string GetOpCodeString(MemoryElement ME);
    string GetContentString(MemoryElement ME);

 public:
    void add(Element *el);
    void finishUp();
    void printMemoryMap();
};
