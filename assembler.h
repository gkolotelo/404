// assembler.cpp
// 


#include <string>
#include <list>
#include  <iomanip>

using namespace std;

#ifndef TYPES_H
    #include "types.h"
#endif
#ifndef ERRORS_H
    #include "errors.h"
#endif
#define ASSEMBLER_H

#define LABEL_NOT_DEFINED -1
#define MAX_MEMORY_LINES 2048


/* Classe para mapa de enderecos de labels e nomes definidos pela diretiva .set */
class AddressMap {
 public:
    std::list<AddressElement> addrList;
    std::list<AddressElement>::iterator addrIterator;
 public:
    AddressElement* setAddress(string _name, int _addr);
    void set2();
    int getAddress(string _name);
    AddressMap() {
        addrIterator = addrList.begin();
    }
};

/*  Classe para o mapa de memoria.
    Organiza metadados sobre cada elemento (Element) lido da entrada 
    em elementos do mapa de memoria (MemoryElement).
*/
class MemoryMap {
 public:
    list<MemoryElement> memoryList;
    list<MemoryElement>::iterator memoryIterator;

    AddressMap *addrMap;

    int cursor;  //Par=esquerda, Impar=Direita

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
    void printMemoryMap(fstream *outputFS);
};
